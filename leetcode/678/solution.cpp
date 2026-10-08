#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    bool checkValidString(string s) {
        int low=0, high=0;
        for(int i=0; i<(int)s.size(); i++){
            if(s[i]=='('){
                low++;
                high++;
            }
            else if(s[i]=='*'){
                low--;
                high++;
            }
            else{
                low--;
                high--;
            }

            if(high<0) return false;
            low = max(low,0);
        }

        return low==0;
    }
};
