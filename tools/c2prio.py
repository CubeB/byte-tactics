"""Show C2's own register candidates for one function: priority, order, register.

    uv run tools/c2prio.py 0x4cf570
    uv run tools/c2prio.py 0x4cf570 build/scratch/0x4cf570/try.cpp
    uv run tools/c2prio.py 0x4cf570 --trace      # also every colouring step

This compiles the function's file with the real back end (C2.EXE) running under
a debugger and reads the global register allocator's own data
(docs/c2-regalloc.md) for that one function. For each register candidate (a
web of a local, parameter, global, compiler temporary or constant) it prints,
in the order FUN_0041bdd7 sorted them:

  - its priority (candidate +0x0c, from FUN_0040ee1d) and the tie key at +0x40
    (the number of the last tuple that writes it); the list is sorted by
    priority, then +0x40, both larger first;
  - its spill cost (+0x3c) and reference count (+0x24);
  - the registers it is still allowed when the list is built;
  - the register it gets, or `split` with the registers of its pieces, or
    `memory` (`immediate` for a constant);
  - a name: the local, parameter or global, `temp` for a compiler temporary,
    `local temp` for an unnamed front-end local, a constant's value, and the
    source lines that read or write it.

--trace adds every colouring step: each register choice with the registers
still allowed and FUN_0041b785's costs, splits, re-sorts and skipped
candidates.

How: a copy of C2.EXE under build/c2prio/<run>/ has `jmp $` at its entry
point. CL runs that copy (/B2), winedbg attaches to it with its gdb server, and
gdb runs this same file as a Python script that puts the entry bytes back and
records what the allocator does at a dozen addresses. The compile then finishes
normally. Needs gdb with Python (the distribution's gdb package); several runs
can go at once. To debug the tool itself, --keep keeps the run directory with
gdb.log, and C2PRIO_DEBUG=1 logs gdb's remote protocol into it.
"""

import json
import os
import struct
import sys

try:
    import gdb  # this file is also the script gdb runs inside the debugger
except ImportError:
    gdb = None


# --- C2.EXE (VC++ 5.0 SP3) addresses -------------------------------------------

C2_SHA256 = "e75aecaf4073b0817ffb638cae5fff636b2e2d1a090daabf8f68dbc954515fae"
ENTRY = 0x452797          # PE entry point; the copy spins here (EB FE) until gdb attaches
ENTRY_BYTES = b"\x55\x8b"  # its first two bytes, push ebp; mov ebp, esp

REFS = 0x416A8D           # FUN_00416a8d entry (from FUN_00414a64), once per function; ecx = function
NEWCAND = 0x40EC45        # FUN_0040ebb6's ret: a new candidate in eax (ids of freed ones are reused)
DEFREF = 0x416AD3         # FUN_00416a8d: a tuple writes a candidate; edi = candidate, eax = tuple number
USEREF = 0x416C94         # FUN_00416a8d: a tuple reads a candidate; eax = candidate
DRIVER = 0x416E6A         # FUN_00416e6a entry, the global allocator
SORTED = 0x4172F4         # FUN_0041bdd7 has built the sorted list; ebx = class
LOWSPILL = 0x417013       # spill cost <= 0, not worth a register as it stands; esi = candidate
DEFER = 0x417246          # FUN_0045aaf9 put it back into the list; ecx = candidate
CHOOSE = 0x4171CF         # call FUN_0041b785; esi = candidate, edi = its interference set
CHOSEN = 0x4171D4         # back from FUN_0041b785; register at candidate +0x10
SPLIT = 0x439385          # FUN_00439385 entry, live-range split; ecx = candidate
RESORT = 0x416FE2         # priorities recomputed (FUN_0040ee1d again) and the list re-sorted
END = 0x417317            # every register class done

# Return addresses of the FUN_0040ebb6 calls that make the pieces of a split
# candidate, and the register holding the candidate being split at each.
PIECE_SITES = {0x4386B9: "ebx", 0x438C93: "ebp", 0x438E38: "ebp"}

LINE = 0x48E004           # the current line while FUN_00416a8d walks the tuples
LIST_HEAD = 0x4910D4
HASH = 0x493238           # 1024 buckets of candidates by id & 0x3ff, chained at +0x2c
COSTS = 0x4931D8          # FUN_0041b785's cost of each register (9 ints, by register number)
REG_TABLE = 0x494758      # register descriptors, 0x50 bytes each, by register number

