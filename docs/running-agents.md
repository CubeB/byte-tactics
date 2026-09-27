# Running agents on the project

Work is handed out as GitHub issues labelled `decomp`, each listing a few
functions. Any coding agent can take part: it reads `AGENTS.md` (OpenCode and
Codex both load it automatically), claims an unassigned issue, works in its own
copy of the repository and opens a pull request. The orchestrator re-checks the
pull request, merges it, records which model did what, and opens new issues.

## One-time setup

Everything runs from the main checkout, `~/repos/personal/byte-tactics`, which
must already have the toolchain (`tools/setup_toolchain.sh`), the original exe
in `orig/` and the Ghidra export in `build/ghidra/`. The agent also needs the
GitHub CLI logged in (`gh auth status`), because it claims issues, pushes
branches and opens pull requests with it.

Agents create their working copies in `.worktrees/issue-<N>` (ignored by git),
linked to the main checkout's toolchain, so several can run at once from one
machine.

## OpenCode

```sh
cd ~/repos/personal/byte-tactics
opencode
```

OpenCode runs as a lead plus cheap workers:

- **The lead** is the model you pick with `/models`, ideally GLM-5.3 or
  Grok 4.7. It claims the issue, sets up the worktree, reviews the results,
  retries what the workers left and opens the pull request.
- **The workers** are the `decomp-worker` subagent defined in
  `.opencode/agents/decomp-worker.md`. They run on DeepSeek V4.1 Flash
  (`opencode-go/deepseek-v4.1-flash`) and do the first attempt at each
  function. Up to four run at once.
- **Limits:** a worker stops after 80 steps (the file's `steps`), and
  `AGENTS.md` caps each function at 15 check runs or 20 minutes. Nothing gets
  stuck for long.

To use a different worker model, change the `model:` line in that file. Your
OpenCode Go plan limits spending per model: DeepSeek V4.1 Flash and Kimi K3
have lower caps than GLM-5.3 and Grok 4.7, and each cap applies per 5 hours,
per week and per month.

Give the lead this prompt:

> Follow AGENTS.md: pick up the lowest-numbered unassigned `decomp` issue,
> hand its functions to decomp-worker subagents, retry what they leave, and
> open a pull request. Then pick up the next one, until none are left.

`AGENTS.md` keeps OpenCode models off issues labelled `hard` (larger functions
and near-misses); those are for GPT-6 Astra and Claude Opus. To steer a model
further, add a size label to the prompt, for example "only take `size:medium`
issues".

For an unattended run, `opencode run` takes the same prompt on the command
line, with `-m opencode-go/glm-5.3` to choose the lead (check
`opencode run --help`). Run several in separate terminals; each claims a
different issue.

OpenCode asks before running shell commands unless you allow them. The agent
needs to run `uv`, `gh`, `git` and the compiler (Wine) freely, so allow those
for this project.

## Codex

```sh
cd ~/repos/personal/byte-tactics
codex
```

Choose GPT-6 Astra with `/model` and give it this prompt:

> Follow AGENTS.md: pick up the lowest-numbered unassigned `decomp` issue
> labelled `hard`, decompile it and open a pull request. Then pick up the next
> `hard` one, until none are left.

`AGENTS.md` already tells Astra to prefer `hard` issues, so the plain prompt
works too; this one keeps it from moving on to easier issues when the hard
ones run out. For an unattended run, `codex exec "<prompt>"`.

Codex runs commands in a sandbox. The agent needs network access (for `gh`
and `git push`) and needs to run Wine. If either is blocked, start Codex with
`--sandbox danger-full-access` (check `codex --help` for the current flag
names). This is your own repository and machine, and the agent only needs the
repository folder.

## Which model

Last night's calibration (`docs/agents.md`) covered Claude models only. Opus
matched 99-100% of functions up to 160 bytes and about 80% at 161-260 bytes.
What's left is mostly the harder, larger functions.

- **Codex, GPT-6 Astra.** OpenAI reports it solves 88% of a
  binary reverse-engineering benchmark first time. That is not the same task
  as matching decompilation, but it makes Astra the strongest candidate for
  the `hard` issues (large and huge functions, near-misses), which are
  reserved for it and Claude Opus.
- **OpenCode.** There is no track record here for any of these models. The
  likeliest candidates are the larger, non-Flash ones (GPT-6 Luna, Kimi K3,
  GLM-5.3, Qwen3.8 Max, MiMo-V2.6-Pro, Grok 4.7). Give two or three of them a
  `size:medium` issue each and compare.
- **Measuring.** Every pull request names its model, and `tools/record.py`
  logs the orchestrator's re-check under that name. `docs/agents.md` then
  shows each model's match rate by function size. Use those numbers, not
  reputation, to decide who gets which issues. If a plan is flat-rate rather
  than per token, match rate is the number that matters.

## The orchestrator's loop

Run from the main checkout, on `main`:

1. **Keep issues open.** Open a handful per size:
   - `uv run tools/issues.py --band medium --count 6`
   - `uv run tools/issues.py --band large --count 6`

   Near-misses to retry with a stronger model:
   - `uv run tools/issues.py --addresses ... --title "Near-misses" --label near-miss --escalation`
2. **Review each pull request.**
   - `tools/review.sh <PR>` checks the pull request out in `.worktrees/pr-<PR>`.
     It lists the changed files, re-checks every function in them and flags
     forbidden constructs.
   - Read the files for made-up names, and fix bad matches before merging.
   - Squash-merge with a commit message in the project's format:
     `gh pr merge <PR> --squash --delete-branch --subject "Add: ..." --body "..."`.
   - `tools/review.sh <PR> --clean` removes the worktree.
3. **Merge and record.** Squash-merge, then on `main`:
   - `uv run tools/record.py <issue> <model> --escalate claude`. Add
     `--model-for <addr>=<model>` for each function another model (such as a
     worker) wrote. `--escalate claude` opens a retry issue labelled `claude`
     for everything left unmatched. Use `--escalate hard` instead only to give
     GPT-6 Astra a go at it.
   - `uv run tools/progress.py`
   - `uv run tools/calibration.py`

   Add any suspected original bugs to `docs/bugs.md` and new techniques to
   `docs/agent-guide.md`, commit and push.
4. **Clean up what was left.** Issues labelled `claude` hold what the other
   agents left unmatched (`gave up`, `not reached`), mostly from the cheap
   OpenCode workers. They are the orchestrator's own work, and every other
   agent is told to skip them. The orchestrator hands them to Claude Opus
   subagents working in the main checkout. It re-checks and commits their
   files directly, and closes the issue.
5. **Fix bad matches too.** A cheap model's file can match and still be wrong
   in other ways: `__fastcall` free functions, hand-stored vtables, invented
   names. The orchestrator fixes those during review, or with a subagent,
   before merging.
6. **Release stale claims.** For claims older than a day with no pull request,
   unassign the issue and comment "Released".
