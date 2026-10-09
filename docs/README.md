# Repository Documentation

Welcome to the internal documentation for the **DSA Solutions** practice repository. This guide explains how the testing framework, build pipeline, and problem organization work under the hood.

---

## Documentation Index

- **[System Architecture](architecture.md)**
  Deep dive into how `runner.h` / `runner.py`, `init.cpp` / `init.py`, and `solution.cpp` / `solution.py` connect, how the `Makefile` builds and runs them, and how GitHub Actions CI tests changes.
- **[Testcase Format & Parsing](testcase-format.md)**
  Detailed specification of `testcases.txt`, supported input/output data types, argument tokenization rules, and parsing gotchas.
- **[Workflow Guide](workflow-guide.md)**
  Step-by-step instructions on setting up a new problem, configuring harnesses, testing locally, and managing solution versions.

---

## Quick Mental Model

```
                    ┌─────────────────────────┐
                    │   templates/            │  (Starter files)
                    │   - init.cpp / init.py  │
                    │   - solution.cpp / .py  │
                    │   - testcases.txt       │
                    └───────────┬─────────────┘
                                │ copy to create problem
                                ▼
┌───────────────────────┐   ┌───────────────────────────┐
│ runner.h              │   │ leetcode/<id>/            │
│ - ValueParser<T>      ├──►│ - init.cpp / init.py      │ (Harness entry)
│ - executeHarness      │   │ - solution.cpp / .py      │ (Your LeetCode class)
│ - main() entry        │   │ - testcases.txt           │ (Input & Expected)
│ runner.py             │   └─────────────┬─────────────┘
│ - parse_value         │                 │
│ - run_tests / main    │     ┌───────────┴───────────┐
└───────────────────────┘     ▼ (make leetcode/<id>)  ▼ (make py-leetcode/<id>)
                    ┌───────────────────────┐  ┌───────────────────────────┐
                    │ build/leetcode/<id>/  │  │ python3 init.py           │
                    │ app (compiled binary) │  │ testcases.txt (Python)    │
                    └───────────────────────┘  └───────────────────────────┘
```
