#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](string seq) -> vector<int> {
        Solution sol;
        return sol.maxDepthAfterSplit(seq);
    });
}
