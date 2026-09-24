#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](vector<int> nums) -> int {
        Solution sol;
        return sol.smallestIndex(nums);
    });
}
