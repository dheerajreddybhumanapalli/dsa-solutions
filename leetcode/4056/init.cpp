#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](vector<vector<int>> intervals) -> int {
        Solution sol;
        return sol.countIntersectingIntervals(intervals);
    });
}
