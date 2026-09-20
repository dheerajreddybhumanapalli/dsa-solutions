#include "runner.h"
#include "solution.cpp"

using namespace std;

bool runTests(istream &in) {
    return runAuto(in, +[](int radius, int xCenter, int yCenter, int x1, int y1, int x2, int y2) -> bool {
        Solution sol;
        return sol.checkOverlap(radius, xCenter, yCenter, x1, y1, x2, y2);
    });
}