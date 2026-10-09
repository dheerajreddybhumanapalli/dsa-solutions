# System Architecture

This document describes how code is organized, compiled, and executed across this repository.

---

## 1. High-Level Design

The repository is structured so that **problem solutions remain 100% clean and identical to LeetCode submissions**, with zero boilerplate or testing clutter in `solution.cpp` / `solution.py`.

A problem can be solved in either or both languages. Each problem directory contains:
- **`solution.cpp` / `solution.py`**: The raw `Solution` class with the target method.
- **`init.cpp` / `init.py`**: A lightweight glue file that hooks the solution into the generic test runner for its language.
- **`testcases.txt`**: Raw test cases (number of cases, input arguments, and expected outputs) — shared by both harnesses.
- **`README.md`**: Problem metadata, time/space complexity, and version history.

---

## 2. The Test Harness Engine (`runner.h`)

[`runner.h`](../runner.h) is a header-only, C++17 generic test harness located in the repository root. It handles reading input files, parsing tokens into typed C++ values, invoking the solution method, and verifying equality.

### Key Components

#### A. Type Parsers (`ValueParser<T>`)
Specialized template structs parse string tokens into C++ types:
- `ValueParser<int>`: via `stoi`
- `ValueParser<long long>`: via `stoll`
- `ValueParser<string>`: strips surrounding double quotes (`"..."`) and trims whitespace
- `ValueParser<bool>`: supports `"true"`, `"false"`, `"1"`, `"0"`
- `ValueParser<vector<T>>`: parses bracketed comma-separated arrays like `[1, 2, 3]` or `["a", "b"]`, recursively delegating to `ValueParser<T>`

#### B. Variadic Tuple Unpacking (`parseArgsTuple`)
Using C++17 `index_sequence`, `parseArgsTuple` unpacks string tokens into a typed `std::tuple<Args...>`:
```cpp
template <typename Tuple, size_t... Is>
Tuple parseArgsTuple(const vector<string> &tokens, index_sequence<Is...>) {
    return make_tuple(parseValue<decay_t<tuple_element_t<Is, Tuple>>>(tokens[Is])...);
}
```
This enables solutions with any number or combination of parameters (e.g. `(int, string, vector<int>)`) to be automatically parsed and forwarded to the solver.

#### C. Verification & Order-Independent Matching (`areEqual`)
- Primitive types are compared using standard equality (`==`).
- `vector<T>` comparisons automatically sort both actual and expected outputs prior to comparison:
  ```cpp
  template <typename T>
  bool areEqual(vector<T> got, vector<T> expected) {
      if (got.size() != expected.size()) return false;
      sort(got.begin(), got.end());
      sort(expected.begin(), expected.end());
      return got == expected;
  }
  ```
  This cleanly handles problems where returning results in any order is acceptable (e.g. subsets, intervals, combinations).

#### D. Central Entry Point (`main`)
`runner.h` provides `int main(int argc, char **argv)`:
```cpp
int main(int argc, char **argv) {
    string casesPath = (argc >= 2) ? argv[1] : "testcases.txt";
    ifstream in(casesPath);
    if (!in) {
        cerr << "Failed to open testcases file: " << casesPath << "\n";
        return 1;
    }
    return runTests(in) ? 0 : 1;
}
```
- If executed from inside the problem folder, it defaults to `"testcases.txt"`.
- If executed from the repo root or CI, it accepts the path as an argument: `./build/<dir>/app <dir>/testcases.txt`.

---

## 3. The Problem Adapter (`init.cpp`)

[`init.cpp`](../templates/init.cpp) defines the function `bool runTests(istream &in)` declared by `runner.h`.

It connects `runner.h`'s `runAuto` template with the problem's `Solution` method via a stateless lambda decaying into a function pointer (`+[]`):

```cpp
#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](string s) -> int {
        Solution sol;
        return sol.reverseDegree(s);
    });
}
```

Because the lambda has no captures, the unary `+` operator decays the lambda into a plain function pointer `Ret (*)(Args...)`, allowing the compiler to deduce all argument types `Args...` and return type `Ret` automatically.

---

## 4. Build System (`Makefile`)

The [`Makefile`](../Makefile) dynamically discovers problems and builds standalone binaries.

