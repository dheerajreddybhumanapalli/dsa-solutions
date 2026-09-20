# Repository Documentation

Welcome to the internal documentation for the **DSA Solutions** practice repository. This guide explains how the testing framework, build pipeline, and problem organization work under the hood.

---

## Documentation Index

- **[System Architecture](architecture.md)**
  Deep dive into how `runner.h`, `init.cpp`, and `solution.cpp` connect, how the `Makefile` builds binaries, and how GitHub Actions CI tests changes.
- **[Testcase Format & Parsing](testcase-format.md)**
  Detailed specification of `testcases.txt`, supported input/output data types, argument tokenization rules, and parsing gotchas.
- **[Workflow Guide](workflow-guide.md)**
  Step-by-step instructions on setting up a new problem, configuring harnesses, testing locally, and managing solution versions.

---

## Quick Mental Model

```
                    ┌─────────────────────────┐
                    │   templates/            │  (Starter files)
                    │   - init.cpp            │
                    │   - solution.cpp        │
                    │   - testcases.txt       │
                    └───────────┬─────────────┘
                                │ copy to create problem
                                ▼
┌──────────────────────┐   ┌───────────────────────────┐
│ runner.h             │   │ leetcode/<id>/            │
│ - ValueParser<T>     ├──►│ - init.cpp                │ (Harness entry)
│ - parseArgsTuple     │   │ - solution.cpp            │ (Your LeetCode class)
│ - executeHarness     │   │ - testcases.txt           │ (Input & Expected)
│ - main() entry       │   └─────────────┬─────────────┘
└──────────────────────┘                 │
                                         ▼ (make leetcode/<id>)
                           ┌───────────────────────────┐
                           │ build/leetcode/<id>/app   │ (Single compiled binary)
                           └───────────────────────────┘
```
