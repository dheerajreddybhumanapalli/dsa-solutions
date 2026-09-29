# Check if There Is a Valid Parentheses String Path

1. perform dfs from (0,0) to (m-1,n-1) and keep track of the count of open brackets. If we reach the end and count is 0 then return true else false.
2. count is incremented when we encounter '(' and decremented when we encounter ')'. If count is negative then return false.
3. dp is used to store the result of dfs for a given (i,j,count) to avoid recomputation. If we have already computed the result for a given (i,j,count) then return the stored result.

TC/SC: O(m*n)/O(m*n*(m+n)) where m is the number of rows and n is the number of columns in the grid.
