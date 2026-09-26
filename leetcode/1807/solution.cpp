class Solution {
public:
    string evaluate(string s, vector<vector<string>>& knowledge) {
        unordered_map<string,string> mp;
        for(auto it: knowledge){
            mp[it[0]] = it[1];
        }

        int flag = 0;
        string temp = "", ans = "";
        for(size_t i=0; i<s.size(); i++){
            if(s[i]=='('){
                flag = 1;
                continue;
            }
            if(s[i]==')'){
                flag = 0;
                if(mp.find(temp)!=mp.end()){
                    ans+=mp[temp];
                }
                else ans+="?";
                temp = "";
                continue;
            }
            if(flag) temp+=s[i];
            else ans+=s[i];
        }

        return ans;
    }
};
