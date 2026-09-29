#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](vector<vector<char>>& grid) -> bool {
        Solution sol;
        return sol.hasValidPath(grid);
    });
}