REG_NAMES = {0: "noreg", 1: "eax", 2: "ecx", 3: "edx", 4: "ebx", 5: "esp", 6: "ebp", 7: "esi", 8: "edi"}
ORDER = [1, 2, 3, 7, 8, 4, 6]   # the allocator's register order (0x49b4a8)
LETTERS = {1: "a", 2: "c", 3: "d", 7: "s", 8: "i", 4: "b", 6: "p"}
KINDS = {3: "temp", 4: "local", 5: "param", 7: "global", 13: "const"}


def tracer() -> None:
    """Runs inside gdb: attach, let C2 go, record the target function's allocation."""
    import time
    import traceback

    t0 = time.time()
    cfg = json.load(open(os.environ["C2PRIO_CONFIG"]))
    out = {"functions": [], "error": None}

    def finish():
        out["seconds"] = round(time.time() - t0, 2)
        with open(cfg["out"], "w") as fh:
            json.dump(out, fh)

    debug = ["set debug timestamp on", "set debug remote 1"] if os.environ.get("C2PRIO_DEBUG") else []
    for cmd in ("set pagination off", "set confirm off", "set auto-solib-add off",
                "set breakpoint always-inserted on", "set debuginfod enabled off", *debug):
        try:
            gdb.execute(cmd, to_string=True)
        except gdb.error:
            pass
    try:
        gdb.execute(f"target remote | {cfg['relay']}", to_string=True)
    except gdb.error as e:
        out["error"] = f"could not connect to winedbg: {e}"
        return finish()
    inf = gdb.selected_inferior()
    exe = gdb.current_progspace().filename or ""
    if cfg["exe"] not in exe.lower():
        # Another run's winedbg got the port first: leave its C2 alone.
        gdb.execute("detach", to_string=True)
        out["error"] = f"connected to {exe!r}, not this run's copy of C2"
        return finish()
    inf.write_memory(ENTRY, ENTRY_BYTES)

    def rd(addr, n):
        return bytes(inf.read_memory(addr, n))

    def u32(addr):
        return struct.unpack("<I", rd(addr, 4))[0]

    def cstr(addr):
        if not addr:
            return None
        for size in (256, 32, 4):
            try:
                return rd(addr, size).split(b"\0")[0].decode("latin1")
            except gdb.MemoryError:
                continue
        return None

    def reg(name):
        return int(gdb.selected_frame().read_register(name)) & 0xFFFFFFFF

    def bitset(addr):
        """C2's sparse bit set: a header pointing at chunks {base, next, 32 bits}."""
        if not addr:
            return None
        bits, chunk, n = [], u32(addr), 0
        while chunk and n < 4096:
            base, nxt, word = struct.unpack("<III", rd(chunk, 12))
            bits += [base + i for i in range(32) if word >> i & 1]
            chunk, n = nxt, n + 1
        return bits

    leaves = {}

    def leaf(sym):
        """A candidate's leaf: +4 kind, +0x10 type, +0 storage record (name at
        +0x18, frame offset at +0xc); a constant's value node is at +0x28."""
        if sym in leaves:
            return leaves[sym]
        info = {}
        if sym:
            raw = rd(sym, 0x2C)
            st = struct.unpack_from("<I", raw, 0)[0]
            info = {"kind": raw[4], "type": struct.unpack_from("<H", raw, 0x10)[0]}
            if st:
                sraw = rd(st, 0x1C)
                info["name"] = cstr(struct.unpack_from("<I", sraw, 0x18)[0])
                info["offset"] = struct.unpack_from("<i", sraw, 0xC)[0]
            if raw[4] == 13:
                node = struct.unpack_from("<I", raw, 0x28)[0]
                if node:
                    vraw = rd(node, 0x10)
                    if struct.unpack_from("<I", vraw, 4)[0] == 0x11D:
                        info["value"] = struct.unpack_from("<i", vraw, 0xC)[0]
        leaves[sym] = info
        return info

    def regnum(p):
        if not p:
            return 0
        if REG_TABLE <= p < REG_TABLE + 0x50 * 64 and (p - REG_TABLE) % 0x50 == 0:
            return (p - REG_TABLE) // 0x50
        return -1

    def walk(head, full=True):
        """The candidate list from its head (+0x14 is the next one)."""
        res, seen = [], set()
        while head and head not in seen and len(res) < 100000:
            seen.add(head)
            raw = rd(head, 0x44)
            f = lambda o: struct.unpack_from("<I", raw, o)[0]
            s = lambda o: struct.unpack_from("<i", raw, o)[0]
            d = {"id": f(0x1C), "prio": s(0xC), "k40": f(0x40), "spill": s(0x3C)}
            if full:
                d.update(refs=f(0x24), allowed=bitset(f(0x20)))
            res.append(d)
            head = f(0x14)
        return res

    def hashed():
        res, table = [], rd(HASH, 4096)
        for b in range(1024):
            c, seen = struct.unpack_from("<I", table, 4 * b)[0], set()
            while c and c not in seen:
                seen.add(c)
                raw = rd(c, 0x30)
                res.append((struct.unpack_from("<I", raw, 0x1C)[0], struct.unpack_from("<I", raw, 0)[0]))
                c = struct.unpack_from("<I", raw, 0x2C)[0]
        return res

    kind, names = cfg["match"]   # exact, prefix or substr

    def wanted(name):
        if name is None:
            return False
        if kind == "exact":
            return name in names
        if kind == "substr":
            return any(n in name for n in names)
        return any(name.startswith(n) for n in names)

    cur = None
    fn24 = 0

    def ev(what, c, **extra):
        raw = rd(c, 0x20)
        cur["events"].append(dict(e=what, id=struct.unpack_from("<I", raw, 0x1C)[0],
                                  prio=struct.unpack_from("<i", raw, 0xC)[0], **extra))

    def on_refs():
        """FUN_00416a8d(function, candidates by web): every candidate exists by now."""
        nonlocal cur, fn24
        fn = reg("ecx")
        name = cstr(u32(u32(fn) + 0x18))
        cur = None
        leaves.clear()
        if wanted(name):
            fn24 = u32(fn + 0x24)
            cur = {"name": name, "events": []}
            out["functions"].append(cur)
            for cid, sym in hashed():
                cur["events"].append({"e": "new", "id": cid, "parent": None, "leaf": leaf(sym)})

    def on_new():
        c = reg("eax")
        site = u32(reg("esp"))
        parent = u32(reg(PIECE_SITES[site]) + 0x1C) if site in PIECE_SITES else None
        raw = rd(c, 0x20)
        cur["events"].append({"e": "new", "id": struct.unpack_from("<I", raw, 0x1C)[0], "parent": parent,
                              "leaf": leaf(struct.unpack_from("<I", raw, 0)[0])})

    def on_ref(c, rw):
        # FUN_00416a8d's locals: the tuple at [esp+0x14], its number at [esp+0x20].
        # The line is the tuple's own (relative to the line before the body's
        # `{`), or failing that the last one FUN_00416a8d saw.
        frame = rd(reg("esp") + 0x14, 0x10)
        own = struct.unpack("<H", rd(struct.unpack_from("<I", frame, 0)[0] + 0x10, 2))[0]
        cur["events"].append({"e": "ref", "id": u32(c + 0x1C), "rw": rw,
                              "n": struct.unpack_from("<I", frame, 0xC)[0],
                              "line": own or (u32(LINE) - fn24) & 0xFFFF})

    def on_sorted():
        cur["events"].append({"e": "sorted", "class": reg("ebx"), "list": walk(u32(LIST_HEAD))})

    def on_choose():
        c = reg("esi")
        ev("choose", c, allowed=bitset(u32(c + 0x20)))

    def on_chosen():
        c = reg("esi")
        ev("chosen", c, reg=regnum(u32(c + 0x10)), costs=list(struct.unpack("<9i", rd(COSTS, 36))))

    handlers = {
        NEWCAND: on_new,
        DEFREF: lambda: on_ref(reg("edi"), "w"),
        USEREF: lambda: on_ref(reg("eax"), "r"),
        DRIVER: lambda: cur["events"].append({"e": "driver"}),
        SORTED: on_sorted,
        RESORT: lambda: cur["events"].append({"e": "resort", "list": walk(u32(LIST_HEAD), False)}),
        LOWSPILL: lambda: ev("lowspill", reg("esi")),
        DEFER: lambda: ev("defer", reg("ecx")),
        CHOOSE: on_choose,
        CHOSEN: on_chosen,
        SPLIT: lambda: ev("split", reg("ecx")),
    }
    failed = []

    class Hook(gdb.Breakpoint):
        """Records and lets C2 run on, without a full stop in gdb."""

        def __init__(self, addr, fn):
            super().__init__(f"*{addr:#x}", internal=True)
            self.fn = fn
            self.enabled = False

        def stop(self):
            if cur is None:
                return False
            try:
                self.fn()
                return False
            except Exception:
                failed.append(traceback.format_exc())
                return True

    hooks = [Hook(addr, fn) for addr, fn in handlers.items()]
    # Real stops, where the hooks are switched on and off: each function's
    # candidates are ready at REFS, and its allocation is over at END.
    gdb.Breakpoint(f"*{REFS:#x}", internal=True)
    gdb.Breakpoint(f"*{END:#x}", internal=True)

    exited = []
    gdb.events.exited.connect(exited.append)
    try:
        while not exited and not failed:
            try:
                gdb.execute("continue", to_string=not debug)
            except gdb.error:
                if exited or not inf.threads():
                    break
                raise
            if exited or failed:
                break
            pc = reg("eip")
            if pc == REFS:
                on_refs()
            elif pc == END:
                cur = None
            for h in hooks:
                h.enabled = cur is not None
    except Exception:
        failed.append(traceback.format_exc())
    if failed:
        out["error"] = failed[0]
        try:
            for h in hooks:
                h.enabled = False
            gdb.execute("detach", to_string=True)  # let C2 finish without us
        except Exception:
            pass
    finish()


