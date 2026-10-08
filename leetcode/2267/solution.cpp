#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    bool dfs(int i, int j, int count, vector<vector<char>>& grid, vector<vector<vector<int>>>& dp) {
        if(i<0 || j<0 || i>=(int)grid.size() || j>=(int)grid[0].size()) return false;

        if(i==(int)grid.size()-1 && j==(int)grid[0].size()-1) {
            if(count==1) return true;
            else return false;
        }

        if(grid[i][j]=='(') count++;
        else count--;

        if(count<0) return false;

        if(dp[i][j][count] != -1) return dp[i][j][count];

        bool res = dfs(i+1,j,count,grid,dp) || dfs(i,j+1,count,grid,dp);
        dp[i][j][count] = res;
        return res;
    }
    bool hasValidPath(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(m+n+1, -1)));

        if(grid[0][0]==')' || grid[m-1][n-1]=='(') return false;

        return dfs(0,0,0,grid,dp);
    }
};
