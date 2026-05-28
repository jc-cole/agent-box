# AgentBox MVP Spec

## 1. Project Summary

**AgentBox** is a local CLI tool for safely running and comparing AI coding agents on the same software task.

It creates disposable **git worktrees** for each agent run, launches configured agent commands inside those worktrees, captures logs, runs tests, summarizes diffs, and lets the user accept or reject an agent’s result without dirtying the main working tree.

### One-sentence pitch

> AgentBox lets developers A/B test coding agents like Codex, Claude Code, Aider, or sketchy local agents in isolated git worktrees, then compare diffs/test results and accept the best patch.

---

## 2. MVP Goal

The MVP should prove one core workflow:

> Given a clean git repo and a coding task, AgentBox can run two or more configured agents from the same base commit in separate worktrees, collect their outputs, run tests, compare results, and apply the chosen patch back to the main repo.

### Goal demo

```bash
agentbox compare \
  --agents codex,sketch \
  --task "Fix parser crash on empty input" \
  --test "make test"
```

Expected output:

```text
Task: Fix parser crash on empty input
Base commit: a81f2d9

Agent     Status      Tests      Files Changed   Branch
codex     complete    passed     2               agent/task-001/codex
sketch    complete    failed     5               agent/task-001/sketch

Recommended: codex
Next:
  agentbox diff task-001 --agent codex
  agentbox accept task-001 --agent codex
  agentbox clean task-001
```

---

## 3. MVP Non-Goals

Do **not** build these in the first version:

- Full OS-level sandboxing
- Perfect malicious-agent isolation
- TUI dashboard
- Parallel agent execution
- Universal command interception
- MCP integration
- HTTP/webhook-triggered agents
- VM/container orchestration
- Complex multi-agent planning
- Agent-to-agent communication

The MVP is about **git/workspace isolation and comparison**, not perfect security.

---

## 4. Core Concepts

### Project

The real user repository where AgentBox is initialized.

### Task

A specific coding objective, such as:

```text
Fix parser crash on empty input
```

A task records:

- Task ID
- Title/prompt
- Base commit
- Status
- Agent runs

### Agent Run

One attempt by one configured agent to solve a task.

An agent run records:

- Agent name
- Branch name
- Worktree path
- Log path
- Exit code
- Test command/result
- Changed file count
- Status

### Worktree

A disposable git working directory created from the same base commit as other runs.

Example:

```text
.agentbox/worktrees/task-001-codex/
.agentbox/worktrees/task-001-sketch/
```

Each worktree has its own branch:

```text
agent/task-001/codex
agent/task-001/sketch
```

---

## 5. CLI Commands

### `agentbox init`

Initializes AgentBox in the current git repo.

Creates:

```text
.agentbox/
  config.toml
  tasks/
  logs/
  worktrees/
```

MVP behavior:

- Verify current directory is inside a git repo.
- Create `.agentbox/` if missing.
- Create default config.

---

### `agentbox compare`

Runs multiple agents on the same task from the same base commit.

Example:

```bash
agentbox compare \
  --agents codex,sketch \
  --task "Fix failing parser test" \
  --test "make test"
```

MVP behavior:

1. Confirm repo is clean.
2. Create task metadata.
3. Create one branch/worktree per agent.
4. Write task instruction file into each worktree.
5. Launch each agent command sequentially.
6. Capture stdout/stderr to logs.
7. Run test command in each worktree.
8. Print comparison summary.

Parallel execution is a post-MVP feature.

---

### `agentbox run <task-id> --agent <agent>`

Runs one configured agent on an existing task.

Example:

```bash
agentbox run task-001 --agent codex
```

MVP behavior:

- Create branch/worktree if not already present.
- Generate `AGENTBOX_TASK.md` inside the worktree.
- Launch configured agent command in the worktree.
- Capture logs.
- Update task metadata.

---

### `agentbox test <task-id> --agent <agent>`

Runs the test command inside an agent worktree.

Example:

```bash
agentbox test task-001 --agent codex
```

MVP behavior:

- Run configured/default test command.
- Capture stdout/stderr.
- Store exit code.
- Update task metadata.

---

### `agentbox diff <task-id> --agent <agent>`

Shows the diff for one agent run.

Example:

```bash
agentbox diff task-001 --agent codex
```

MVP behavior:

- Show `git diff --stat`.
- Show full `git diff`.

---

### `agentbox status`

Lists known tasks and agent run statuses.

Example output:

