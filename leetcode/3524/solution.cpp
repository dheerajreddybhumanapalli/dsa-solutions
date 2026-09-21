#include <vector>

using namespace std;

class Solution {
public:
    vector<long long> resultArray(vector<int>& nums, int k) {
        int n = nums.size();
        vector<long long> result(k, 0);
        vector<long long> dp(k, 0);

        for(int i=0; i<n; i++){
            vector<long long> ndp(k, 0);
            ndp[nums[i]%k]++;
            
            for(int j=0; j<k; j++){
                ndp[(long long)j*nums[i] % k] += dp[j];
            }

            dp = ndp;
            for(int j=0; j<k; j++){
                result[j] += dp[j];
            }
        }

        return result;
    }
};