# 20. Valid Parentheses
# Version: optimal | Time: O(n) | Space: O(n)
class Solution:
    def isValid(self, s: str) -> bool:
        stack = []
        pairs = {")": "(", "]": "[", "}": "{"}

        for ch in s:
            if ch in "([{":
                stack.append(ch)
            elif not stack or stack[-1] != pairs[ch]:
                return False
            else:
                stack.pop()

        return not stack