```text
Task       Agent    Status      Tests     Files Changed
001        codex    complete    passed    2
001        sketch   complete    failed    5
```

---

### `agentbox accept <task-id> --agent <agent>`

Applies one agent’s result back to the main working tree.

Example:

```bash
agentbox accept task-001 --agent codex
```

MVP behavior:

1. Confirm main repo is clean.
2. Show diff summary.
3. Ask for confirmation.
4. Export patch from agent worktree.
5. Apply patch to main repo.

Preferred MVP implementation:

```bash
git -C <worktree> diff > patch
git apply patch
```

This is simpler and more predictable than merging branches during MVP.

---

### `agentbox clean <task-id>`

Removes task worktrees and optionally deletes branches.

Example:

```bash
agentbox clean task-001
```

MVP behavior:

- Remove worktrees.
- Delete agent branches after confirmation.
- Keep logs by default.

---

## 6. Config File

Path:

```text
.agentbox/config.toml
```

Example:

```toml
[project]
default_test = "make test"

[workspace]
worktree_dir = ".agentbox/worktrees"
log_dir = ".agentbox/logs"
task_dir = ".agentbox/tasks"

[agents.codex]
cmd = "codex"
args = []
mode = "host"

[agents.claude]
cmd = "claude"
args = []
mode = "host"

[agents.sketch]
cmd = "python /Users/jared/agents/sketch/main.py"
args = []
mode = "host"
```

MVP only needs `mode = "host"`.

Container/sandbox modes come later.

---

## 7. Task Metadata

Path:

```text
.agentbox/tasks/task-001.toml
```

Example:

```toml
id = "task-001"
title = "Fix parser crash on empty input"
base_commit = "a81f2d93"
created_at = "2026-05-27T14:30:00"
status = "complete"

[[runs]]
agent = "codex"
branch = "agent/task-001/codex"
worktree = ".agentbox/worktrees/task-001-codex"
status = "complete"
exit_code = 0
test_command = "make test"
test_exit_code = 0
files_changed = 2
log = ".agentbox/logs/task-001-codex.log"
test_log = ".agentbox/logs/task-001-codex-test.log"

[[runs]]
agent = "sketch"
branch = "agent/task-001/sketch"
worktree = ".agentbox/worktrees/task-001-sketch"
status = "complete"
exit_code = 0
test_command = "make test"
test_exit_code = 2
files_changed = 5
log = ".agentbox/logs/task-001-sketch.log"
test_log = ".agentbox/logs/task-001-sketch-test.log"
```

---

## 8. Generated Task Instruction File

Each worktree should receive:

```text
AGENTBOX_TASK.md
```

Example:

```markdown
# AgentBox Task

## Task

Fix parser crash on empty input.

## Rules

- Work only in this repository.
- Prefer minimal changes.
- Do not modify unrelated files.
- Run `make test` before finishing if possible.
- Leave a concise summary of what changed.
```

AgentBox should launch the agent with a prompt like:

```text
Read AGENTBOX_TASK.md and complete the task.
```

---

## 9. Suggested Implementation Stack

Preferred MVP stack:

- C++20 or C++23
- Shell out to `git` instead of using libgit2
- `toml++` for TOML parsing
- `CLI11` for CLI parsing
- `fmt` or `spdlog` for output/logging
- Standard library filesystem/process helpers where possible

Core modules:

```text
src/
  main.cpp
  config.cpp/.hpp
  task.cpp/.hpp
  git.cpp/.hpp
  process.cpp/.hpp
  diff.cpp/.hpp
  paths.cpp/.hpp
```

MVP should prioritize correctness and workflow over fancy abstractions.

---

## 10. Internal Flow

### `compare` flow

```text
parse args
load config
verify git repo
verify clean working tree
create task ID
record base commit
for each agent:
  create branch
  create worktree
  write AGENTBOX_TASK.md
  run agent command in worktree
  capture log
  run test command if provided
  capture test log
  compute changed file count
  update task metadata
print comparison summary
```

### `accept` flow

```text
load task
find selected agent run
verify main repo clean
show diff stat
ask confirmation
export patch from worktree
apply patch to main repo
report success/failure
```

### `clean` flow

```text
load task
for each run:
  remove git worktree
  optionally delete branch
mark task cleaned
keep logs by default
```

---

## 11. Success Criteria for MVP

The MVP is successful when:

