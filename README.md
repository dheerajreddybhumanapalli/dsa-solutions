# DSA Solutions — C++ & Python

Practice repo for Data Structures & Algorithms: every problem keeps **all versions tried**
(brute force → optimal) with a short write-up comparing them. Built for signal, not volume.

## Structure

```text
.
├── leetcode/
│   ├── README.md                          # progress tracker table
│   └── <id>-<slug>/                       # one folder per problem
│       ├── README.md                      # summary + versions + complexity
│       ├── solution.cpp                   # optimal / latest accepted
│       └── solution_v1_bruteforce.cpp     # earlier attempts (v1, v2, ...)
├── templates/
│   ├── solution.cpp                       # starter for new C++ solutions
│   └── README_problem.md                  # starter for new problem write-ups
├── .github/workflows/ci.yml              # compiles every solution.cpp on push
├── Makefile                               # `make <id>` / `make all`
├── build/                                 # local binaries only (git-ignored)
└── README.md
```

**Naming rules**

- Folder: `<leetcode-id>-<kebab-slug>`, e.g. `1520-maximum-number-of-non-overlapping-substrings`.
- Files: `solution.cpp` (optimal), `solution_v1_*.cpp`, `solution_v2_*.cpp` for history.
- Every folder **must** have a `README.md` (copy from `templates/README_problem.md`).

## Coding practices

- C++17, `-Wall -Wextra -O2`, no `using namespace std;` in new code.
- Compiled binaries go to `build/` (git-ignored). Never commit binaries, `.dSYM`, `__pycache__`.
- Optimal solution guards its demo `main()` with `#ifndef LEETCODE_JUDGE` so it pastes cleanly into LeetCode.
- Each version documents time / space complexity and verdict in its problem README.

## Quick start

```bash
# build + run one problem (uses folder prefix match)
make 1520
./build/1520

# build everything
make all

# manual compile
clang++ -std=c++17 -Wall -Wextra -O2 \
  leetcode/1520-maximum-number-of-non-overlapping-substrings/solution.cpp \
  -o build/1520 && ./build/1520
```

## Adding a new problem

```bash
cp -r templates/new-problem leetcode/XXXX-slug  # or mkdir manually
# 1. paste optimal code into solution.cpp
# 2. keep attempts as solution_v1_*.cpp
# 3. fill templates/README_problem.md -> leetcode/XXXX-slug/README.md
# 4. add a row to leetcode/README.md
make XXXX && ./build/XXXX
```
