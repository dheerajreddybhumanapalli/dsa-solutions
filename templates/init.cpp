#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](/* arg_type arg_name */) -> /* return_type */ {
        Solution sol;
        return sol.method(/* arg_name */);
    });
}
