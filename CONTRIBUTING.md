# Contributing

Help is welcome, mostly in the form of running a coding agent (Codex,
OpenCode, Claude Code) on the issues. Every function an agent claims to match
is re-checked by the maintainers before it is merged, so a wrong answer costs
you some tokens but can't break anything.

## Setting up

You need Linux (or WSL) with `wine`, `7z`, `cabextract`, `curl`, `git`, the
GitHub CLI (`gh`, logged in) and [uv](https://docs.astral.sh/uv/). You also
need your own copy of Total Annihilation: the Steam version is the one the
project matches.

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

> Follow AGENTS.md: pick up the lowest-numbered unassigned `decomp` issue,
> decompile it and open a pull request. Then pick up the next one, until none
> are left.

`docs/running-agents.md` has more on setting up each tool, OpenCode's cheap
subagents, and which models suit which issues. In short:

- Any model may take any `decomp` issue. Issues labelled `hard` hold
  functions over 1000 bytes.
- There is no time limit: agents keep working on a function until it matches
  or its best score stops improving (30 check runs or 60 minutes without a
  new best). See `AGENTS.md`.
- Please have one or two issues claimed at a time rather than many.
- When a function is close (about 90% or more) and stuck, agents run the
  permuter, `uv run tools/permute.py <addr>`, which tries thousands of
  meaning-preserving rewrites for 15 minutes (`docs/permuter.md`). It needs no
  extra setup, but it runs 12 compiles at once: on a smaller machine, or with
  several agents running, tell your agent to pass `--jobs 4`. Its output has
  to be tidied into plausible source before it is committed (`AGENTS.md`);
  pull requests with its raw leftovers are held for a look.

## What happens to your pull request

A maintainer re-checks every function in it with `tools/check.py`, fixes up
anything that matches for the wrong reasons, and merges it. Functions your
agent could not match go back out as a retry issue, with your agent's notes.
Your files stay credited through the `// Decompiled by ...` line and the git
history.

The reconstructed game code is not licensed (see the README); the tools are
MIT. Please don't commit any game files or Microsoft software.
