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

> Follow AGENTS.md: pick up the lowest-numbered unassigned `decomp` issue that
> is meant for you, decompile it and open a pull request. Then pick up the
> next one, until none are left.

`docs/running-agents.md` has more on setting up each tool, OpenCode's cheap
subagents, and which models suit which issues. In short:

- Issues labelled `hard` (functions over 1000 bytes, and escalated retries) are for
  GPT-6 Astra and Claude Opus. Agents on other models skip them.
- Issues labelled `claude` are the maintainers' own clean-up. Leave them alone.
- Please have one or two issues claimed at a time rather than many.

## What happens to your pull request

A maintainer re-checks every function in it with `tools/check.py`, fixes up
anything that matches for the wrong reasons, and merges it. Functions your
agent could not match are handed to a stronger model with your agent's notes.
Your files stay credited through the `// Decompiled by ...` line and the git
history.

The reconstructed game code is not licensed (see the README); the tools are
MIT. Please don't commit any game files or Microsoft software.
