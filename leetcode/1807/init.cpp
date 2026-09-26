#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](string s, vector<vector<string>> knowledge) -> string {
        Solution sol;
        return sol.evaluate(s, knowledge);
    });
}
