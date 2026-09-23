#include <numeric>
using namespace std;

class Solution {
public:
    int minOperations(vector<int>& nums, int x) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        int target = totalSum - x;
        int sum = 0, maxLen = -1;

        for(int r=0, l=0; r<nums.size(); r++) {
            sum += nums[r];
            while(sum > target && l <= r) {
                sum -= nums[l++];
            }
            if(sum == target) {
                maxLen = max(maxLen, r-l+1);
            }
        }

        return maxLen == -1 ? -1 : nums.size() - maxLen;
    }
};
