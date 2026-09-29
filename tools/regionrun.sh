#!/usr/bin/env bash
# Start one opencode agent per region of a big function, each in its own
# worktree, and kill every agent at a fixed deadline.
#
#   tools/regionrun.sh <addr> <base-branch> <brief.md> <log-dir> <minutes> [model] [region ...]
#
# Regions default to the names in data/regions/<addr>.csv. The base branch must
# hold the skeleton with `// REGION <name> begin/end` markers. Worktrees are
# .worktrees/<addr>-<name> on branches region-<addr>-<name>.
#
# The deadline is counted from each agent's own start. At most MAX_PARALLEL
# (default 4) agents run at once, and a new one waits until the container's
# pids cgroup has PIDS_PER_AGENT (default 150) free slots: every agent uses
# 55 to 70 threads plus Wine for each compile, and a capped container fails
# thread spawns (uv and tokio panics) well before the CPU is busy. The launcher
# stays in the foreground while it waits, so run it in the background.
#
# The skeleton commit is written to <log-dir>/base.txt. Check every branch with
#   tools/regguard.py <file> --base-file <log-dir>/base.txt --head <branch> --region <name>
# before merging it. The base must be that fixed commit, not the moving branch.
# DRY_RUN=1 prints the plan without creating anything.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ADDR="${1:?address}"; BASE="${2:?base branch}"; BRIEF="${3:?brief}"; LOGS="${4:?log dir}"
MINUTES="${5:?minutes}"
MODEL="${6:-opencode-go/deepseek-v4.1-flash}"
if [ $# -ge 6 ]; then shift 6; else shift $#; fi
MAX_PARALLEL="${MAX_PARALLEL:-4}"
PIDS_PER_AGENT="${PIDS_PER_AGENT:-150}"
SK="$HOME/.claude-home/skills/opencode-subagents/scripts"
CSV="$ROOT/data/regions/$ADDR.csv"
mkdir -p "$LOGS"
LOGS="$(cd "$LOGS" && pwd)"
if [ $# -gt 0 ]; then REGIONS=("$@"); else mapfile -t REGIONS < <(tail -n +2 "$CSV" | cut -d, -f1); fi
git -C "$ROOT" rev-parse "$BASE" > "$LOGS/base.txt"

pids_headroom() {
    local cg max cur
    cg="$(sed -n 's/^0:://p' /proc/self/cgroup)"
    max="$(cat "/sys/fs/cgroup$cg/pids.max" 2>/dev/null || echo max)"
    [ "$max" = max ] && { echo 100000; return; }
    cur="$(cat "/sys/fs/cgroup$cg/pids.current" 2>/dev/null || echo 0)"
    echo $((max - cur))
}

running() {
    local n=0 r
    for r in "${STARTED[@]:-}"; do
        [ -n "$r" ] && ! grep -q '^result:' "$LOGS/runner-$r.out" 2>/dev/null && n=$((n + 1))
    done
    echo "$n"
}

STARTED=()
for R in "${REGIONS[@]}"; do
    if [ -z "${DRY_RUN:-}" ]; then
        while [ "$(running)" -ge "$MAX_PARALLEL" ] || [ "$(pids_headroom)" -lt "$PIDS_PER_AGENT" ]; do
            sleep 5
        done
    fi
    WT="$ROOT/.worktrees/$ADDR-$R"
    ROW="$(grep "^$R," "$CSV" || true)"
    if [ -n "${DRY_RUN:-}" ]; then
        echo "would start $R in $WT ($ROW), running=$(running), pids headroom=$(pids_headroom)"
        continue
    fi
    [ -d "$WT" ] || git -C "$ROOT" worktree add -q -b "region-$ADDR-$R" "$WT" "$BASE"
    ln -sfn "$ROOT/toolchain" "$WT/toolchain"
    mkdir -p "$WT/orig" "$WT/build"
    ln -sf "$ROOT/orig/TotalA.exe" "$WT/orig/TotalA.exe"
    [ -e "$ROOT/build/ghidra" ] && ln -sfn "$ROOT/build/ghidra" "$WT/build/ghidra"
    cat > "$LOGS/task-$R.md" <<EOF
# Task: region $R of $ADDR

Worktree: $WT (branch region-$ADDR-$R, based on $BASE)
Your region: \`// REGION $R begin\` .. \`// REGION $R end\` in src/unsorted/$ADDR.cpp
Original address range: ${ROW:-see data/regions/$ADDR.csv} (name,start,end)
Goal: raise this region's shape score to 100, then exact, per the brief.
Hard deadline: $MINUTES minutes from now (started $(date +%H:%M)). Commit each improvement.
EOF
    BEFORE="$(ls -t "$LOGS/$ADDR-$R-"*.env 2>/dev/null | head -1 || true)"
    : > "$LOGS/runner-$R.out"
    (cd "$WT" && setsid nohup "$SK/run-agent.sh" "$WT" "$BRIEF" "$LOGS/task-$R.md" "$LOGS" "$MODEL" \
        > "$LOGS/runner-$R.out" 2>&1 &)
    ENVF=""
    for _ in $(seq 1 60); do
        NEW="$(ls -t "$LOGS/$ADDR-$R-"*.env 2>/dev/null | head -1 || true)"
        if [ -n "$NEW" ] && [ "$NEW" != "$BEFORE" ]; then ENVF="$NEW"; break; fi
        sleep 1
    done
    (
        sleep $((MINUTES * 60))
        if [ -n "$ENVF" ] && ! grep -q '^result:' "$LOGS/runner-$R.out" 2>/dev/null; then
            # shellcheck disable=SC1090
            PID="$(. "$ENVF"; echo "$PID")"
            kill -- "-$PID" 2>/dev/null || true
            echo "result: killed at deadline ($MINUTES min)" >> "$LOGS/runner-$R.out"
        fi
    ) </dev/null >/dev/null 2>&1 &
    disown
    STARTED+=("$R")
    echo "started $R in $WT at $(date +%H:%M:%S)"
done
echo "all started; each agent has $MINUTES minutes from its own start"
