# Find X Value of Array I

1. Maintain dp[k] array of size k, where dp[i] is the number of subarrays ending at current index with product modulo k equal to i.
2. For each index i, create a new dp array ndp of size k.
3. ndp[nums[i]%k] = 1 (for the subarray of length 1 starting and ending at i)
4. Iterate through all the products j of previous subarrays and if dp[j] > 0 then update ndp[(j*nums[i])%k] += dp[j]
5. Add all the counts in ndp[j] to the result[j]
6. dp = ndp
7. return result


TC/SC: O(n*k)/O(k)
