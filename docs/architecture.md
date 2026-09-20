# System Architecture

This document describes how code is organized, compiled, and executed across this repository.

---

## 1. High-Level Design

The repository is structured so that **problem solutions remain 100% clean and identical to LeetCode submissions**, with zero boilerplate or testing clutter in `solution.cpp`.

Each problem directory contains four primary files:
- **`solution.cpp`**: Contains the raw `Solution` class with the target method.
- **`init.cpp`**: A lightweight glue file that hooks `solution.cpp` into the generic test runner.
- **`testcases.txt`**: Raw test cases (number of cases, input arguments, and expected outputs).
- **`README.md`**: Problem metadata, time/space complexity, and version history.

---

## 2. The Test Harness Engine (`runner.h`)

[`runner.h`](file:///Users/dheerajbhumanapalli/practice/runner.h) is a header-only, C++17 generic test harness located in the repository root. It handles reading input files, parsing tokens into typed C++ values, invoking the solution method, and verifying equality.

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

[`init.cpp`](file:///Users/dheerajbhumanapalli/practice/templates/init.cpp) defines the function `bool runTests(istream &in)` declared by `runner.h`.

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

The [`Makefile`](file:///Users/dheerajbhumanapalli/practice/Makefile) dynamically discovers problems and builds standalone binaries.

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
$(BUILD_DIR)/%/app: %/init.cpp %/solution.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I. -I$* $< -o $@
	@echo "Built $@"
```
- `-I.` allows `init.cpp` to locate `runner.h` in the workspace root.
- `-I$*` allows `init.cpp` to resolve `#include "solution.cpp"` from within its problem directory.
- Output binary is created at `build/<dir>/app`.

### C. Execution Targets
- `make leetcode/1401`: Compiles the binary `build/leetcode/1401/app`.
- `make all`: Compiles all discovered problem binaries and immediately runs each against its test cases in sequence:
  ```bash
  (cd "$$d" && "$(ROOT_DIR)/$(BUILD_DIR)/$$d/app") || exit 1
  ```

---

## 5. Continuous Integration (`.github/workflows/cpp.yml`)

The repository runs an automated CI pipeline on push and pull requests:

1. **Change Detection**: Identifies modified `solution.cpp` files using `git diff`:
   - For pull requests: compares against `origin/${{ github.base_ref }}`.
   - For pushes: compares `${{ github.event.before }}` to `${{ github.sha }}`.
2. **Selective Build**: Only runs `make "$folder"` for directories containing modified solutions.
3. **Execution**: Runs `./build/$folder/app "$folder/testcases.txt"` to ensure all test cases pass before PRs can be merged.
