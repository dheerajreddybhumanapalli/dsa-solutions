# Testcase Format & Parsing Guide

This guide explains how `testcases.txt` must be structured so that the `runner.h` harness parses input arguments and expected outputs properly.

---

## 1. File Structure

Each `testcases.txt` file adheres to an alternating line structure:

```text
<N>
<arguments_for_case_1>
<expected_output_for_case_1>
<arguments_for_case_2>
<expected_output_for_case_2>
...
<arguments_for_case_N>
<expected_output_for_case_N>
```

- **Line 1 (`N`)**: Total number of test cases (integer).
- **Even Lines**: Input arguments for the test case. If the method takes multiple arguments, separate them with commas.
- **Odd Lines**: The expected return value.

> [!IMPORTANT]
> The test harness (`runner.h`) expects exactly two non-empty lines per test case. If you omit an expected output line, subsequent lines will become misaligned and tests will fail. Do not add `#` or `//` comment lines in `testcases.txt`.

---

## 2. Supported Data Types & Syntax

| Type in C++ | `testcases.txt` Input Format | Example |
| :--- | :--- | :--- |
| `int` / `long long` | Plain number | `42`, `-100` |
| `string` | Quoted or unquoted string | `"abc"`, `hello` |
| `bool` | `true`, `false`, `1`, `0` | `true`, `false` |
| `vector<T>` | Bracketed comma-separated items | `[1, 2, 3]`, `["cat", "dog"]` |

---

## 3. Concrete Examples

### Example 1: Single String Argument returning Integer
From [LeetCode 3498](file:///Users/dheerajbhumanapalli/practice/leetcode/3498/testcases.txt):
```text
2
"abc"
148
"zaza"
160
```
- Case 1 input: `"abc"`, expected: `148`
- Case 2 input: `"zaza"`, expected: `160`

### Example 2: Multiple Arguments
From [LeetCode 1401](file:///Users/dheerajbhumanapalli/practice/leetcode/1401/testcases.txt) (`checkOverlap(radius, xCenter, yCenter, x1, y1, x2, y2)`):
```text
3
1, 0, 0, 1, -1, 3, 1
true
1, 1, 1, 1, -3, 2, -1
false
1, 0, 0, -1, 0, 0, 1
true
```
- The 7 arguments are separated by commas on the input line.

### Example 3: Returning a `vector<string>`
From [LeetCode 1520](file:///Users/dheerajbhumanapalli/practice/leetcode/1520/testcases.txt):
```text
2
"adefaddaccc"
["e","f","ccc"]
"abbaccd"
["d","bb","cc"]
```
- Output vectors are matched order-independently (sorted before comparison).

---

## 4. Troubleshooting Common Issues

| Symptom | Cause | Solution |
| :--- | :--- | :--- |
| `Case X ERROR: argument count mismatch` | Comma separation in input line does not match method arity. | Check the number of arguments in the input line. |
| `Case 1: FAIL` with empty / misaligned comparison | Omitted expected output in `testcases.txt`. | Ensure every input line is followed by an expected output line. |
| `stoi: no conversion` | Line 1 is empty or contains non-numeric characters. | Ensure the very first line contains only the integer count $N$. |
