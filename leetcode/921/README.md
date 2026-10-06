# Minimum Add to Make Parentheses Valid

- Maintain Stack for open brackets.
    1. If there are no open brackets for a close bracket, increment ans.
    2. If there are open brackets in the stack even after all traversing, add those empty open brackets to ans as well.

TC/SC: O(n)/O(n)
