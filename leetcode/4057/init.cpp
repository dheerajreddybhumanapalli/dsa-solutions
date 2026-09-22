#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](vector<vector<int>> intervals) -> long long {
        Solution sol;
        return sol.countIntersectingIntervals(intervals);
    });
}
