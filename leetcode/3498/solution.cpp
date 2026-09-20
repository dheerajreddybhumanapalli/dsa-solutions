#include <string>
using namespace std;

class Solution {
public:
    int reverseDegree(string s) {
        int ans = 0;
        for(size_t i=0; i<s.size(); i++){
            ans+=(26-s[i]+'a')*(i+1);
        }

        return ans;
    }
};