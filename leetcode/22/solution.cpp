#include <vector>
#include <string>
using namespace std;

class Solution {
public:
    void dfs(int open, int close, string str, vector<string>& res) {
        if (open == 0 && close == 0) {
            res.push_back(str);
            return;
        }
        if (open > 0) {
            dfs(open - 1, close, str + '(', res);
        }
        if (close > open) {
            dfs(open, close - 1, str + ')', res);
        }
    }
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        dfs(n, n, "", res);
        return res;
    }
};
