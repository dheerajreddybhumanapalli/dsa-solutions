#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int minAddToMakeValid(string s) {
        stack<char> st;
        int ans = 0;

        for(char ch: s){
            if(ch=='('){
                st.push(ch);
            }
            else{
                if(st.empty()) ans++;
                else st.pop();
            }
        }

        return ans+st.size();
    }
};