### A. Dynamic Problem Discovery
```makefile
INIT_SRCS := $(shell find . -mindepth 2 -type f -name init.cpp \
               -not -path "*/.*" \
               -not -path "./$(BUILD_DIR)/*" \
               -not -path "./templates/*" \
               -not -path "./docs/*")
DIRS      := $(sort $(patsubst %/init.cpp,%,$(patsubst ./%,%,$(INIT_SRCS))))
```
Any folder containing `init.cpp` (except hidden directories, `build/`, `templates/`, and `docs/`) is automatically registered as a target.

### B. Compilation Rule
```makefile
$(BUILD_DIR)/%/app: %/init.cpp %/solution.cpp runner.h Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I. -I$* $< -o $@
	@echo "Built $@"
```
- `-I.` allows `init.cpp` to locate `runner.h` in the workspace root.
- `-I$*` allows `init.cpp` to resolve `#include "solution.cpp"` from within its problem directory.
- Output binary is created at `build/<dir>/app`.

### C. Solution Source Convention
Each `solution.cpp` is self-contained and starts with:
```cpp
#include <bits/stdc++.h>

using namespace std;
```
Because the file declares its own includes, it compiles identically in the local Makefile build and in CI, with no build-level prelude needed.

### D. Execution Targets
- `make leetcode/1401`: Compiles the binary `build/leetcode/1401/app`.
- `make leetcode/1401/run`: Builds the binary if stale, then runs that problem's suite from inside its own directory.
- `make leetcode/1401/rebuild`: Removes the existing binary and recompiles it, so `-Wall -Wextra` warnings are shown again.
- `make all`: Compiles all discovered problem binaries and immediately runs each against its test cases in sequence:
  ```bash
  (cd "$$d" && "$(ROOT_DIR)/$(BUILD_DIR)/$$d/app") || exit 1
  ```

---

## 5. Python Support (`runner.py` + `init.py`)

[`runner.py`](../runner.py) is the Python counterpart of `runner.h`, reproducing the same contract: read `N`, read two lines per case, tokenize the args line (the whole line for a single `string` argument), coerce tokens by declared type, invoke the method, compare, and print `Case i: PASS` / `Case i: FAIL (Got: ..., Expected: ...)` followed by `<passed>/<n> passed`.

### A. Type Specifiers
Because Python has no static signature to deduce, `init.py` declares the argument and return types using the same type names as `init.cpp`:

```python
import sys

from runner import main, run_tests
from solution import Solution


def run_tests_hook(stream):
    return run_tests(stream, Solution(), "isValid", arg_types=["string"], return_type="bool")


if __name__ == "__main__":
    sys.exit(main(run_tests_hook))
```

Supported specifiers: `int`, `long`, `float`, `double`, `bool`, `char`, `string`, `vector<T>` (nesting supported), `ListNode`, `TreeNode`, and `void` (return only — the first argument is mutated in place and compared to the expected line).

### B. Comparison
Identical semantics to the C++ harness: floats within `1e-5`, vectors matched exactly first with a sorted (order-insensitive) fallback, and structural `ListNode` / `TreeNode` equality.

### C. Entry Point (`main`)
`runner.main(run_hook)` mirrors `main` in `runner.h`: it reads the testcases path from `argv` (default `testcases.txt`), opens it, and returns `0` iff every case passed.

### D. Running
The Python harness needs the repo root on the import path (the counterpart of C++'s `-I.`):

```bash
# via the Makefile (sets PYTHONPATH automatically)
make py-leetcode/20

# manually
PYTHONPATH=. python3 leetcode/20/init.py leetcode/20/testcases.txt
```

### E. Makefile Targets
- `make py-<dir>` (e.g. `make py-leetcode/20`): runs `<dir>/init.py` against `<dir>/testcases.txt`.
- `make py-all`: runs every discovered `init.py`.

---

## 6. Continuous Integration (`.github/workflows/{cpp,python}.yml`)

The repository runs an automated CI pipeline on pushes to `main` and pull requests targeting `main`, one workflow per language:

1. **Change Detection**: Identifies modified source files using `git diff` (`solution.cpp` for `cpp.yml`, `solution.py` for `python.yml`):
   - For pull requests: compares against `origin/${{ github.base_ref }}`.
   - For pushes: compares `${{ github.event.before }}` to `${{ github.sha }}`.
2. **Selective Build / Run**: `cpp.yml` runs `make "$folder"` for each changed directory, then executes `./build/$folder/app "$folder/testcases.txt"`. `python.yml` runs `make "py-$folder"`, which executes the Python harness against the same testcases file.
3. Both fail the job if any test case fails (the harness exits non-zero), so PRs cannot merge with a broken solution.
