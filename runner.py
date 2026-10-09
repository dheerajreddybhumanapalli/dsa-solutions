"""Python test harness — the counterpart of runner.h.

Mirrors the C++ harness (runner.h) so a problem folder can carry both a
``init.cpp`` and an ``init.py`` against the same ``testcases.txt``:

  * reads ``N`` (case count) then alternating args / expected lines
  * tokenizes the args line on top-level commas (the whole line is one token
    when the only argument is a ``string``)
  * coerces each token to the declared argument type
  * invokes ``Solution.<method>(*args)``
  * compares the result (float tolerance 1e-5, order-insensitive vectors)
  * prints ``Case i: PASS`` / ``Case i: FAIL (Got: ..., Expected: ...)`` and a
    final ``<passed>/<n> passed``
  * returns True iff every case passed (the entry point exits 0 on True)

Type specifiers (strings, mirroring the C++ types used in ``init.cpp``):
    int, long, long long, float, double, bool, char, string
    vector<int>, vector<string>, vector<vector<int>>, vector<vector<char>>, ...
    ListNode, TreeNode            (built from the array/testcase syntax)
    void                          (return_type only — in-place / mutate first arg)
"""

from __future__ import annotations

import json
import sys
from collections import deque
from typing import Any, Callable, List, Optional, TextIO


# ---------------------------------------------------------------------------
# Standard LeetCode data structures
# ---------------------------------------------------------------------------

class ListNode:
    def __init__(self, val: int = 0, next: Optional["ListNode"] = None) -> None:
        self.val = val
        self.next = next


class TreeNode:
    def __init__(
        self,
        val: int = 0,
        left: Optional["TreeNode"] = None,
        right: Optional["TreeNode"] = None,
    ) -> None:
        self.val = val
        self.left = left
        self.right = right


def _build_list(values: List[Any]) -> Optional[ListNode]:
    dummy = ListNode()
    cur = dummy
    for v in values:
        cur.next = ListNode(int(v))
        cur = cur.next
    return dummy.next


def _build_tree(values: List[Any]) -> Optional[TreeNode]:
    if not values or values[0] is None:
        return None
    root = TreeNode(int(values[0]))
    q: deque = deque([root])
    i = 1
    while q and i < len(values):
        node = q.popleft()
        if i < len(values):
            v = values[i]
            i += 1
            if v is not None:
                node.left = TreeNode(int(v))
                q.append(node.left)
        if i < len(values):
            v = values[i]
            i += 1
            if v is not None:
                node.right = TreeNode(int(v))
                q.append(node.right)
    return root


# ---------------------------------------------------------------------------
# Tokenizer (mirrors splitTokens in runner.h)
# ---------------------------------------------------------------------------

def _split_top_level(line: str) -> List[str]:
    tokens: List[str] = []
    current: List[str] = []
    depth = 0
    in_quotes = False
    for i, c in enumerate(line):
        if c == '"' and (i == 0 or line[i - 1] != "\\"):
            in_quotes = not in_quotes
            current.append(c)
        elif not in_quotes and c in "[{(":
            depth += 1
            current.append(c)
        elif not in_quotes and c in "]})":
            depth -= 1
            current.append(c)
        elif c == "," and depth == 0 and not in_quotes:
            tokens.append("".join(current).strip())
            current = []
        else:
            current.append(c)
    tail = "".join(current).strip()
    if tail:
        tokens.append(tail)
    return tokens


def _strip_brackets(text: str) -> str:
    t = text.strip()
    if t.startswith("["):
        t = t[1:]
    if t.endswith("]"):
        t = t[:-1]
    return t


def _parse_string(raw: str) -> str:
    t = raw.strip()
    if len(t) >= 2 and t[0] == '"' and t[-1] == '"':
        try:
            return json.loads(t)
        except ValueError:
            return t[1:-1]
    return t


def _parse_char(raw: str) -> str:
    t = raw.strip()
    if len(t) >= 2 and t[0] in "'\"" and t[-1] == t[0]:
        body = t[1:-1]
        return body[0] if body else ""
    return t[0] if t else ""


def _parse_literal(raw: str) -> Any:
    t = raw.strip()
    if t == "" or t.lower() in ("null", "none"):
        return None
    try:
        return json.loads(t)
    except ValueError:
        pass
    if len(t) >= 2 and t[0] == t[-1] and t[0] in "\"'":
        return t[1:-1]
    return t


def _norm_spec(spec: Any) -> str:
    return str(spec).strip().lower()


def _inner_spec(spec: str) -> str:
    return spec[spec.index("<") + 1:-1]


def _parse_value(raw: str, spec: Any) -> Any:
    spec = _norm_spec(spec)
    if (spec.startswith("vector<") or spec.startswith("list<")) and spec.endswith(">"):
        inner = _inner_spec(spec)
        return [_parse_value(p, inner) for p in _split_top_level(_strip_brackets(raw)) if p.strip() != ""]
    if spec == "string":
        return _parse_string(raw)
    if spec == "char":
        return _parse_char(raw)
    if spec == "bool":
        return raw.strip().lower() in ("true", "1")
    if spec in ("int", "long", "long long", "unsigned", "unsigned int", "unsigned long long"):
        return int(raw.strip())
    if spec in ("float", "double"):
        return float(raw.strip())
    if spec == "listnode":
        return _build_list(_parse_value(raw, "vector<int>"))
    if spec == "treenode":
        vals = [None if p.strip().lower() in ("null", "none", "") else int(p.strip())
                for p in _split_top_level(_strip_brackets(raw))]
        return _build_tree(vals)
    return _parse_literal(raw)


