# Generate Parentheses

1. Maintain open and close brackets count. If open is less than n, add open bracket and call dfs. If close is less than open, add close bracket and call dfs. When both open and close are equal to n, add the string to the result.

TC/SC: O(2^n)/O(2^n)
