---
name: workspace-instructions-bootstrap
description: 'Bootstrap or refresh workspace instruction files (copilot-instructions.md or AGENTS.md). Use for discovering conventions, extracting architecture/build/test commands, merging existing instructions, and producing concise, actionable agent guidance.'
argument-hint: 'Use for: bootstrap from scratch, refresh outdated instructions, or align instructions to current repo practices.'
user-invocable: true
---

# Workspace Instructions Bootstrap

## What This Skill Produces
- A new or updated workspace-wide instruction file for AI coding agents.
- Preferred target file: `.github/copilot-instructions.md`.
- Alternate target only when needed: `AGENTS.md` (root or monorepo subtree).

## When To Use
- A repo has no usable agent instructions.
- Existing instructions are stale, duplicated, or too vague.
- Build/test commands changed and agents need current commands.
- You want standardized, high-signal onboarding guidance for agents.

## Procedure
1. Discover existing conventions and instruction sources.
   - Search:
     - `**/{.github/copilot-instructions.md,AGENT.md,AGENTS.md,CLAUDE.md,.cursorrules,.windsurfrules,.clinerules,.cursor/rules/**,.windsurf/rules/**,.clinerules/**,README.md}`
   - Decide one workspace instruction type to maintain:
     - Keep `copilot-instructions.md` for project-wide defaults.
     - Use `AGENTS.md` hierarchy only if different subtrees need different defaults.

2. Explore repository facts that must be in instructions.
   - Gather exact, runnable build/test/lint/analysis commands.
   - Map architecture layers and boundaries.
   - Capture project-specific conventions that differ from common defaults.
   - Capture constraints/pitfalls (OS/runtime/toolchain specifics).
   - Identify 5-10 high-value reference files.
   - Use read-only subagents for broad exploration when repo is large.

3. Generate or merge instructions.
   - If file exists: preserve useful guidance, remove duplication, update stale sections.
   - If file does not exist: create a concise file with only relevant sections.
   - Keep sections practical and short; avoid copying README wholesale.

4. Apply quality checks before finishing.
   - Commands are executable in the current environment (for this repo: Windows via WSL for shell scripts).
   - Architecture and boundaries are explicit.
   - Constraints and non-default conventions are present.
   - File stays concise and actionable.
   - No conflicting instruction file type pair (`copilot-instructions.md` plus `AGENTS.md`) unless intentionally designed.

5. Iterate with user feedback.
   - Ask for any missing team rules (commit style, review gates, release rules).
   - If repo is multi-domain, propose scoped `*.instructions.md` with `applyTo` for focused areas.

## Decision Points
- Existing both `copilot-instructions.md` and `AGENTS.md`:
  - Default to one source of truth, then migrate content.
- Monorepo with diverging practices:
  - Prefer nearest `AGENTS.md` hierarchy for subtrees.
- Single-repo with mostly shared behavior:
  - Prefer `.github/copilot-instructions.md`.

## Completion Criteria
- Instruction file is present and readable.
- It includes:
  - architecture boundaries,
  - build/test commands,
  - conventions and constraints,
  - high-value references.
- Content is concise and free from duplicated boilerplate.
- User has clear example prompts to validate behavior.

## Output Checklist
- Updated instruction file path.
- Short summary of what changed.
- 3-5 example prompts to try.
- 2-4 next customization suggestions (`/create-instruction`, `/create-agent`, `/create-prompt`, `/create-skill`).
