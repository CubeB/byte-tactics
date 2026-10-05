"""Play-test builds of TotalA.exe under Wine, each run fenced off from the rest.

    uv run tools/playtest.py --exe orig,carve --scenario full
    uv run tools/playtest.py --exe orig --exe carve --scenario arm,core --repeat 2 --parallel 4
    uv run tools/playtest.py --exe orig+carve --scenario mp --trap-stubs
    uv run tools/playtest.py --list

`--exe` takes `orig` (orig/TotalA.exe), `place` (build/place/TotalA.exe from
tools/place.py), `carve` (build/link/TotalA.exe from tools/link.py --carve) or
the path of an exe; `orig+carve` runs orig in a scenario's first instance and
carve in the others. A scenario is a script in tools/playtest/<name>.txt (the
step language is described in docs/linking.md). Every pair of scenario and
exe is one run, and `--parallel` runs that many at once. `--trap-stubs` plays
a copy of an ordinary link whose stubs fault when reached (see stub_traps).

Each run has its own:

- X server: an invisible Xvfb display, so nothing opens on the desktop and
  its pointer and keyboard belong to nobody else (`--window` makes it a
  nested Xephyr window instead, to watch the game);
- Wine prefix: a copy of a template made once with wineboot
  (build/playtest/prefix);
- game directory: the game's small files copied from the Steam (or GOG)
  install, its large archives linked;
- audio sink: a PulseAudio null sink, so the game is silent on the desktop
  and the run can measure what it plays.

The run's logs and screenshots go to build/playtest/<session>/<run>/:
steps.log (each step with its time), wine.log, audio.log (the sink's level
every second), files.log (the music files the game opened), the named
screenshots as PNG and the periodic ones as JPEG. Everything the run started
(the X server, Wine and its server, parec, the null sink) is stopped at the end,
also on Ctrl-C, and the prefix and game directory are deleted unless
`--keep` is given. The summary (build/playtest/<session>/summary.md) has one
line per run, PASS or FAIL with the reason, and how each named screenshot
compares with the original exe's.
"""

import argparse
import atexit
import datetime
import fcntl
import json
import os
import re
import shutil
import signal
import subprocess
import sys
import tempfile
import threading
import time
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SCENARIOS = ROOT / "tools/playtest"
OUT = ROOT / "build/playtest"
EXES = {
    "orig": ROOT / "orig/TotalA.exe",
    "place": ROOT / "build/place/TotalA.exe",
    "carve": ROOT / "build/link/TotalA.exe",
}
GAME_DIRS = [
    Path.home() / ".local/share/Steam/steamapps/common/Total Annihilation",
    Path.home() / ".steam/steam/steamapps/common/Total Annihilation",
    Path.home() / "GOG Games/Total Annihilation",
]
# Files at least this big are linked into the run's game directory, not copied
# (the .hpi/.ccx archives the game only reads).
LINK_SIZE = 4_000_000
REGKEY = r"HKEY_CURRENT_USER\Software\Cavedog Entertainment\Total Annihilation"
POLL = 1.0          # seconds between checks of the watched screens while waiting
SOUND_LEVEL = 200   # RMS (16-bit samples) above which a second counts as sound
TEMPLATE_REVISION = 2   # bump to remake the template prefixes

_live: list["Instance"] = []
_live_lock = threading.Lock()
_interrupted = threading.Event()


def log_time(t0: float) -> str:
    s = time.time() - t0
    return f"{int(s // 60):3d}:{s % 60:05.2f}"


# --- screens --------------------------------------------------------------


@dataclass
class Screen:
    """A known screen, recognised by a signature of one area of it."""

    name: str
    x: int
    y: int
    w: int
    h: int
    kind: str                      # "grid": mean grey per cell; "mask": text over any background
    cell: int = 4
    tolerance: float = 8.0
    grid: list[int] = field(default_factory=list)
    bright: list[int] = field(default_factory=list)   # mask: indexes of the text's bright pixels
    dark: list[int] = field(default_factory=list)     # mask: indexes of its dark outline

    def distance(self, img: "Gray") -> float:
        """Grid: mean difference per cell (0 to 255). Mask: share of mask pixels that disagree."""
        crop = img.crop(self.x, self.y, self.w, self.h)
        if crop is None:
            return 255.0
        if self.kind == "grid":
            cells = cell_means(crop, self.w, self.h, self.cell)
            return sum(abs(a - b) for a, b in zip(cells, self.grid)) / len(self.grid)
        bad_b = sum(1 for i in self.bright if crop[i] <= 120) / max(1, len(self.bright))
        bad_d = sum(1 for i in self.dark if crop[i] >= 70) / max(1, len(self.dark))
        return max(bad_b, bad_d)

    def matches(self, img: "Gray") -> bool:
        d = self.distance(img)
        return d <= (self.tolerance if self.kind == "grid" else 0.2)


