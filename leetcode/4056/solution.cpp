#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    int countIntersectingIntervals(vector<vector<int>>& intervals) {
        sort(intervals.begin(),intervals.end());
        priority_queue<int, vector<int>, greater<int>> pq;
        pq.push(intervals[0][1]);
        int ans = 0;

        for(int i=1; i<intervals.size(); i++){
            while(!pq.empty() && intervals[i][0]>pq.top()){
                pq.pop();
            }
            ans+=pq.size();
            pq.push(intervals[i][1]);
        }

        return ans;
    }
};
