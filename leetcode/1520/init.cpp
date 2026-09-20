#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](string s) -> vector<string> {
        Solution sol;
        return sol.maxNumOfSubstrings(s);
    });
}