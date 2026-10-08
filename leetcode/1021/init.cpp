#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](string s) -> string {
        Solution sol;
        return sol.removeOuterParentheses(s);
    });
}