def cell_means(pixels: bytes, w: int, h: int, cell: int) -> list[int]:
    out = []
    for cy in range(0, h - cell + 1, cell):
        for cx in range(0, w - cell + 1, cell):
            s = 0
            for y in range(cy, cy + cell):
                row = y * w
                s += sum(pixels[row + cx:row + cx + cell])
            out.append(s // (cell * cell))
    return out


def load_screens() -> dict[str, Screen]:
    screens = {}
    for line in (SCENARIOS / "screens.txt").read_text().splitlines():
        words = line.split("#", 1)[0].split()
        if not words:
            continue
        name, geo, kind = words[:3]
        m = re.fullmatch(r"(\d+)x(\d+)\+(\d+)\+(\d+)", geo)
        if not m:
            raise SystemExit(f"screens.txt: bad geometry {geo}")
        w, h, x, y = (int(v) for v in m.groups())
        s = Screen(name, x, y, w, h, kind)
        if kind == "grid":
            s.cell, s.tolerance = int(words[3]), float(words[4])
            s.grid = list(bytes.fromhex(words[5]))
        elif kind == "mask":
            s.bright = bits_to_indexes(words[3])
            s.dark = bits_to_indexes(words[4])
        else:
            raise SystemExit(f"screens.txt: unknown kind {kind}")
        screens[name] = s
    return screens


def bits_to_indexes(hexbits: str) -> list[int]:
    n = int(hexbits, 16) if hexbits != "-" else 0
    return [i for i in range(n.bit_length()) if n >> i & 1]


def indexes_to_bits(idx: list[int]) -> str:
    n = 0
    for i in idx:
        n |= 1 << i
    return f"{n:x}" if n else "-"


class Gray:
    """A greyscale screenshot."""

    def __init__(self, w: int, h: int, pixels: bytes):
        self.w, self.h, self.pixels = w, h, pixels

    @staticmethod
    def parse_pgm(data: bytes) -> "Gray | None":
        m = re.match(rb"P5\s+(?:#[^\n]*\n\s*)*(\d+)\s+(\d+)\s+(\d+)\s", data)
        if not m:
            return None
        w, h = int(m.group(1)), int(m.group(2))
        pixels = data[m.end():m.end() + w * h]
        return Gray(w, h, pixels) if len(pixels) == w * h else None

    @staticmethod
    def load(path: Path) -> "Gray":
        r = subprocess.run(["convert", str(path), "-colorspace", "Gray", "-depth", "8", "pgm:-"],
                           capture_output=True, check=True)
        img = Gray.parse_pgm(r.stdout)
        if img is None:
            raise SystemExit(f"cannot read {path}")
        return img

    def crop(self, x: int, y: int, w: int, h: int) -> bytes | None:
        if x + w > self.w or y + h > self.h:
            return None
        return b"".join(self.pixels[(y + r) * self.w + x:(y + r) * self.w + x + w] for r in range(h))


def make_signature(name: str, geo: str, images: list[Path], kind: str, cell: int, tolerance: float) -> str:
    """A screens.txt line for the area `geo` of screenshots of one screen (or of crops that size).

    A grid averages the screenshots. A mask keeps the pixels bright in every one
    of them that touch a pixel dark in every one, and the other way round: the
    text and its outline, not the terrain behind it, which differs between
    screenshots taken over different terrain.
    """
    m = re.fullmatch(r"(\d+)x(\d+)\+(\d+)\+(\d+)", geo)
    if not m:
        raise SystemExit(f"bad geometry {geo}")
    w, h, x, y = (int(v) for v in m.groups())
    crops = []
    for image in images:
        img = Gray.load(image)
        crop = img.pixels if (img.w, img.h) == (w, h) else img.crop(x, y, w, h)
        if crop is None:
            raise SystemExit(f"{geo} is outside {image}")
        crops.append(crop)
    if kind == "mask":
        bright = {i for i in range(w * h) if all(c[i] > 150 for c in crops)}
        dark = {i for i in range(w * h) if all(c[i] < 50 for c in crops)}

        def touches(i: int, other: set[int]) -> bool:
            px, py = i % w, i // w
            return any((px + dx, py + dy) != (px, py) and 0 <= px + dx < w and 0 <= py + dy < h
                       and (py + dy) * w + px + dx in other for dx in (-1, 0, 1) for dy in (-1, 0, 1))

        text = sorted(i for i in bright if touches(i, dark))
        outline = sorted(i for i in dark if touches(i, bright))
        return f"{name} {geo} mask {indexes_to_bits(text)} {indexes_to_bits(outline)}"
    grids = [cell_means(c, w, h, cell) for c in crops]
    mean = [sum(g[i] for g in grids) // len(grids) for i in range(len(grids[0]))]
    return f"{name} {geo} grid {cell} {tolerance:g} {bytes(mean).hex()}"


# --- one game instance ----------------------------------------------------


def find_game_dir(arg: str | None) -> Path:
    if arg:
        return Path(arg).expanduser()
    for d in GAME_DIRS:
        if (d / "totala1.hpi").exists():
            return d
    raise SystemExit("no Total Annihilation install found: pass --game-dir")


def wine_env(prefix: Path, display: str = "") -> dict[str, str]:
    env = dict(os.environ)
    env.update(WINEPREFIX=str(prefix), WINEARCH="win32", WINEDLLOVERRIDES="mscoree,mshtml=", WINEDEBUG="-all")
    env.pop("WAYLAND_DISPLAY", None)
    if display:
        env["DISPLAY"] = display
    else:
        env.pop("DISPLAY", None)
    return env


def template_prefix() -> Path:
    """build/playtest/prefix: a fresh 32-bit prefix, made again when Wine changes.

    A crash prints winedbg's backtrace to wine.log and ends the process, rather
    than waiting on Wine's crash dialog.
    """
    path = OUT / "prefix"
    version = subprocess.run(["wine", "--version"], capture_output=True, text=True).stdout.strip()
    version += f" (template {TEMPLATE_REVISION})"
    stamp = path / ".playtest-wine-version"
    if stamp.exists() and stamp.read_text() == version:
        return path
    print(f"making the template Wine prefix {path.relative_to(ROOT)} ({version})", flush=True)
    shutil.rmtree(path, ignore_errors=True)
    path.parent.mkdir(parents=True, exist_ok=True)
    env = wine_env(path)
    subprocess.run(["wineboot", "-i"], env=env, capture_output=True)
    subprocess.run(["wine", "reg", "add", r"HKCU\Software\Wine\WineDbg", "/v", "ShowCrashDialog", "/t",
                    "REG_DWORD", "/d", "0", "/f"], env=env, capture_output=True)
    subprocess.run(["wineserver", "-w"], env=env)
    stamp.write_text(version)
    return path


def directplay_prefix() -> Path:
    """build/playtest/prefix-directplay: the template with Microsoft's DirectPlay.

    Wine's own dpwsockx (the TCP/IP service provider) is a stub up to Wine 9 at
    least, so multiplayer needs the native DLLs: winetricks' directplay verb takes
    them from the DirectX June 2010 redistributable (from its cache, or downloaded).
    """
    base = template_prefix()
    path = OUT / "prefix-directplay"
    stamp = path / ".playtest-directplay"
    version = (base / ".playtest-wine-version").read_text()
    if stamp.exists() and stamp.read_text() == version:
        return path
    if not shutil.which("winetricks"):
        raise SystemExit("multiplayer needs winetricks (sudo apt install winetricks) for native DirectPlay")
    print(f"making {path.relative_to(ROOT)}: winetricks directplay", flush=True)
    shutil.rmtree(path, ignore_errors=True)
    subprocess.run(["cp", "-a", str(base), str(path)], check=True)
    env = wine_env(path)
    r = subprocess.run(["winetricks", "-q", "directplay"], env=env, capture_output=True, text=True)
    subprocess.run(["wineserver", "-w"], env=env)
    log = path / "winetricks.log"
    if r.returncode != 0 or not log.exists() or "directplay" not in log.read_text().split():
        raise SystemExit(f"winetricks directplay failed:\n{r.stdout[-2000:]}{r.stderr[-2000:]}")
    stamp.write_text(version)
    return path


def copy_game(src: Path, dst: Path) -> None:
    for d, dirs, files in os.walk(src):
        rel = Path(d).relative_to(src)
        (dst / rel).mkdir(parents=True, exist_ok=True)
        for f in files:
            s = Path(d) / f
            if s.stat().st_size >= LINK_SIZE:
                (dst / rel / f).symlink_to(s)
            else:
                shutil.copy2(s, dst / rel / f)


class Instance:
    """One copy of the game: an X server, a prefix, a game directory and a null sink."""

    def __init__(self, run: "Run", index: int):
        self.run = run
        self.index = index
        self.suffix = "" if index == 1 else f"_{index}"
        self.base = run.work / f"i{index}"
        self.prefix = self.base / "prefix"
        self.game = self.base / "game"
        self.display = ""
        self.xserver: subprocess.Popen | None = None
        self.wine: subprocess.Popen | None = None
        self.parec: subprocess.Popen | None = None
        self.sink = ""
        self.module = ""
        self.rc: int | None = None
        self.stop_threads = threading.Event()
        self.threads: list[threading.Thread] = []
        self.shot_every = 0.0
        self.shot_event = threading.Event()
        self.sound_seconds = 0
        self.music_seconds = 0     # seconds with sound while a music track was open
        self.loudest = 0
        self.music: dict[str, float] = {}     # track -> first time it was open
        self.music_open: set[str] = set()
        self.last_level = 0
        self.stop_lock = threading.Lock()
        self.stopped = False
        self.quit = False          # exited when the scenario told it to

    def env(self) -> dict[str, str]:
        env = wine_env(self.prefix, self.display)
        if self.sink:
            env["PULSE_SINK"] = self.sink
        return env

    # setting up

    def prepare(self, template: Path, game_src: Path, reg: list[str], files: dict[str, str]) -> None:
        self.base.mkdir(parents=True, exist_ok=True)
        subprocess.run(["cp", "-a", str(template), str(self.prefix)], check=True)
        copy_game(game_src, self.game)
        shutil.copy2(self.run.exes[min(self.index, len(self.run.exes)) - 1], self.game / "TotalA.exe")
        for name, text in files.items():
            (self.game / name).write_text(text.replace("\\n", "\r\n"))
        if reg:
            regfile = self.base / "settings.reg"
            regfile.write_text("REGEDIT4\r\n\r\n" + "\r\n".join(reg) + "\r\n")
            env = wine_env(self.prefix)
            subprocess.run(["wine", "regedit", "/S", "Z:" + str(regfile).replace("/", "\\")], env=env,
                           capture_output=True)
            subprocess.run(["wineserver", "-w"], env=env)

    def start_x(self) -> None:
        r, w = os.pipe()
        if self.run.window:
            if not os.environ.get("DISPLAY") and not os.environ.get("WAYLAND_DISPLAY"):
                raise RuntimeError("Xephyr needs a desktop to open its window on (DISPLAY is not set)")
            cmd = ["Xephyr", "-displayfd", str(w), "-screen", "800x600x24", "-ac", "-br", "-noreset",
                   "-title", f"TA play test: {self.run.name}{self.suffix}"]
        else:
            cmd = ["Xvfb", "-displayfd", str(w), "-screen", "0", "640x480x24", "-ac", "-br", "-noreset",
                   "-nolisten", "tcp"]
        self.xserver = subprocess.Popen(
            cmd, pass_fds=(w,), stdout=(self.run.out / f"xserver{self.suffix}.log").open("w"),
            stderr=subprocess.STDOUT, start_new_session=True)
        os.close(w)
        with os.fdopen(r) as f:
            num = f.readline().strip()
        if not num.isdigit():
            raise RuntimeError(f"{cmd[0]} did not start (see xserver.log)")
        self.display = f":{num}"
        deadline = time.time() + 10
        while subprocess.run(["xdpyinfo"], env={**os.environ, "DISPLAY": self.display},
                             capture_output=True).returncode != 0:
            if time.time() > deadline:
                raise RuntimeError(f"X server {self.display} does not answer")
            time.sleep(0.2)

    def start_sink(self) -> None:
        name = f"bt_playtest_{os.getpid()}_{self.run.number}_{self.index}"
        r = subprocess.run(["pactl", "load-module", "module-null-sink", f"sink_name={name}",
                            f"sink_properties=device.description={name}"], capture_output=True, text=True)
        if r.returncode != 0:
            self.run.note(f"no audio sink ({r.stderr.strip()}): the game's sound goes to the desktop")
            return
        self.module, self.sink = r.stdout.strip(), name
        self.parec = subprocess.Popen(
            ["parec", f"--device={name}.monitor", "--format=s16le", "--rate=8000", "--channels=1", "--raw"],
            stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, start_new_session=True)
        self.thread(self.audio_loop)

    def start(self, debug: str) -> None:
        with _live_lock:
            _live.append(self)
        self.start_x()
        self.start_sink()
        (self.run.out / f"exe{self.suffix}.sha256").write_text(
            subprocess.run(["sha256sum", str(self.game / "TotalA.exe")], capture_output=True,
                           text=True).stdout)
        env = self.env()
        env["WINEDEBUG"] = debug
        exe = "Z:" + str(self.game / "TotalA.exe").replace("/", "\\")
        self.wine = subprocess.Popen(["wine", exe], cwd=self.game, env=env, start_new_session=True,
                                     stdout=(self.run.out / f"wine{self.suffix}.log").open("w"),
                                     stderr=subprocess.STDOUT)
        self.run.note(f"  game{self.suffix} started on display {self.display}, pid {self.wine.pid}"
                      + (f", sound to {self.sink}" if self.sink else ""))
        self.thread(self.files_loop)
        self.thread(self.shots_loop)

    def thread(self, fn) -> None:
        t = threading.Thread(target=fn, daemon=True)
        t.start()
        self.threads.append(t)

    # watching

    def alive(self) -> bool:
        """Is the game running? Notes its exit code when it has ended by itself."""
        if self.wine is None:
            return False
        if self.wine.poll() is None:
            return True
        if not self.stopped:
            self.rc = self.wine.returncode
        return False

    def audio_loop(self) -> None:
        log = (self.run.out / f"audio{self.suffix}.log").open("w")
        log.write("# second, RMS of the game's output (16-bit), music files open\n")
        n = 0
        while not self.stop_threads.is_set() and self.parec and self.parec.stdout:
            chunk = self.parec.stdout.read(16000)      # one second at 8 kHz
            if not chunk:
                break
            samples = memoryview(chunk).cast("h") if len(chunk) % 2 == 0 else memoryview(chunk[:-1]).cast("h")
            rms = int((sum(s * s for s in samples) / max(1, len(samples))) ** 0.5)
            n += 1
            self.last_level = rms
            self.loudest = max(self.loudest, rms)
            if rms > SOUND_LEVEL:
                self.sound_seconds += 1
                if self.music_open:
                    self.music_seconds += 1
            log.write(f"{n} {rms} {' '.join(sorted(self.music_open))}\n")
            log.flush()

    def files_loop(self) -> None:
        """Note the music files the game's processes hold open (GOG's win32.dll plays them)."""
        log = (self.run.out / f"files{self.suffix}.log").open("w")
        prefix = str(self.prefix)
        server_pids: list[int] = []
        last_scan = 0.0
        while not self.stop_threads.wait(1.0):
            if time.time() - last_scan > 10:
                server_pids, last_scan = prefix_pids(prefix), time.time()
            pids = list(server_pids)
            if self.wine and self.wine.poll() is None:
                pids.append(self.wine.pid)
            now = set()
            for pid in pids:
                try:
                    fds = os.listdir(f"/proc/{pid}/fd")
                except OSError:
                    continue
                for fd in fds:
                    try:
                        target = os.readlink(f"/proc/{pid}/fd/{fd}")
                    except OSError:
                        continue
                    m = re.search(r"/music/([^/]+\.mp3)$", target, re.I)
                    if m:
                        now.add(m.group(1))
            for track in sorted(now - self.music_open):
                t = time.time() - self.run.t0
                self.music.setdefault(track, t)
                log.write(f"{t:8.1f}s opened music/{track}\n")
            for track in sorted(self.music_open - now):
                log.write(f"{time.time() - self.run.t0:8.1f}s closed music/{track}\n")
            log.flush()
            self.music_open = now

    def shots_loop(self) -> None:
        while not self.stop_threads.is_set():
            if self.shot_every <= 0:
                self.shot_event.wait(1.0)
                self.shot_event.clear()
                continue
            if self.stop_threads.wait(self.shot_every):
                break
            if self.shot_every > 0 and self.alive():
                self.shot(f"auto_{int(time.time() - self.run.t0):05d}{self.suffix}", jpeg=True)

    # input and output

    def xdo(self, *args: str) -> None:
        subprocess.run(["xdotool", *args], env={**os.environ, "DISPLAY": self.display}, capture_output=True)

    def grab(self) -> Gray | None:
        try:
            r = subprocess.run(["import", "-window", "root", "-colorspace", "Gray", "-depth", "8", "pgm:-"],
                               env={**os.environ, "DISPLAY": self.display}, capture_output=True, timeout=15)
        except subprocess.TimeoutExpired:
            return None
        return Gray.parse_pgm(r.stdout)

    def shot(self, name: str, jpeg: bool = False) -> None:
        path = self.run.out / (name + (".jpg" if jpeg else ".png"))
        args = ["import", "-window", "root"]
        if jpeg:
            args += ["-quality", "80"]
        subprocess.run([*args, str(path)], env={**os.environ, "DISPLAY": self.display},
                       capture_output=True, timeout=30)

    # stopping

    def stop(self) -> None:
        """Stop everything this instance started; safe to call twice and from two threads."""
        with self.stop_lock:
            if self.stopped:
                return
            self.stopped = True
            self._stop()

    def _stop(self) -> None:
        self.stop_threads.set()
        self.shot_event.set()
        if self.prefix.exists():
            try:
                subprocess.run(["wineserver", "-k"], env=wine_env(self.prefix), capture_output=True, timeout=30)
            except subprocess.TimeoutExpired:
                pass
        # The game first, so that it is gone before its X server.
        for p in (self.wine, self.parec, self.xserver):
            if p and p.poll() is None:
                try:
                    p.wait(3 if p is self.wine else 0.1)
                    continue
                except subprocess.TimeoutExpired:
                    pass
                try:
                    os.killpg(p.pid, signal.SIGTERM)
                except OSError:
                    pass
                try:
                    p.wait(5)
                except subprocess.TimeoutExpired:
                    try:
                        os.killpg(p.pid, signal.SIGKILL)
                    except OSError:
                        pass
        # Anything else still running in the prefix (winedevice, services...).
        for pid in prefix_pids(str(self.prefix)):
            try:
                os.kill(pid, signal.SIGKILL)
            except OSError:
                pass
        if self.module:
            subprocess.run(["pactl", "unload-module", self.module], capture_output=True)
            self.module = ""
        with _live_lock:
            if self in _live:
                _live.remove(self)

    def clean(self) -> None:
        shutil.rmtree(self.base, ignore_errors=True)


def prefix_pids(prefix: str) -> list[int]:
    """The processes whose WINEPREFIX is this prefix."""
    want = f"WINEPREFIX={prefix}".encode()
    out = []
    for d in os.listdir("/proc"):
        if not d.isdigit():
            continue
        try:
            env = Path(f"/proc/{d}/environ").read_bytes()
        except OSError:
            continue
        if want in env.split(b"\0"):
            out.append(int(d))
    return out


def stop_all(*_args) -> None:
    with _live_lock:
        live = list(_live)
    for inst in live:
        try:
            inst.stop()
        except Exception:
            pass


# --- scenarios ------------------------------------------------------------


@dataclass
class Scenario:
    name: str
    lines: list[tuple[int, list[str]]]
    labels: dict[str, int]
    instances: int = 1
    directplay: bool = False
    reg: list[str] = field(default_factory=list)
    files: dict[str, str] = field(default_factory=dict)
    timeout: float = 3600.0


def load_scenario(name: str) -> Scenario:
    path = SCENARIOS / f"{name}.txt"
    if not path.exists():
        raise SystemExit(f"no scenario {path.relative_to(ROOT)} (see --list)")
    sc = Scenario(name, [], {})
    regs: dict[str, list[str]] = {}
    for n, raw in enumerate(path.read_text().splitlines(), 1):
        words = raw.split("#", 1)[0].split()
        if not words:
            continue
        cmd = words[0]
        if cmd == "instances":
            sc.instances = int(words[1])
        elif cmd == "directplay":
            sc.directplay = True
        elif cmd == "timeout":
            sc.timeout = float(words[1]) * 60
        elif cmd == "reg":
            # reg [<subkey>/]<name> dword|sz <value>
            key, _, val_name = words[1].rpartition("/")
            full = REGKEY + ("\\" + key.replace("/", "\\") if key else "")
            if words[2] == "dword":
                value = f"dword:{int(words[3], 0):08x}"
            else:
                value = '"' + " ".join(words[3:]).replace("\\", "\\\\").replace('"', '\\"') + '"'
            regs.setdefault(full, []).append(f'"{val_name}"={value}')
        elif cmd == "file":
            sc.files[words[1]] = " ".join(words[2:])
        elif cmd == "label":
            sc.labels[words[1]] = len(sc.lines)
        else:
            sc.lines.append((n, words))
    for key, values in regs.items():
        sc.reg += [f"[{key}]", *values, ""]
    return sc


# Each step, with how many arguments it needs and where a screen or label goes.
STEPS = {
    "start": 0, "sleep": 1, "click": 2, "sclick": 2, "dclick": 2, "hold": 2, "drag": 4, "move": 2,
    "key": 1, "keyhold": 2, "type": 1, "chat": 1, "shot": 1, "every": 1, "waitfor": 2, "trywait": 2,
    "until": 3, "ifmatch": 2, "ifnot": 2, "expect": 1, "repeat": 2, "alive": 0, "on": 2, "goto": 1,
    "waitexit": 1, "music": 1, "note": 1, "fail": 1, "stop": 0,
}
SCREEN_ARG = {"waitfor", "trywait", "until", "ifmatch", "ifnot", "expect", "on"}


def check_scenario(sc: Scenario, screens: dict[str, Screen]) -> None:
    """Refuse a scenario with a step, screen, label or instance that does not exist,
    before any run starts."""

    def check(n: int, words: list[str]) -> None:
        if words and words[0].startswith("@"):
            if not words[0][1:].isdigit() or not 1 <= int(words[0][1:]) <= sc.instances:
                raise SystemExit(f"{sc.name}.txt:{n}: no instance {words[0]}")
            words = words[1:]
        if not words or words[0] not in STEPS:
            raise SystemExit(f"{sc.name}.txt:{n}: unknown step {' '.join(words)!r}")
        cmd, args = words[0], words[1:]
        if len(args) < STEPS[cmd]:
            raise SystemExit(f"{sc.name}.txt:{n}: {cmd} needs {STEPS[cmd]} argument(s)")
        if cmd in SCREEN_ARG and args[0] not in screens:
            raise SystemExit(f"{sc.name}.txt:{n}: unknown screen {args[0]} (tools/playtest/screens.txt)")
        if cmd == "goto" or (cmd == "on" and args[1] != "off"):
            label = args[0] if cmd == "goto" else args[2] if len(args) > 2 else ""
            if label not in sc.labels:
                raise SystemExit(f"{sc.name}.txt:{n}: no label {label!r}")
        nested = {"until": 2, "repeat": 1, "ifmatch": 1, "ifnot": 1}.get(cmd)
        if nested is not None:
            for part in " ".join(args[nested:]).split(";"):
                if part.strip():
                    check(n, part.split())

    if not any(words == ["start"] or words[:1] == ["start"] for _, words in sc.lines):
        raise SystemExit(f"{sc.name}.txt: no start step")
    for n, words in sc.lines:
        check(n, words)


def list_scenarios() -> None:
    for path in sorted(SCENARIOS.glob("*.txt")):
        if path.name == "screens.txt":
            continue
        first = next((l[1:].strip() for l in path.read_text().splitlines() if l.startswith("#")), "")
        print(f"{path.stem:12} {first}")


class StepFailed(Exception):
    pass


class Interrupted(Exception):
    pass


class GotoLabel(Exception):
    def __init__(self, label: str):
        self.label = label


class Run:
    def __init__(self, number: int, name: str, scenario: Scenario, exes: list[Path], exe_tag: str, session: Path,
                 screens: dict[str, Screen], debug: str, window: bool = False):
        self.number = number
        self.name = name
        self.scenario = scenario
        self.exes = exes             # one per instance; the last serves any more
        self.exe_tag = exe_tag
        self.out = session / name
        self.work = OUT / "work" / f"{session.name}-{name}"
        self.screens = screens
        self.debug = debug
        self.window = window         # a visible Xephyr window instead of an invisible Xvfb display
        self.instances = [Instance(self, i + 1) for i in range(scenario.instances)]
        self.t0 = time.time()
        self.log = None
        self.watch: dict[str, str] = {}
        self.result = "not run"
        self.reason = ""
        self.notes: list[str] = []
        self.events: list[str] = []
        self.started = False
        self.duration = 0.0

    def note(self, text: str) -> None:
        line = f"{log_time(self.t0)} {text}"
        if self.log:
            self.log.write(line + "\n")
            self.log.flush()

    def inst(self, words: list[str], default: Instance | None) -> tuple[Instance, list[str]]:
        if words and words[0].startswith("@"):
            return self.instances[int(words[0][1:]) - 1], words[1:]
        return default or self.instances[0], words

    def screen(self, name: str) -> Screen:
        if name not in self.screens:
            raise SystemExit(f"{self.scenario.name}: unknown screen {name} (tools/playtest/screens.txt)")
        return self.screens[name]

    def on_screen(self, inst: Instance, name: str, img: Gray | None = None) -> bool:
        img = img or inst.grab()
        return bool(img) and self.screen(name).matches(img)

    def check_watch(self) -> None:
        """Jump to a watched screen's label when it shows (a victory screen, say)."""
        if not self.watch:
            return
        for inst in self.instances:
            if not inst.alive():
                continue
            img = inst.grab()
            if not img:
                continue
            for name, label in list(self.watch.items()):
                if self.screen(name).matches(img):
                    del self.watch[name]
                    self.note(f"  {name} on screen{inst.suffix}: goto {label}")
                    self.events.append(f"{name} at {log_time(self.t0).strip()}")
                    raise GotoLabel(label)

    def wait(self, seconds: float) -> None:
        end = time.time() + seconds
        while True:
            left = end - time.time()
            if left <= 0:
                return
            time.sleep(min(POLL, left))
            self.check_watch()
            self.check_alive()

    def check_alive(self) -> None:
        if _interrupted.is_set():
            raise Interrupted()
        for inst in self.instances:
            if inst.wine is not None and not inst.stopped and not inst.quit and not inst.alive():
                raise StepFailed(f"the game{inst.suffix} exited unexpectedly (exit code {inst.rc})")

    def waitfor(self, inst: Instance, name: str, timeout: float) -> bool:
        deadline = time.time() + timeout
        while time.time() < deadline:
            img = inst.grab()
            if img and self.screen(name).matches(img):
                self.note(f"  {name} on screen{inst.suffix}")
                return True
            self.check_watch()
            self.check_alive()
            time.sleep(0.5)
        self.note(f"  {name} not seen in {timeout:g}s")
        inst.shot(f"timeout_{name}{inst.suffix}")
        return False

    def step(self, words: list[str], default: Instance | None = None) -> None:
        """Run one step; `@<n>` before it picks the instance (default: the enclosing step's, or 1)."""
        inst, words = self.inst(words, default)
        cmd, args = words[0], words[1:]
        if cmd == "start":
            debug = args[0] if args else self.debug
            for i in self.instances:
                i.start(debug)
            self.started = True
        elif cmd == "sleep":
            self.wait(float(args[0]))
        elif cmd == "click":
            button = args[2] if len(args) > 2 else "1"
            inst.xdo("mousemove", args[0], args[1], "sleep", "0.15", "mousedown", button, "sleep", "0.12",
                     "mouseup", button)
        elif cmd == "sclick":
            # shift-click: queue an order, or five of a unit in a factory's menu
            inst.xdo("mousemove", args[0], args[1], "sleep", "0.15", "keydown", "shift", "sleep", "0.05",
                     "mousedown", "1", "sleep", "0.12", "mouseup", "1", "sleep", "0.05", "keyup", "shift")
        elif cmd == "dclick":
            inst.xdo("mousemove", args[0], args[1], "sleep", "0.15", "click", "--repeat", "2", "--delay", "120",
                     "1")
        elif cmd == "hold":
            inst.xdo("mousemove", args[0], args[1], "sleep", "0.15", "mousedown", "1", "sleep",
                     args[2] if len(args) > 2 else "0.5", "mouseup", "1")
        elif cmd == "drag":
            x1, y1, x2, y2 = (int(a) for a in args[:4])
            inst.xdo("mousemove", str(x1), str(y1), "sleep", "0.15", "mousedown", "1", "sleep", "0.2",
                     "mousemove", str((x1 + x2) // 2), str((y1 + y2) // 2), "sleep", "0.2",
                     "mousemove", str(x2), str(y2), "sleep", "0.3", "mouseup", "1")
        elif cmd == "move":
            inst.xdo("mousemove", args[0], args[1])
        elif cmd == "key":
            inst.xdo("key", "--delay", "100", *args)
        elif cmd == "keyhold":
            inst.xdo("keydown", args[0], "sleep", args[1], "keyup", args[0])
        elif cmd == "type":
            inst.xdo("type", "--delay", "100", " ".join(args))
        elif cmd == "chat":
            inst.xdo("key", "Return")
            time.sleep(0.4)
            inst.xdo("type", "--delay", "100", " ".join(args))
            time.sleep(0.2)
            inst.xdo("key", "Return")
            time.sleep(0.4)
        elif cmd == "shot":
            inst.shot(args[0] + inst.suffix)
        elif cmd == "every":
            inst.shot_every = float(args[0])
            inst.shot_event.set()
        elif cmd == "waitfor":
            if not self.waitfor(inst, args[0], float(args[1])):
                raise StepFailed(f"{args[0]} not seen in {args[1]}s")
        elif cmd == "trywait":
            self.waitfor(inst, args[0], float(args[1]))
        elif cmd == "until":
            # until <screen> <timeout> <step> [; <step>...]: repeat the steps until the screen shows
            deadline = time.time() + float(args[1])
            while not self.on_screen(inst, args[0]):
                if time.time() > deadline:
                    inst.shot(f"timeout_{args[0]}{inst.suffix}")
                    raise StepFailed(f"{args[0]} not seen in {args[1]}s")
                self.check_alive()
                for st in " ".join(args[2:]).split(";"):
                    if st.strip():
                        self.step(st.split(), inst)
            self.note(f"  {args[0]} on screen{inst.suffix}")
        elif cmd == "ifmatch":
            if self.on_screen(inst, args[0]):
                self.note(f"  {args[0]} on screen: {' '.join(args[1:])}")
                self.step(args[1:], inst)
            else:
                self.note(f"  {args[0]} not on screen")
        elif cmd == "ifnot":
            if not self.on_screen(inst, args[0]):
                self.note(f"  {args[0]} not on screen: {' '.join(args[1:])}")
                self.step(args[1:], inst)
            else:
                self.note(f"  {args[0]} on screen")
        elif cmd == "expect":
            if not self.on_screen(inst, args[0]):
                inst.shot(f"expect_{args[0]}{inst.suffix}")
                raise StepFailed(f"{args[0]} not on screen")
        elif cmd == "repeat":
            for _ in range(int(args[0])):
                for st in " ".join(args[1:]).split(";"):
                    if st.strip():
                        self.step(st.split(), inst)
        elif cmd == "alive":
            self.check_alive()
        elif cmd == "on":
            if args[1] == "off":
                self.watch.pop(args[0], None)
            else:
                self.screen(args[0])
                self.watch[args[0]] = args[2]
        elif cmd == "goto":
            raise GotoLabel(args[0])
        elif cmd == "waitexit":
            deadline = time.time() + float(args[0])
            while time.time() < deadline and any(i.alive() for i in self.instances):
                time.sleep(0.5)
            for i in self.instances:
                if i.alive():
                    raise StepFailed(f"the game{i.suffix} still runs {args[0]}s after quitting")
                self.note(f"  exited{i.suffix}, exit code {i.rc}")
                i.quit = True
                if i.rc != 0:
                    raise StepFailed(f"the game{i.suffix} exited with code {i.rc}")
        elif cmd == "music":
            self.music(inst, float(args[0]))
        elif cmd == "note":
            self.notes.append(" ".join(args))
        elif cmd == "fail":
            raise StepFailed(" ".join(args))
        elif cmd == "stop":
            for i in self.instances:
                i.alive()
                i.stop()
        else:
            raise SystemExit(f"{self.scenario.name}: unknown step {cmd}")

    def music(self, inst: Instance, timeout: float) -> None:
        """Wait until the game holds a music track open and its sink carries sound."""
        deadline = time.time() + timeout
        while time.time() < deadline:
            if inst.music_open and inst.last_level > SOUND_LEVEL:
                track = ", ".join(sorted(inst.music_open))
                self.note(f"  music: {track} open, level {inst.last_level}")
                self.events.append(f"heard {track} at {log_time(self.t0).strip()}")
                return
            self.check_alive()
            time.sleep(0.5)
        what = "no music file open" if not inst.music_open else f"{', '.join(inst.music_open)} open but silent"
        if not inst.sink:
            what += " (no audio sink to measure)"
        raise StepFailed(f"music: {what} after {timeout:g}s")

    def execute(self, template: Path, game_src: Path) -> None:
        self.out.mkdir(parents=True, exist_ok=True)
        self.log = (self.out / "steps.log").open("w")
        self.note(f"run {self.name}: {', '.join(map(str, self.exes))} ({self.exe_tag}), "
                  f"scenario {self.scenario.name}")
        lock = None
        if self.scenario.directplay:
            # A host on 127.0.0.1 holds DirectPlay's port, and a guest joins the
            # first game it finds there: one network game at a time on this
            # machine, also across sessions.
            lock = (Path(tempfile.gettempdir()) / f"byte-tactics-playtest-{os.getuid()}.lock").open("w")
            self.note("  waiting for the network (one DirectPlay game at a time)")
            while True:
                try:
                    fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
                    break
                except BlockingIOError:
                    if _interrupted.wait(2):
                        break
        try:
            for inst in self.instances:
                inst.prepare(template, game_src, self.scenario.reg, self.scenario.files)
            self.t0 = time.time()
            pc = 0
            lines = self.scenario.lines
            while pc < len(lines):
                n, words = lines[pc]
                pc += 1
                if time.time() - self.t0 > self.scenario.timeout:
                    raise StepFailed(f"the scenario's {self.scenario.timeout / 60:g} minutes are up")
                self.note(" ".join(words))
                try:
                    if self.started:
                        self.check_watch()
                    self.step(words)
                except GotoLabel as g:
                    if g.label not in self.scenario.labels:
                        raise SystemExit(f"{self.scenario.name}: no label {g.label}")
                    pc = self.scenario.labels[g.label]
                except StepFailed as e:
                    raise StepFailed(f"line {n} ({' '.join(words)}): {e}") from None
            self.result = "PASS"
        except Interrupted:
            self.result, self.reason = "INTERRUPTED", "stopped by a signal"
        except StepFailed as e:
            self.result, self.reason = "FAIL", str(e)
            for inst in self.instances:
                if inst.alive():
                    inst.shot(f"failed{inst.suffix}")
        except (Exception, SystemExit) as e:     # a broken harness, not a broken game
            self.result, self.reason = "ERROR", f"{type(e).__name__}: {e}"
        finally:
            self.duration = time.time() - self.t0
            for inst in self.instances:
                inst.alive()
                inst.stop()
            self.check_wine_logs()
            self.note(f"result: {self.result} {self.reason}")
            self.log.close()
            self.write_result()
            if lock:
                lock.close()

    def check_wine_logs(self) -> None:
        for inst in self.instances:
            path = self.out / f"wine{inst.suffix}.log"
            if not path.exists():
                continue
            text = path.read_text(errors="replace")
            crash = re.findall(r"^.*(?:Unhandled exception|Unhandled page fault|page fault on).*$", text, re.M)
            errs = [l for l in text.splitlines() if re.match(r"^[0-9a-f]{4}:err:", l)]
            if crash:
                self.notes.append(f"wine{inst.suffix}: {crash[0].strip()}")
                if self.result == "PASS":
                    self.result, self.reason = "FAIL", f"crash: {crash[0].strip()}"
            if errs:
                self.notes.append(f"wine{inst.suffix}: {len(errs)} err lines, first: {errs[0].strip()}")

    def write_result(self) -> None:
        music = {}
        for inst in self.instances:
            music.update({f"{t}{inst.suffix}": round(v, 1) for t, v in inst.music.items()})
        data = {
            "run": self.name, "scenario": self.scenario.name, "exe": self.exe_tag,
            "paths": [str(e) for e in self.exes],
            "result": self.result, "reason": self.reason, "seconds": round(self.duration, 1),
            "exit_codes": [i.rc for i in self.instances], "events": self.events, "notes": self.notes,
            "music": music, "sound_seconds": [i.sound_seconds for i in self.instances],
            "music_seconds": [i.music_seconds for i in self.instances],
            "loudest": [i.loudest for i in self.instances],
        }
        (self.out / "result.json").write_text(json.dumps(data, indent=1) + "\n")


# --- the session ----------------------------------------------------------


def resolve_exes(values: list[str]) -> list[list[tuple[str, Path]]]:
    """Each --exe choice: the exe of each instance, `orig+carve` giving the
    first instance orig and the second (and any more) carve."""
    out = []
    for v in values:
        for choice in v.split(","):
            parts = []
            for item in choice.split("+"):
                if item in EXES:
                    path = EXES[item]
                    tag = item
                else:
                    path = Path(item).resolve()
                    tag = path.parent.name if path.name.lower() == "totala.exe" else path.stem
                if not path.exists():
                    hint = {"place": "uv run tools/place.py", "carve": "uv run tools/link.py --carve"}.get(item)
                    raise SystemExit(f"{path} is missing" + (f": build it with {hint}" if hint else ""))
                parts.append((tag, path))
            out.append(parts)
    return out


def stub_traps(exe: Path, out: Path) -> str:
    """Write a copy of an ordinary link whose stubs fault when reached.

    tools/link.py --carve fills the names nothing defines from stubs.obj: a
    function stub returns 0, and the data stub DAT_00000000 is a zero-filled
    word where the original reads address 0. In the copy every function stub
    starts with int3 and every reference to a data stub holds 0 again, so a
    run reports the stub it reached as a crash (at the stub's address, see the
    map) instead of carrying on without what the original would have done.
    """
    import pefile

    map_text = exe.with_suffix(".map").read_text(errors="replace")
    pe = pefile.PE(str(exe))
    base = pe.OPTIONAL_HEADER.ImageBase
    data = bytearray(exe.read_bytes())
    funcs = datas = refs = 0
    for line in map_text.splitlines():
        m = re.match(r"\s*[0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{8})\s+(f\s+)?\S*stubs\.obj\s*$", line)
        if not m:
            continue
        va = int(m.group(2), 16)
        if m.group(3):
            off = pe.get_offset_from_rva(va - base)
            if data[off] != 0x31:           # xor eax, eax; ret
                raise SystemExit(f"{exe}: {m.group(1)} at {va:#x} is not a stub")
            data[off] = 0xCC
            funcs += 1
            continue
        datas += 1
        needle = va.to_bytes(4, "little")
        for sec in pe.sections:
            if not sec.Characteristics & 0x20000000:      # code
                continue
            lo, hi = sec.PointerToRawData, sec.PointerToRawData + sec.SizeOfRawData
            i = data.find(needle, lo, hi)
            while i >= 0:
                data[i:i + 4] = bytes(4)
                refs += 1
                i = data.find(needle, i + 4, hi)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(bytes(data))
    return f"{funcs} function stubs trapped, {refs} references to {datas} data stubs pointed at 0"


def compare_shots(runs: list[Run], session: Path) -> dict[str, list[str]]:
    """Each run's named screenshots against the first original run of its
    scenario: a count for the summary, and every difference in screens.md."""
    out: dict[str, list[str]] = {}
    detail = [f"# Screenshots against the original's, {session.name}", ""]
    refs = {}
    for r in runs:
        if r.exe_tag == "orig" and r.scenario.name not in refs:
            refs[r.scenario.name] = r
    for r in runs:
        ref = refs.get(r.scenario.name)
        if ref is None or ref is r:
            continue
        same, differ = 0, []
        for shot in sorted(r.out.glob("*.png")):
            other = ref.out / shot.name
            if shot.name.startswith(("timeout_", "failed", "expect_")) or not other.exists():
                continue
            res = subprocess.run(["compare", "-metric", "AE", str(shot), str(other), "null:"],
                                 capture_output=True, text=True)
            try:
                diff = int(float(res.stderr.split()[0]))
            except (IndexError, ValueError):
                diff = -1
            if diff == 0:
                same += 1
            else:
                differ.append(f"{shot.stem} {diff}px")
        if same or differ:
            out[r.name] = [f"{same} of {same + len(differ)} screenshots identical to {ref.name}"]
            detail.append(f"- {r.name}: {same} of {same + len(differ)} identical to {ref.name}"
                          + (f"; pixels that differ: {', '.join(differ)}" if differ else ""))
    if len(detail) > 2:
        (session / "screens.md").write_text("\n".join(detail) + "\n")
    return out


def summary(session: Path, runs: list[Run], shots: dict[str, list[str]]) -> str:
    lines = [f"# Play tests {session.name}", "",
             "| run | scenario | exe | result | time | exit | notes |",
             "| --- | --- | --- | --- | ---: | --- | --- |"]
    for r in runs:
        notes = []
        if r.reason:
            notes.append(r.reason)
        notes += r.events + r.notes + shots.get(r.name, [])
        tracks = sorted({t for i in r.instances for t in i.music}, key=lambda t: (len(t), t))
        if tracks:
            notes.append(f"tracks opened: {', '.join(tracks)}")
        sound = [i.sound_seconds for i in r.instances]
        if any(sound):
            with_music = [i.music_seconds for i in r.instances]
            notes.append(f"sound in {'/'.join(map(str, sound))} s ({'/'.join(map(str, with_music))} s with "
                         f"a track open)")
        rc = "/".join("-" if i.rc is None else str(i.rc) for i in r.instances)
        m, s = divmod(int(r.duration), 60)
        lines.append(f"| {r.name} | {r.scenario.name} | {r.exe_tag} | {r.result} | {m}:{s:02d} | {rc} | "
                     f"{'; '.join(notes)} |")
    return "\n".join(lines) + "\n"


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe", action="append", default=[],
                    help="orig, place, carve or a path (repeat, or commas); A+B: A in instance 1, B in the others")
    ap.add_argument("--scenario", action="append", default=[], help="a tools/playtest/ scenario (repeat, or commas)")
    ap.add_argument("--repeat", type=int, default=1, help="run each pair this many times")
    ap.add_argument("--parallel", type=int, default=0, help="runs at once (default: all of them, at most 3)")
    ap.add_argument("--game-dir", help="the game's install (default: Steam's)")
    ap.add_argument("--debug", default="-all,err+seh", help="WINEDEBUG for the game (default -all,err+seh)")
    ap.add_argument("--window", action="store_true",
                    help="show each game in a Xephyr window on the desktop (default: an invisible Xvfb display)")
    ap.add_argument("--keep", action="store_true", help="keep each run's prefix and game directory")
    ap.add_argument("--trap-stubs", action="store_true",
                    help="play copies of ordinary links (an exe with a map) whose stubs fault when reached")
    ap.add_argument("--list", action="store_true", help="list the scenarios")
    ap.add_argument("--signature", nargs="+", metavar="NAME GEOMETRY IMAGE",
                    help="print a screens.txt line for an area of screenshots of one screen")
    ap.add_argument("--mask", action="store_true", help="--signature: a text mask, for text over terrain")
    ap.add_argument("--cell", type=int, default=4)
    ap.add_argument("--tolerance", type=float, default=8)
    ap.add_argument("--match", nargs="+", metavar="IMAGE", help="which known screens these screenshots show")
    args = ap.parse_args()

    if args.list:
        list_scenarios()
        return
    if args.signature:
        if len(args.signature) < 3:
            ap.error("--signature NAME WxH+X+Y SCREENSHOT...")
        name, geo, *images = args.signature
        print(make_signature(name, geo, [Path(i) for i in images], "mask" if args.mask else "grid", args.cell,
                             args.tolerance))
        return
    screens = load_screens()
    if args.match:
        for image in args.match:
            img = Gray.load(Path(image))
            found = [f"{s.name}({s.distance(img):.2f})" for s in screens.values() if s.matches(img)]
            print(f"{image}: {' '.join(found) or '-'}")
        return
    if not args.exe or not args.scenario:
        ap.error("--exe and --scenario are required")
    for tool in ("Xephyr" if args.window else "Xvfb", "xdotool", "import", "wine", "pactl", "parec"):
        if not shutil.which(tool):
            raise SystemExit(f"{tool} is not installed")
    exes = resolve_exes(args.exe)
    scenarios = [load_scenario(s) for v in args.scenario for s in v.split(",")]
    for sc in scenarios:
        check_scenario(sc, screens)
    game_src = find_game_dir(args.game_dir)

    session = OUT / datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
    session.mkdir(parents=True, exist_ok=True)
    latest = OUT / "latest"
    if latest.is_symlink() or latest.exists():
        latest.unlink()
    latest.symlink_to(session.name)
    if args.trap_stubs:
        copies: dict[Path, tuple[str, Path]] = {}
        for parts in exes:
            for i, (tag, path) in enumerate(parts):
                if path not in copies:
                    copies[path] = (tag, path)
                    map_path = path.with_suffix(".map")
                    if map_path.exists() and "stubs.obj" in map_path.read_text(errors="replace"):
                        copy = session / "exes" / f"{tag}-trap.exe"
                        print(f"{copy.relative_to(ROOT)}: {stub_traps(path, copy)}", flush=True)
                        copies[path] = (f"{tag}-trap", copy)
                parts[i] = copies[path]
    runs: list[Run] = []
    for sc in scenarios:
        for parts in exes:
            tag = "+".join(t for t, _ in parts)
            for k in range(args.repeat):
                name = f"{sc.name}-{tag}" + (f"-{k + 1}" if args.repeat > 1 else "")
                runs.append(Run(len(runs) + 1, name, sc, [p for _, p in parts], tag, session, screens,
                                args.debug, args.window))
    parallel = args.parallel or min(3, len(runs))
    template = template_prefix()
    dplay = directplay_prefix() if any(sc.directplay for sc in scenarios) else None

    atexit.register(stop_all)

    def interrupt(*_args) -> None:
        # Only set the flag: the main loop stops the games (a handler that took locks could
        # deadlock), each run ends as INTERRUPTED and the summary is still written.
        _interrupted.set()

    for sig in (signal.SIGINT, signal.SIGTERM, signal.SIGHUP):
        signal.signal(sig, interrupt)

    print(f"{len(runs)} run(s), {parallel} at a time, in {session.relative_to(ROOT)}", flush=True)
    pending = list(runs)
    active: list[threading.Thread] = []
    lock = threading.Lock()

    def work(run: Run) -> None:
        run.execute(dplay if run.scenario.directplay else template, game_src)
        if not args.keep:
            for inst in run.instances:
                inst.clean()
            shutil.rmtree(run.work, ignore_errors=True)
        with lock:
            m, s = divmod(int(run.duration), 60)
            print(f"{run.result:5} {run.name} ({m}:{s:02d}) {run.reason}", flush=True)

    while pending or active:
        active = [t for t in active if t.is_alive()]
        if _interrupted.is_set():
            for run in pending:
                run.result = "SKIPPED"
            pending = []
            stop_all()
        while pending and len(active) < parallel:
            t = threading.Thread(target=work, args=(pending.pop(0),))
            t.start()
            active.append(t)
            time.sleep(2)
        time.sleep(1)

    shots = compare_shots(runs, session)
    text = summary(session, runs, shots)
    (session / "summary.md").write_text(text)
    print()
    print(text, end="")
    print(f"\nlogs and screenshots: {session.relative_to(ROOT)}/")
    sys.exit(130 if _interrupted.is_set() else 0 if all(r.result == "PASS" for r in runs) else 1)


if __name__ == "__main__":
    main()
