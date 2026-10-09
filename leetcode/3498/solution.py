# 3498. Reverse Degree of a String
# Version: optimal | Time: O(n) | Space: O(1)
class Solution:
    def reverseDegree(self, s: str) -> int:
        total = 0
        for i, ch in enumerate(s):
            total += (26 - ord(ch) + ord("a")) * (i + 1)
        return total