# --- the host side ---------------------------------------------------------------

def host_main() -> None:
    import argparse
    import hashlib
    import re
    import secrets
    import shlex
    import shutil
    import socket
    import subprocess
    import time
    from pathlib import Path

    sys.path.insert(0, str(Path(__file__).resolve().parent))
    from check import (DEFAULT_FLAGS, FILE_FLAGS, FILE_FLAGS_LINE, FORBIDDEN, annotations, find_source,
                       mangled_prefixes, select_function, winpath)
    from coff import parse_object

    root = Path(__file__).resolve().parent.parent
    tc = root / "toolchain" / "msvc5-sp3"

    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("address", type=lambda s: int(s, 16))
    ap.add_argument("source", type=Path, nargs="?")
    ap.add_argument("--sym", help="substring of the mangled name, if the annotation can't be used")
    ap.add_argument("--flags", default=DEFAULT_FLAGS)
    ap.add_argument("--trace", action="store_true", help="also print every colouring step")
    ap.add_argument("--json", type=Path, help="write the raw trace to this file")
    ap.add_argument("--keep", action="store_true", help="keep the run directory under build/c2prio/")
    args = ap.parse_args()

    if shutil.which("gdb") is None or shutil.which("winedbg") is None:
        sys.exit("c2prio needs gdb and winedbg; ask the human to install gdb (it comes with Python)")
    c2 = tc / "BIN" / "C2.EXE"
    if not c2.exists():
        sys.exit("Toolchain missing, run tools/setup_toolchain.sh")
    if hashlib.sha256(c2.read_bytes()).hexdigest() != C2_SHA256:
        sys.exit(f"{c2} is not the VC++ 5.0 SP3 C2.EXE whose addresses this tool uses")

    src = args.source or find_source(args.address)
    if src is None:
        sys.exit(f"no file under src/ has '// FUNCTION: {args.address:#x}'")
    qualname = next((q for a, q in annotations(src) if a == args.address), None)
    text = src.read_text(errors="replace")
    bad = FORBIDDEN.search(text)
    if bad:
        sys.exit(f"{src}: '{bad.group(0)}' is not allowed; write the function in plain C++")
    flags = args.flags.split()
    m = FILE_FLAGS_LINE.search(text)
    if m:
        flags += [f for f in m.group(1).split() if f in FILE_FLAGS and f not in flags]

    if args.sym:
        match = ["substr", [args.sym]]
    elif qualname and qualname.startswith("="):
        match = ["exact", [qualname[1:]]]
    elif qualname:
        match = ["prefix", mangled_prefixes(qualname)]
    else:
        match = ["substr", [""]]

    env = dict(os.environ, WINEPREFIX=str(root / "toolchain" / "wineprefix"), WINEDEBUG="-all")
    env.pop("BT_TOOLCHAIN", None)
    if not env.get("WINEARCH"):
        sysreg = root / "toolchain" / "wineprefix" / "system.reg"
        arch = re.search(r"^#arch=(\S+)", sysreg.read_text(errors="replace"), re.M) if sysreg.exists() else None
        env["WINEARCH"] = arch.group(1) if arch else "win64"

    runs = root / "build" / "c2prio"
    runs.mkdir(parents=True, exist_ok=True)
    for old in runs.iterdir():  # left behind by a run that was killed
        try:
            if time.time() - old.stat().st_mtime > 86400:
                shutil.rmtree(old, ignore_errors=True)
        except OSError:
            pass
    token = secrets.token_hex(4)
    run = runs / token
    run.mkdir()
    # A unique file name, so this run finds its own C2 among other agents' processes.
    exe = run / f"c2p{token}.exe"
    data = bytearray(c2.read_bytes())
    off = file_offset(data, ENTRY)
    assert data[off:off + 2] == ENTRY_BYTES
    data[off:off + 2] = b"\xeb\xfe"  # jmp $
    exe.write_bytes(data)
    (run / "MSPDB50.DLL").symlink_to(tc / "BIN" / "MSPDB50.DLL")
    obj = run / "out.obj"
    out_json = run / "trace.json"

    procs = []
    t0 = time.time()
    try:
        fd = [f"/Fd{winpath(run / 'out.pdb')}"] if "/Gi" in flags else []
        cl = subprocess.Popen([str(root / "tools" / "wcl"), "/c", *flags, *fd, f"/I{winpath(root / 'include')}",
                               f"/B2{winpath(exe)}", f"/Fo{winpath(obj)}", winpath(src.resolve())],
                              cwd=root, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        procs.append(cl)
        pid = None
        while pid is None:
            info = subprocess.run(["winedbg"], input="info process\nquit\n", env=env, capture_output=True,
                                  text=True, timeout=60).stdout
            for line in info.splitlines():
                mm = re.match(r"\s*=?\s*([0-9a-fA-F]{8})\s+\d+\s+(?:\\_ )?'(.*)'", line)
                if mm and mm.group(2).lower() == exe.name:
                    pid = int(mm.group(1), 16)
            if pid is None:
                if cl.poll() is not None:
                    sys.exit(f"compile failed before C2 ran:\n{cl.stdout.read()}")
                if time.time() - t0 > 600:
                    sys.exit("C2 did not start within 600 s")
                time.sleep(0.05)
        with socket.socket() as sk:
            sk.bind(("", 0))
            port = sk.getsockname()[1]
        srv = subprocess.Popen(["winedbg", "--gdb", "--no-start", "--port", str(port), str(pid)], env=env,
                               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        procs.append(srv)
        cfg = run / "config.json"
        me = str(Path(__file__).resolve())
        relay_cmd = " ".join(shlex.quote(a) for a in (sys.executable, me, "--relay", str(port)))
        cfg.write_text(json.dumps({"relay": relay_cmd, "exe": exe.name, "match": match, "out": str(out_json)}))
        g = subprocess.run(["gdb", "-nx", "-q", "-batch", "-x", me],
                           env=dict(env, C2PRIO_CONFIG=str(cfg)), capture_output=True, text=True, timeout=3600)
        (run / "gdb.log").write_text(g.stdout + g.stderr)
        trace = json.loads(out_json.read_text()) if out_json.exists() else None
        if trace is None or trace.get("error"):
            kill_matching(exe.name)
            why = trace["error"] if trace else f"{g.stdout}{g.stderr}"
            sys.exit(f"the trace failed:\n{why}")
        log = cl.communicate(timeout=600)[0].replace("\r", "")
        if cl.returncode != 0 or not obj.exists():
            sys.exit(f"compile failed:\n{log}")
        elapsed = time.time() - t0
        picked, err = select_function(parse_object(obj.read_bytes(), obj.name), args.sym, qualname)
        if picked is None:
            sys.exit(err)
        fns = [f for f in trace["functions"] if f["name"] == picked[0]]
        if args.json:
            args.json.write_text(json.dumps(fns, indent=1))
        if not fns:
            sys.exit(f"C2 never ran the global allocator for {picked[0]}")
        brace = body_line(text, args.address)
        try:
            shown = src.resolve().relative_to(root)
        except ValueError:
            shown = src
        print(f"{args.address:#x}  {picked[0]}  ({shown}, {elapsed:.1f} s)")
        for n, fn in enumerate(fns):
            if len(fns) > 1:
                print(f"\nC2 allocated this function {len(fns)} times; run {n + 1}:")
            report(fn, brace, args.trace)
    finally:
        for p in procs:
            if p.poll() is None:
                p.kill()
        kill_matching(exe.name)
        if not args.keep:
            shutil.rmtree(run, ignore_errors=True)


def file_offset(pe: bytes, va: int) -> int:
    """File offset of a virtual address in a PE image."""
    peoff = struct.unpack_from("<I", pe, 0x3C)[0]
    nsec, optsize = struct.unpack_from("<H", pe, peoff + 6)[0], struct.unpack_from("<H", pe, peoff + 20)[0]
    base = struct.unpack_from("<I", pe, peoff + 24 + 28)[0]
    sec = peoff + 24 + optsize
    for i in range(nsec):
        vsize, vaddr, rsize, raddr = struct.unpack_from("<IIII", pe, sec + 40 * i + 8)
        if vaddr <= va - base < vaddr + max(vsize, rsize):
            return va - base - vaddr + raddr
    raise ValueError(f"{va:#x} is not in the image")


def kill_matching(name: str) -> None:
    """Kill this run's Wine processes (CL and the C2 copy), found by the copy's unique name."""
    import signal
    for pid in os.listdir("/proc"):
        if not pid.isdigit():
            continue
        try:
            cmd = open(f"/proc/{pid}/cmdline", "rb").read().decode("latin1").lower()
        except OSError:
            continue
        if name in cmd:
            try:
                os.kill(int(pid), signal.SIGKILL)
            except OSError:
                pass


def body_line(text: str, address: int):
    """Line of the `{` that opens the annotated function's body. C1 numbers each
    tuple's line from the line before it."""
    from check import ANNOTATION
    lines = text.splitlines(keepends=True)
    start = None
    for i, l in enumerate(lines):
        m = ANNOTATION.match(l)
        if m and int(m.group(1), 16) == address:
            start = i
    if start is None:
        return None
    body = "".join(lines[start + 1:])
    depth, i, line = 0, 0, start + 2
    while i < len(body):
        ch = body[i]
        if ch == "\n":
            line += 1
        elif body.startswith("//", i):
            i = body.find("\n", i) - 1 if "\n" in body[i:] else len(body)
        elif body.startswith("/*", i):
            end = body.find("*/", i + 2)
            end = len(body) if end < 0 else end
            line += body.count("\n", i, end)
            i = end + 1
        elif ch in "\"'":
            j = i + 1
            while j < len(body) and body[j] != ch and body[j] != "\n":
                j += 2 if body[j] == "\\" else 1
            i = j
        elif ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif ch == "{" and depth == 0:
            return line
        elif ch == ";" and depth == 0:
            return None  # a declaration, not a definition
        i += 1
    return None


# --- the report ------------------------------------------------------------------

class Cand:
    """One candidate, from its creation until C2 frees it (C2 reuses the ids of freed ones)."""

    def __init__(self, cid, leaf, root=None):
        self.id, self.leaf = cid, leaf
        self.root = root or self
        self.pieces = []        # on a root: every piece split off it or off its pieces
        self.lines = []
        self.reg = None
        self.split = False
        self.listed = False
        self.late = False       # made during allocation by a call this tool does not know


def model(fn: dict):
    """Replay the events, binding each to the candidate its id meant at that moment."""
    cur, roots = {}, []
    phase = "build"
    for e in fn["events"]:
        k = e["e"]
        if k == "driver":
            phase = "alloc"
        elif k == "new":
            parent = cur.get(e["parent"]) if e["parent"] is not None else None
            c = Cand(e["id"], e["leaf"], parent.root if parent else None)
            if parent:
                parent.split = True
                c.root.pieces.append(c)
            else:
                c.late = phase != "build"
                roots.append(c)
            cur[e["id"]] = c
        elif k == "ref":
            if e["id"] in cur:
                cur[e["id"]].lines.append(e["line"])
        elif k in ("sorted", "resort"):
            e["cands"] = [cur.get(s["id"]) for s in e["list"]]
            if k == "sorted":
                for c in e["cands"]:
                    if c:
                        c.listed = True
        elif "id" in e:
            c = e["cand"] = cur.get(e["id"])
            if c is not None and k == "chosen":
                c.reg = e["reg"]
            elif c is not None and k == "split":
                c.split = True
    return roots


def compress(nums) -> str:
    nums = sorted(set(nums))
    parts, i = [], 0
    while i < len(nums):
        j = i
        while j + 1 < len(nums) and nums[j + 1] == nums[j] + 1:
            j += 1
        parts.append(str(nums[i]) if i == j else f"{nums[i]}-{nums[j]}")
        i = j + 1
    s = ",".join(parts)
    return s if len(s) <= 16 else f"{nums[0]}..{nums[-1]}"


def describe(c) -> str:
    """A readable name for a candidate."""
    if c is None:
        return "?"
    leaf = c.leaf
    kind, name = leaf.get("kind"), leaf.get("name")
    width = {1: " (8-bit)", 2: " (16-bit)"}.get(leaf.get("type", 0) & 0xFF, "")
    if kind == 13:
        v = leaf.get("value")
        s = "const ?" if v is None else f"const {v if -10 < v < 10 else hex(v & 0xFFFFFFFF)}"
    elif kind == 7 and name:
        from check import base_name  # host_main put tools/ on the path
        s = base_name(name)
    elif name:
        s = (name[1:] if name.startswith("_") else name) + width
    elif kind == 4:
        off = leaf.get("offset", 0)
        s = "local temp" + (f" [{'-' if off < 0 else '+'}{abs(off):#x}]" if off else "") + width
    else:
        s = KINDS.get(kind, f"kind {kind}") + width
    return s if c.root is c else f"{s}, piece of #{c.root.id}"


def allowed_str(bits) -> str:
    if bits is None:
        return "-"
    return "".join(LETTERS[r] if r in bits else "." for r in ORDER)


def regname(r) -> str:
    return REG_NAMES.get(r, f"reg{r}")


def result(c) -> str:
    """The register a candidate ended with, or what happened to it instead."""
    if c is None:
        return "?"
    none = "immediate" if c.leaf.get("kind") == 13 else "memory"
    if not c.split:
        return regname(c.reg) if c.reg else none
    rest = [p for p in c.root.pieces if not p.split]
    got = [f"{regname(p.reg)} #{p.id}" for p in rest if p.reg]
    left = len(rest) - len(got)
    if not got:
        return f"split, {none}"
    return "split: " + ", ".join(got) + (f", {left} piece{'s' * (left > 1)} {none}" if left else "")


def report(fn: dict, brace, trace: bool) -> None:
    roots = model(fn)
    events = fn["events"]

    def lines(c):
        if not c or not c.lines:
            return ""
        if brace is None:
            return "+" + compress(c.lines)
        return compress([brace - 1 + d for d in c.lines])

    sorts = [e for e in events if e["e"] == "sorted"]
    for s in sorts:
        cname = "integer" if s["class"] == 0 else "x87"
        print(f"\n{cname} register candidates in C2's order (FUN_0041bdd7): priority, then +0x40, larger first.")
        print("FUN_0041b785 colours them from the top. allowed: a=eax c=ecx d=edx s=esi i=edi b=ebx p=ebp")
        print(f"{'#':>3} {'id':>4}  {'candidate':<22} {'lines':<16} {'prio':>6} {'+0x40':>5} {'spill':>5} "
              f"{'refs':>4}  {'allowed':<7}  register")
        for i, (snap, c) in enumerate(zip(s["list"], s["cands"])):
            print(f"{i:>3} {snap['id']:>4}  {describe(c):<22.22} {lines(c):<16} {snap['prio']:>6} "
                  f"{snap['k40']:>5} {snap['spill']:>5} {snap['refs']:>4}  {allowed_str(snap['allowed']):<7}  "
                  f"{result(c)}")
    if not sorts:
        print("\nC2's global allocator had no integer candidates in this function.")
    unlisted = [c for c in roots if not c.listed and not c.late and (c.lines or c.leaf.get("name"))]
    x87 = [c for c in unlisted if c.leaf.get("type", 0) >> 12 == 4]
    dropped = [c for c in unlisted if c not in x87]
    if dropped:
        print("\nnot in the list: dropped before the sort (FUN_0041a6f8 keeps a candidate with one reference "
              "in memory\nand forwards a copy into its use):")
        for c in dropped:
            print(f"    {c.id:>4}  {describe(c):<22.22} {lines(c):<16} {result(c) if c.split else ''}".rstrip())
    if x87 and not any(s["class"] != 0 for s in sorts):
        print("\nfloating-point candidates (the x87 stack, FUN_0045f4b7, is not traced): "
              + ", ".join(f"{describe(c)} ({lines(c)})" for c in x87))
    if trace:
        print_trace(events)


def print_trace(events) -> None:
    print("\ncolouring, step by step (C2 reuses the ids of freed candidates for new pieces):")
    current = {}
    step = 0
    for e in events:
        k = e["e"]
        label = f"#{e['id']} {describe(e.get('cand'))}" if "id" in e else ""
        if k == "sorted":
            print(f"  -- {len(e['list'])} {'integer' if e['class'] == 0 else 'x87'} candidates sorted")
        elif k == "resort":
            print("  -- priorities recomputed (FUN_0040ee1d), the rest re-sorted: "
                  + ", ".join(f"#{s['id']} {s['prio']}" for s in e["list"][:24])
                  + (" ..." if len(e["list"]) > 24 else ""))
        elif k == "choose":
            current = e
        elif k == "chosen":
            step += 1
            costs = ", ".join(f"{regname(r)} {c:+d}" for r, c in enumerate(e["costs"]) if c)
            print(f"  {step:>3}. {label:<38.38} prio {e['prio']:>6}  allowed {allowed_str(current.get('allowed'))}"
                  f" -> {regname(e['reg'])}" + (f"   costs {costs}" if costs else ""))
        elif k == "lowspill":
            print(f"       {label:<38.38} prio {e['prio']:>6}  spill cost not positive, not coloured now")
        elif k == "defer":
            print(f"       {label:<38.38} prio {e['prio']:>6}  put back into the list (FUN_0045aaf9)")
        elif k == "split":
            print(f"       {label:<38.38} prio {e['prio']:>6}  no register left, split (FUN_00439385)")


def relay(port: int) -> None:
    """gdb's link to winedbg's gdb server (gdb runs it as `target remote | ...`).

    winedbg reads gdb's first packet but answers it only when more bytes
    arrive, and gdb resends only after its 2 s timeout. So after the first
    packet this sends one `+` (an ack, which the server ignores) to wake it,
    and otherwise passes the bytes through unchanged."""
    import selectors
    import socket
    import time
    for _ in range(600):
        try:
            sock = socket.create_connection(("localhost", port))
            break
        except ConnectionRefusedError:
            time.sleep(0.05)
    else:
        sys.exit("winedbg's gdb server did not open")
    sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    sel = selectors.DefaultSelector()
    sel.register(sock, selectors.EVENT_READ)
    sel.register(0, selectors.EVENT_READ)
    nudge = None   # when to wake the server, while its first answer is pending
    while True:
        timeout = None if nudge is None else max(0.0, nudge - time.time())
        ready = sel.select(timeout)
        if not ready and nudge is not None:
            sock.sendall(b"+")
            nudge = time.time() + 0.05
            continue
        for key, _ in ready:
            if key.fileobj is sock:
                data = sock.recv(65536)
                if not data:
                    return
                if b"$" in data:
                    nudge = None
                while data:
                    data = data[os.write(1, data):]
            else:
                data = os.read(0, 65536)
                if not data:
                    return
                sock.sendall(data)
                if nudge is None and data.lstrip(b"+").startswith(b"$qSupported"):
                    nudge = time.time() + 0.02


if gdb is not None:
    tracer()
elif __name__ == "__main__" and sys.argv[1:2] == ["--relay"]:
    relay(int(sys.argv[2]))
elif __name__ == "__main__":
    host_main()
