# DSA Solutions — C++

Practice repo for Data Structures & Algorithms: every problem keeps **all versions tried**
(brute force → optimal) with a short write-up comparing them. Built for signal, not volume.

## Structure

```text
.
├── <platform>/                        # e.g., leetcode/, codeforces/
│   ├── README.md                          # progress tracker table
│   └── <id>-<slug>/                       # one folder per problem
│       ├── README.md                      # summary + versions + complexity
│       ├── init.cpp                       # test harness entry point
│       ├── testcases.txt                  # test cases
│       ├── solution.cpp                   # optimal / latest accepted
│       └── solution_v1_bruteforce.cpp     # earlier attempts (v1, v2, ...)
├── docs/                                  # detailed guides & architecture docs
│   ├── README.md                          # docs index
│   ├── architecture.md                    # test runner & build internals
│   ├── testcase-format.md                 # testcases.txt parsing rules
│   └── workflow-guide.md                  # adding problems step-by-step
├── templates/
│   ├── init.cpp                           # starter for test harness
│   ├── testcases.txt                      # blank test cases
│   ├── solution.cpp                       # starter for new C++ solutions
│   └── README_problem.md                  # starter for new problem write-ups
├── .github/workflows/cpp.yml              # compiles and runs testcases on push
├── Makefile                               # `make <platform>/<id>` / `make all`
├── runner.h                               # central C++ test runner engine
├── build/                                 # local binaries only (git-ignored)
└── README.md
```

## Documentation

Comprehensive guides are available in the [`docs/`](docs/) directory:
- [System Architecture](docs/architecture.md): Explains `runner.h`, `init.cpp`, dynamic Makefile targets, and CI workflow.
- [Testcase Format & Parsing](docs/testcase-format.md): Specification for `testcases.txt` inputs, outputs, and supported types.
- [Developer Workflow Guide](docs/workflow-guide.md): Step-by-step instructions for adding problems and tracking versions.

**Naming rules**

- Folder: `<platform-id>-<kebab-slug>`, e.g. `1520-maximum-number-of-non-overlapping-substrings`.
- Files: `solution.cpp` (optimal), `solution_v1_*.cpp`, `solution_v2_*.cpp` for history.
- Every folder **must** have a `README.md` (copy from `templates/README_problem.md`).

## Coding practices

- C++17, `-Wall -Wextra -O2`, no `using namespace std;` in new code.
- Compiled binaries go to `build/` (git-ignored). Never commit binaries or `.dSYM`.
- Test harness (`init.cpp`) automatically parses `testcases.txt` and verifies the solution.
- Each version documents time / space complexity and verdict in its problem README.

## Quick start

```bash
# build one problem
make leetcode/1520

# run the testcases for one problem
cd leetcode/1520 && ../../build/leetcode/1520/app

# build and run all problems
make all
```

## Adding a new problem

```bash
cp -r templates/ <platform>/XXXX-slug  # copy all templates
# 1. paste optimal code into solution.cpp
# 2. configure the runner in init.cpp and paste test cases into testcases.txt
# 3. keep attempts as solution_v1_*.cpp
# 4. fill templates/README_problem.md -> <platform>/XXXX-slug/README.md
# 5. add a row to <platform>/README.md
make <platform>/XXXX-slug
cd <platform>/XXXX-slug && ../../build/<platform>/XXXX-slug/app
```
