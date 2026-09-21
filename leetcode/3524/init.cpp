#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](vector<int> nums, int k) -> vector<long long> {
        Solution sol;
        return sol.resultArray(nums, k);
    });
}
