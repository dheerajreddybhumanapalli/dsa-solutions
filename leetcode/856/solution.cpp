#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int func(int i, int j, string& s){
        int bal=0, ans=0;

        for(int k=i; k<j; k++){
            bal += s[k]=='(' ? 1 : -1;

            if(bal==0){
                if(k-i==1) ans++;
                else{
                    ans += 2*func(i+1,k,s);
                }
                i = k+1;
            }
        }

        return ans;
    }
    int scoreOfParentheses(string s) {
        return func(0,s.size(),s);
    }
};
