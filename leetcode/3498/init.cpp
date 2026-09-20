#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](string s) -> int {
        Solution sol;
        return sol.reverseDegree(s);
    });
}
