# Maximum Nesting Depth of the Parentheses

1. Maintain a stack with all open parentheses. For every open parenthesis, push it to the stack and for every close parenthesis, pop the top of the stack. The maximum size of the stack at any point in time is the maximum depth of the parentheses.
2. Return the maximum size of the stack at any point in time as the maximum depth of the parentheses.

TC/SC: O(n)/O(n)
