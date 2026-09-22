# Number of Intersecting Interval Pairs I

1. Sort the intervals by their start time.
2. Maintain a min-heap of the end times of the intervals that are currently active (i.e., those that have started but not yet ended).
3. For each interval, remove from the heap all intervals that have ended before the current interval's start time. The size of the heap after this operation will give the number of intervals that intersect with the current interval. Add this count to the total count of intersecting pairs.
4. Add the current interval's end time to the heap.
5. Return the total count of intersecting pairs
