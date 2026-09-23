#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](vector<int> nums, int x) -> int {
        Solution sol;
        return sol.minOperations(nums, x);
    });
}
