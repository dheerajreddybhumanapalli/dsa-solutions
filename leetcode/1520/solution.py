# 1520. Maximum Number of Non-Overlapping Substrings
# Version: optimal | Time: O(n^3) | Space: O(n)
from collections import Counter
from typing import List


class Solution:
    def maxNumOfSubstrings(self, s: str) -> List[str]:
        total = Counter(s)
        intervals = []

        for i in range(len(s)):
            freq = Counter()
            for j in range(i, len(s)):
                freq[s[j]] += 1
                if all(freq[c] == total[c] for c in freq):
                    intervals.append((i, j + 1, s[i:j + 1]))

        intervals.sort()
        best: List[str] = []
        best_len = float("inf")

        def search(ind, length, prev_end, chosen):
            nonlocal best, best_len
            if ind >= len(intervals):
                if len(chosen) > len(best) or (len(chosen) == len(best) and length < best_len):
                    best = chosen[:]
                    best_len = length
                return

            search(ind + 1, length, prev_end, chosen)

            start, end, sub = intervals[ind]
            if not chosen or prev_end <= start:
                chosen.append(sub)
                nxt = ind + 1
                while nxt < len(intervals) and intervals[nxt][0] < end:
                    nxt += 1
                search(nxt, length + len(sub), end, chosen)
                chosen.pop()

        search(0, 0, -1, [])
        return best
