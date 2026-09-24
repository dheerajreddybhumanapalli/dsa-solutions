class Solution {
public:
    int smallestIndex(vector<int>& nums) {
        for(int i = 0; i < nums.size(); i++) {
            int number = nums[i], sum = 0;
            
            while(number) {
                sum += number % 10;
                number /= 10;
            }
            if(sum == i) {
                return i;
            }
        }

        return -1;
    }
};
