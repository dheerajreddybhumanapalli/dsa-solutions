# 1401. Circle and Rectangle Overlapping
# Version: optimal | Time: O(1) | Space: O(1)
class Solution:
    def checkOverlap(self, radius, xCenter, yCenter, x1, y1, x2, y2) -> bool:
        ans = 0

        if xCenter < x1 or xCenter > x2:
            ans += min((xCenter - x1) ** 2, (xCenter - x2) ** 2)
        if yCenter < y1 or yCenter > y2:
            ans += min((yCenter - y1) ** 2, (yCenter - y2) ** 2)

        return ans <= radius * radius