- It initializes inside a git repo.
- It can create disposable worktrees from the same base commit.
- It can run at least two configured commands/agents in separate worktrees.
- It captures logs for each agent run.
- It can run a test command in each worktree.
- It summarizes test result and changed files.
- It can show diffs for each run.
- It can apply the selected patch back to the main repo.
- It can clean up worktrees/branches.

A strong demo should show:

```text
Codex and a sketch/fake agent attempt the same bug fix.
Main repo remains clean during both attempts.
AgentBox compares results.
User accepts the better patch.
Main repo receives only the accepted change.
```

---

## 12. Project Stages

### Stage 0 — Manual workflow spike

Goal: prove the flow manually.

Tasks:

- Learn `git worktree` commands.
- Manually create two worktrees.
- Run two agent commands.
- Compare diffs.
- Apply winning patch.

Exit condition:

- You understand the workflow well enough to automate it.

---

### Stage 1 — Prototype CLI

Goal: hardcoded proof of concept.

Features:

- Create two worktrees.
- Run two hardcoded commands.
- Capture logs.
- Show diff stats.

Exit condition:

- One command can run two agent-like commands on the same task.

---

### Stage 2 — Real MVP

Goal: usable local tool.

Features:

- `init`
- config file
- configurable agents
- `compare`
- `run`
- `test`
- `diff`
- `status`
- `accept`
- `clean`

Exit condition:

- You can use AgentBox on a real small C/C++ repo.

---

### Stage 3 — UX polish

Goal: make it pleasant and understandable.

Features:

- Better error messages
- Colored summaries
- Log viewer
- Safer confirmation prompts
- Better task listing
- Improved diff summaries
- Better failure handling

Exit condition:

- Someone else could use it without you explaining every step.

---

### Stage 4 — Agent profiles

Goal: reusable workflows.

Example:

```toml
[profiles.test_fixer]
agents = ["codex", "claude"]
test = "make test"
prompt = "Fix failing tests with minimal changes. Do not refactor unrelated code."
```

Command:

```bash
agentbox compare --profile test_fixer --task "Fix parser edge case"
```

Exit condition:

- Repeated workflows do not require rewriting agent instructions each time.

---

### Stage 5 — Container mode

Goal: safer execution for sketchy agents.

Features:

- Run agent inside Docker/container.
- Mount only the worktree.
- Fake home directory.
- Strip environment variables.
- Optional network disable.
- CPU/memory/process limits.

Exit condition:

- A sketchy agent can run with reduced access to the host machine.

---

### Stage 6 — Parallel/TUI mode

Goal: better multi-agent monitoring.

Features:

- Run agents concurrently.
- Live status dashboard.
- Stream logs.
- Attach/kill/restart runs.

Exit condition:

- AgentBox feels like a real control center rather than a batch script.

---

### Stage 7 — Strong Linux sandbox mode

Goal: serious systems/security extension.

Possible mechanisms:

- Bubblewrap
- Linux namespaces
- cgroups
- Landlock
- seccomp
- read-only bind mounts
- network namespace isolation

Exit condition:

- Agent can only write inside workspace, cannot read host home, and has resource/network limits.

---

## 13. Nice-to-Have Features

Post-MVP ideas:

- Risk warnings for sensitive files changed
- Patch export
- Commit generation
- Rerun from original base commit
- Continue one agent’s work with another agent
- Compete mode with multiple agents
- Sequential pipeline mode
- Project auto-detection for test/build commands
- Secret scanning before launch
- Local web dashboard
- MCP server integration
- Container image builder for sketch agents
- Resource usage tracking
- Cost/token tracking where available

---

## 14. Resume Bullet Target

After MVP:

> Built AgentBox, a C++ CLI for A/B testing AI coding agents using disposable git worktrees, structured task metadata, log capture, automated test execution, diff comparison, and accept/reject patch workflows.

After container/sandbox mode:

> Added containerized execution mode for untrusted agents with isolated workspaces, fake home directories, stripped environment variables, optional network disablement, and resource limits.

---

## 15. Guiding Principle

Do not try to build a smarter coding agent.

Build the missing systems layer around agents:

```text
workspace isolation
repeatable task setup
multi-agent comparison
logging/auditability
diff/test review
safe accept/reject workflow
```

The first useful version should make this painful manual workflow:

```text
git worktree add ...
run codex ...
run sketch agent ...
run tests ...
git diff ...
apply patch ...
clean branches ...
```

feel like this:

```bash
agentbox compare --agents codex,sketch --task "Fix failing parser test" --test "make test"
agentbox accept task-001 --agent codex
agentbox clean task-001
```

