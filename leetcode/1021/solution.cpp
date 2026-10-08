#include <string>
using namespace std;

class Solution {
public:
    string removeOuterParentheses(string s) {
        int open=0;
        string ans = "";

        for(char &ch: s){
            if(ch=='(') {
                if(open>0) ans+=ch;
                open++;
            }
            else{
                open--;
                if(open>0) ans+=ch;
            }
        }

        return ans;
    }
};