# ---------------------------------------------------------------------------
# Equality comparison (mirrors areEqual / areEqual(vector) in runner.h)
# ---------------------------------------------------------------------------

def _list_equal(a: Optional[ListNode], b: Optional[ListNode]) -> bool:
    while a and b:
        if not _are_equal(a.val, b.val):
            return False
        a = a.next
        b = b.next
    return a is None and b is None


def _tree_equal(a: Optional[TreeNode], b: Optional[TreeNode]) -> bool:
    if a is None and b is None:
        return True
    if a is None or b is None:
        return False
    return a.val == b.val and _tree_equal(a.left, b.left) and _tree_equal(a.right, b.right)


def _seq_equal(a: list, b: list) -> bool:
    if len(a) != len(b):
        return False
    return all(_are_equal(x, y) for x, y in zip(a, b))


def _are_equal(got: Any, expected: Any) -> bool:
    if isinstance(got, bool) or isinstance(expected, bool):
        return got == expected
    if isinstance(got, float) or isinstance(expected, float):
        try:
            return abs(got - expected) <= 1e-5
        except TypeError:
            return got == expected
    if isinstance(got, ListNode) or isinstance(expected, ListNode):
        return _list_equal(got, expected)
    if isinstance(got, TreeNode) or isinstance(expected, TreeNode):
        return _tree_equal(got, expected)
    if isinstance(got, list) and isinstance(expected, list):
        if _seq_equal(got, expected):
            return True
        try:
            return _seq_equal(sorted(got), sorted(expected))
        except TypeError:
            return False
    return got == expected


# ---------------------------------------------------------------------------
# Pretty printing (for failure diagnostics)
# ---------------------------------------------------------------------------

def _list_to_str(head: Optional[ListNode]) -> str:
    vals: List[str] = []
    while head:
        vals.append(str(head.val))
        head = head.next
    return "[" + ", ".join(vals) + "]"


def _tree_to_str(root: Optional[TreeNode]) -> str:
    if root is None:
        return "[]"
    out: List[str] = []
    q: deque = deque([root])
    while q:
        node = q.popleft()
        if node is None:
            out.append("null")
        else:
            out.append(str(node.val))
            q.append(node.left)
            q.append(node.right)
    while out and out[-1] == "null":
        out.pop()
    return "[" + ", ".join(out) + "]"


def _to_str(value: Any) -> str:
    if isinstance(value, bool):
        return "true" if value else "false"
    if value is None:
        return "null"
    if isinstance(value, str):
        return '"' + value + '"'
    if isinstance(value, ListNode):
        return _list_to_str(value)
    if isinstance(value, TreeNode):
        return _tree_to_str(value)
    if isinstance(value, list):
        return "[" + ", ".join(_to_str(v) for v in value) + "]"
    return str(value)


# ---------------------------------------------------------------------------
# Test harness core
# ---------------------------------------------------------------------------

def _next_line(stream: TextIO) -> Optional[str]:
    for raw in stream:
        line = raw.rstrip("\r\n")
        if line.strip() != "":
            return line
    return None


def _is_void(return_type: Any) -> bool:
    return _norm_spec(return_type) in ("void", "none", "null")


def run_tests(
    stream: TextIO,
    solution: Any,
    method_name: str,
    arg_types: List[Any],
    return_type: Any = "void",
) -> bool:
    """Run every case in ``stream`` against ``solution.<method_name>``."""
    specs = [_norm_spec(s) for s in arg_types]
    method: Callable = getattr(solution, method_name)

    first = _next_line(stream)
    if first is None:
        return False
    n = int(first.strip())
    passed = 0

    for i in range(1, n + 1):
        args_line = _next_line(stream)
        exp_line = _next_line(stream)
        if args_line is None or exp_line is None:
            return False

        if len(specs) == 1 and specs[0] == "string":
            tokens = [args_line.strip()]
        else:
            tokens = _split_top_level(args_line)

        if len(tokens) != len(specs):
            print(
                f"Case {i} ERROR: argument count mismatch (expected "
                f"{len(specs)}, got {len(tokens)})",
                file=sys.stderr,
            )
            continue

        args = [_parse_value(tok, spec) for tok, spec in zip(tokens, specs)]

        if _is_void(return_type):
            method(*args)
            expected = _parse_value(exp_line, specs[0])
            got = args[0]
        else:
            got = method(*args)
            expected = _parse_value(exp_line, return_type)

        ok = _are_equal(got, expected)
        if ok:
            passed += 1

        message = f"Case {i}: {'PASS' if ok else 'FAIL'}"
        if not ok:
            message += f" (Got: {_to_str(got)}, Expected: {_to_str(expected)})"
        print(message)

    print(f"{passed}/{n} passed")
    return passed == n


def main(run_hook: Callable[[TextIO], bool]) -> int:
    """Entry point analogous to ``main`` in runner.h.

    Reads the testcases path from argv (default ``testcases.txt``), opens it,
    delegates to ``run_hook`` and returns an exit code (0 iff all passed).
    """
    cases_path = sys.argv[1] if len(sys.argv) > 1 else "testcases.txt"
    try:
        with open(cases_path, encoding="utf-8") as stream:
            ok = run_hook(stream)
    except OSError:
        print(f"Failed to open testcases file: {cases_path}", file=sys.stderr)
        return 1
    return 0 if ok else 1
