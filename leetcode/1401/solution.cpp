#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Solution {
public:
    bool checkOverlap(int radius, int xCenter, int yCenter, int x1, int y1, int x2, int y2) {
        long long ans = 0;

        if(xCenter<x1 || xCenter>x2){
            ans += min(pow(xCenter-x1,2), pow(xCenter-x2,2));
        }
        if(yCenter<y1 || yCenter>y2){
            ans += min(pow(yCenter-y1,2), pow(yCenter-y2,2));
        }

        return ans <= radius*radius;
    }
};
