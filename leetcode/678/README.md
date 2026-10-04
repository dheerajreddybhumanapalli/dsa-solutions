# Valid Parenthesis String

1. Maintain `low` and `high` as the minimum and maximum possible number of open parentheses. For `'('`, increment both; for `')'`, decrement both; and for `'*'`, decrement `low` and increment `high`, since it can act as either `')'` or `'('`.
2. If `high < 0`, return `false` as there are too many closing parentheses. Keep `low` at least `0` since the number of open parentheses cannot be negative.
3. At the end, return `true` if `low == 0`, meaning there is a valid assignment of `'*'` characters.

TC/SC: O(n)/O(1)
