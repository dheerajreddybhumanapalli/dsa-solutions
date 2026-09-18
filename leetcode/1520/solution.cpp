#include <algorithm>
#include <climits>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    vector<pair<pair<int,int>,string>> validIntervals;
    unordered_map<char,int> mp;
    vector<string> ans;
    int stringsLen = INT_MAX;

    bool isValid(string temp){
        unordered_map<char,int> freqMap;
        for(char ch: temp){
            freqMap[ch]++;
        }

        for(auto it: freqMap){
            if(it.second!=mp[it.first]) return false;
        }

        return true;
    }

    void getValidSubstrings(size_t ind, string s){
        if(ind==s.size()) return;

        string temp = "";
        for(size_t i=ind; i<s.size(); i++){
            temp+=s[i];
            if(isValid(temp)){
                validIntervals.push_back({{ind,i+1},temp});
            }
        }

        getValidSubstrings(ind+1,s);
    }

    void maxNonOverlappingSubstrings(size_t ind, int tempLen, int prevInd, vector<string> temp){
        if(ind>=validIntervals.size()){
            if(ans.size()<temp.size()){
                ans = temp;
                stringsLen = tempLen;
            }
            else if(ans.size()==temp.size()){
                if(tempLen<stringsLen){
                    ans = temp;
                    stringsLen = tempLen;
                }
            }
            return;
        }

        if(temp.size()==0 || prevInd<=validIntervals[ind].first.first){
            temp.push_back(validIntervals[ind].second);
            maxNonOverlappingSubstrings(validIntervals[ind].first.second,tempLen+validIntervals[ind].second.size(),validIntervals[ind].first.second,temp);
            temp.pop_back();
        }
        maxNonOverlappingSubstrings(ind+1,tempLen,prevInd,temp);
    }

    vector<string> maxNumOfSubstrings(string s) {
        for(size_t i=0; i<s.size(); i++){
            mp[s[i]]++;
        }

        getValidSubstrings(0,s);

        sort(validIntervals.begin(),validIntervals.end());

        vector<string> temp;
        maxNonOverlappingSubstrings(0,0,-1,temp);

        return ans;
    }
};

#ifdef LOCAL
int main(){
    string s = "adefaddaccc";
    Solution soln;
    vector<string> ans = soln.maxNumOfSubstrings(s);
    for(size_t i=0; i<ans.size(); i++){
        cout << ans[i] << endl;
    }
    return 0;
}
#endif