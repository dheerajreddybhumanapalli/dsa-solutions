// Include the actual solution file directly
#include "solution.cpp"

// Define the signature to bridge the test runner to the Solution class
auto SIGNATURE = [](int radius, int xCenter, int yCenter, int x1, int y1, int x2, int y2) -> bool {
    Solution sol;
    return sol.checkOverlap(radius, xCenter, yCenter, x1, y1, x2, y2);
};