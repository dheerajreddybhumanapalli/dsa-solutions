#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](string s) -> bool {
        Solution sol;
        return sol.checkValidString(s);
    });
}
