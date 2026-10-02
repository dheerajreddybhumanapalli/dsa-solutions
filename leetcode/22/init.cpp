#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](int n) -> vector<string> {
        Solution sol;
        return sol.generateParenthesis(n);
    });
}
