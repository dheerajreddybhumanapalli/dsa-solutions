#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    string reverseParentheses(string s) {
        stack<string> st;
        string ans = "";

        for (char c : s) {
            if(c==')'){
                reverse(ans.begin(), ans.end());
                st.pop();
                while(!st.empty() && st.top() != "("){
                    ans = st.top() + ans;
                    st.pop();
                }
            }
            else if(c=='('){
                st.push(ans);
                st.push("(");
                ans = "";
            }
            else{
                ans += c;
            }

        }

        return ans;
    }
};
