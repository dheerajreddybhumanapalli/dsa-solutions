# Remove Outermost Parentheses

1. Maintain count of open brackets.
2. If open>0 before adding '(', then add it to final string. If open>0 after adding ')', even then add it to final string.

TC/SC: O(n)/O(n)
