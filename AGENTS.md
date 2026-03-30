# Branch Policy

This branch, `codex-acgc-style-port-process`, is a fast-port branch.

The goal here is to get the Twilight Princess PC port working as quickly as
possible. We are not optimizing for minimal diffs, upstream cleanliness, or
strict separation between decomp code and port code.

## Rules For This Branch

- Prefer shipping progress over preserving architecture.
- Compile broadly by default and use a short exclusion list for blocker subsystems.
- Make invasive port-side or game-side changes when they unblock the native port.
- Replace GC/Wii runtime assumptions aggressively when they block host builds.
- Do not protect brittle abstractions just because they match the original layout.
- Treat this branch as a practical fork focused on getting the port made.

## Non-Goals

- Keeping the port layer thin at all costs.
- Avoiding `#ifdef` or targeted source edits in game code.
- Preserving perfect compatibility with upstream porting strategy.

If a choice exists between a cleaner long-term abstraction and a faster path to
a running port, choose the faster path in this branch.
