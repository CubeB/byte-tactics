# Routing stuck functions

Issue #4841's experiments settled a few things about the near-miss tail:

- On functions at 90% and above, both the permuter and a model proposal
  loop sit on the same plateau: most rewrites compile to code byte-identical
  to the current source. Neither a model reading the diff nor 10,000 more
  random candidates moves that band. What helps there is understanding, so
  do not burn issue slots on the batch proposal loop at that score.
- In the 70 to 90% band the model has leverage the mutation catalogue does
  not: it matched 0x47de60 after about 11,500 random candidates and five
  passes had failed. Route that band through `tools/propose.py`.
- The residual on the near-miss tail is usually decided by the back end
  (register colouring, SIB base swaps), not the front end.

## Triage: classify.py and jev_route.py

`tools/classify.py` buckets a function from the diff alone, using the same
instruction penalties as the permuter:

```sh
uv run tools/classify.py 0x4ba000
uv run tools/classify.py --band 70 90 --csv out.csv
```

- `frame`: prologue or frame differs (pushes, [esp+N] slots). See
  docs/splitting-huge-functions.md's skeleton fix.
- `slots`: same instructions, only registers or stack slots differ. A
  permuter run, or a regalloc-focused prompt with the guide sections.
- `shape`: something else (moved, missing or extra instructions). A strong
  model pass; likely a wrong type, callee or loop shape.

`tools/jev_route.py` asks Jev (`~typesafe/jev-latest` on OpenRouter's
`/api/alpha/decisions`) the same question as a typed choice, with the
`compiler_state` route added (the source is right; the residual is a
scheduler or inliner tie):

```sh
uv run tools/jev_route.py --band 90 99 --csv routes.csv
```

The state is a diff summary, not a conversation. Every answer carries a
top probability; below 0.65, do not act on it, fall back to an LLM. Cost
is about $0.0001 a function. The API key lives in `.envrc` at the repo
root (never commit it; it is in `.git/info/exclude`).

## Proposal loop: propose.py

Score a batch of whole-file variants in parallel, the way the #4841
experiment ran:

```sh
uv run tools/propose.py 0x4c0a90 v1.cpp v2.cpp ... --top 3
uv run tools/propose.py 0x4c0a90 --base mine.cpp variants/*.cpp
```

Each variant is one whole source file under `src/`. It is compiled with
check.py's `compile_source` and scored with the permuter's `Scorer`, plus
check.py's own ratio and a shape ratio with jump targets masked. Twelve
variants compile in one parallel batch; it never writes to `src/`, the
best few land in `build/propose/<address>/` with `--top`.

Use it in the 70 to 90% band: read the diff and the file's notes, write
8 to 16 whole-file variants, score them in one batch, and start the next
round from the best. Two rounds is usually enough. The useful output when
nothing scores better is notes for the file's header comment.
