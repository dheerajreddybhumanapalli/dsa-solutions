# Developer Workflow Guide

This guide walks through everyday development tasks: adding new problems, running tests, and keeping solution histories.

---

## 1. Everyday Commands

```bash
# Build and execute all problems across the repository
make all

# Build a single problem (e.g. leetcode/1401)
make leetcode/1401

# Execute a specific problem binary
./build/leetcode/1401/app leetcode/1401/testcases.txt
# OR
cd leetcode/1401 && ../../build/leetcode/1401/app

# Clean all generated build artifacts
make clean
```

---

## 2. Adding a New Problem (Step-by-Step)

### Step 1: Copy Templates
Copy the starter files to your target platform and problem directory:
```bash
cp -r templates leetcode/<id>
```

### Step 2: Implement Solution in `solution.cpp`
Paste or write your LeetCode class in `leetcode/<id>/solution.cpp`:
```cpp
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    int myMethod(string s, int k) {
        // Solution implementation
        return 0;
    }
};
```

### Step 3: Configure the Test Runner in `init.cpp`
Update the lambda arguments and return type in `leetcode/<id>/init.cpp` to match `myMethod`:
```cpp
#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](string s, int k) -> int {
        Solution sol;
        return sol.myMethod(s, k);
    });
}
```

### Step 4: Populate `testcases.txt`
Specify total cases on line 1, followed by alternating input arguments and expected outputs:
```text
2
"example", 3
5
"test", 1
2
```

### Step 5: Document the Problem
Fill out `leetcode/<id>/README.md` (copied from `templates/README_problem.md`) and add a row to the tracker table in `leetcode/README.md`.

### Step 6: Verify Locally
Build and test your solution:
```bash
make leetcode/<id>
./build/leetcode/<id>/app leetcode/<id>/testcases.txt
```

---

## 3. Managing Multiple Versions

When working on a problem, keep your attempts for comparison:
1. Save earlier attempts as `solution_v1_bruteforce.cpp`, `solution_v2_memoization.cpp`, etc.
2. Keep the latest optimal/accepted code in `solution.cpp` (since `init.cpp` includes `solution.cpp`).
3. Document time/space complexity and verdict comparison in the problem's `README.md`.
