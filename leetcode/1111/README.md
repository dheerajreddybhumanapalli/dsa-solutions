# Maximum Nesting Depth of Two Valid Parentheses Strings

1. Maintain ans which assigns 0 or 1 based on the depth of the parentheses. If the depth is even, assign 0, else assign 1. This will ensure that the two strings are valid and have maximum nesting depth. It considers parity of the depth to assign the parentheses to either of the two strings.

TC/SC: O(n)/O(n)
