# Contributing

Help is welcome, mostly in the form of running a coding agent (Codex,
OpenCode, Claude Code) on the issues. Every function already matches; the
issues now are cleanup, making the source read like source without changing a
byte. Every pull request is re-checked by the maintainers before it is merged,
so a wrong answer costs you some tokens but can't break anything.

## Setting up

You need Linux (or WSL) with `wine`, `7z`, `cabextract`, `curl`, `git`, the
GitHub CLI (`gh`, logged in) and [uv](https://docs.astral.sh/uv/). `gdb` is
optional; `tools/c2prio.py`, which reads the compiler's register priorities,
needs it. You also need your own copy of Total Annihilation: the Steam version
is the one the project matches.

```sh
git clone https://github.com/HectorBailey/byte-tactics
cd byte-tactics
tools/setup_toolchain.sh        # Visual C++ 5.0 + SP3, Ghidra and zlib into toolchain/;
                                # copies TotalA.exe from Steam (set STEAM_TA if needed)
tools/ghidra.sh                 # optional but helpful: Ghidra pseudo-C for every function
                                # (needs Java 21; takes a while the first time)
uv run tools/check.py 0x401070  # must print MATCH
```

## Running an agent

Start your agent in the repository folder. Codex and OpenCode both read
`AGENTS.md` automatically, and that file tells the agent everything: how to
claim an issue, work in its own copy, and open a pull request from your fork.
Give it this prompt:

> Follow AGENTS.md: pick up a `cleanup` issue labelled `ready`, do it and
> open a pull request. Then pick up the next one, until none are left.

In short:

- Take only issues labelled `ready`; the others wait on work that has not
  merged yet, and become `ready` by themselves.
- Every function has to keep matching and the build has to keep the shipped
  MD5. `tools/place.py` checks the whole exe; it compiles every changed file,
  so a run takes a while.
- Please have one or two issues claimed at a time rather than many.

## What happens to your pull request

A maintainer re-checks it (every function with `tools/check.py`, and the
exe's MD5) and merges it. CI cannot build a pull request from a fork until a
maintainer approves the run, so it may show as waiting for a while. The
decompile round's credits stay in the `// Decompiled by ...` lines and the git
history.

The reconstructed game code is not licensed (see the README); the tools are
MIT. Please don't commit any game files or Microsoft software.
