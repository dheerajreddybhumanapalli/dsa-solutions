# Maximum Number of Non-Overlapping Substrings
 
1. It is a straight forward brute-force solution which will first get all valid substrings by verifying if the substring is valid during the generation.
2. We will need to sort these valid substrings as they may generated early during the process.
3. The next step is that we take intervals of those substrings and find Non-overlapping intervals. During this recursion, we will also see if the conditions mentioned are satisified whether it has maximum substrings and minimum total length.

TC/SC: O(n<sup>3</sup>)/O(n)
