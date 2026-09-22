#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <cctype>
#include <algorithm>
#include <type_traits>
#include <queue>
#include <cmath>
#include <utility>

using namespace std;

// ============================================================================
// Standard LeetCode Data Structures
// ============================================================================

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


// ============================================================================
// Basic String Utilities & Tokenizer
// ============================================================================

inline string trim(const string &s) {
    size_t b = 0;
    while (b < s.size() && isspace(static_cast<unsigned char>(s[b]))) b++;
    size_t e = s.size();
    while (e > b && isspace(static_cast<unsigned char>(s[e - 1]))) e--;
    return s.substr(b, e - b);
}

inline bool nextLine(istream &in, string &out) {
    while (getline(in, out)) {
        if (!out.empty() && out.back() == '\r') out.pop_back();
        if (!trim(out).empty()) return true;
    }
    return false;
}

inline vector<string> splitTokens(const string &line) {
    vector<string> tokens;
    string current;
    int bracketDepth = 0;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"' && (i == 0 || line[i - 1] != '\\')) {
            inQuotes = !inQuotes;
            current += c;
        } else if (!inQuotes && (c == '[' || c == '{' || c == '(')) {
            bracketDepth++;
            current += c;
        } else if (!inQuotes && (c == ']' || c == '}' || c == ')')) {
            bracketDepth--;
            current += c;
        } else if (c == ',' && bracketDepth == 0 && !inQuotes) {
            tokens.push_back(trim(current));
            current.clear();
        } else {
            current += c;
        }
    }
    if (!trim(current).empty()) {
        tokens.push_back(trim(current));
    }
    return tokens;
}

// ============================================================================
// Generic Value Parser
// ============================================================================

template <typename T>
struct ValueParser;

template <>
struct ValueParser<int> {
    static int parse(const string &s) { return stoi(trim(s)); }
};

template <>
struct ValueParser<long long> {
    static long long parse(const string &s) { return stoll(trim(s)); }
};

template <>
struct ValueParser<unsigned int> {
    static unsigned int parse(const string &s) { return static_cast<unsigned int>(stoul(trim(s))); }
};

template <>
struct ValueParser<unsigned long long> {
    static unsigned long long parse(const string &s) { return stoull(trim(s)); }
};

template <>
struct ValueParser<float> {
    static float parse(const string &s) { return stof(trim(s)); }
};

template <>
struct ValueParser<double> {
    static double parse(const string &s) { return stod(trim(s)); }
};

template <>
struct ValueParser<char> {
    static char parse(const string &s) {
        string t = trim(s);
        if (t.size() >= 2 && ((t.front() == '\'' && t.back() == '\'') || (t.front() == '"' && t.back() == '"'))) {
            return t[1];
        }
        return t.empty() ? '\0' : t[0];
    }
};

template <>
struct ValueParser<string> {
    static string parse(const string &s) {
        string t = trim(s);
        if (t.size() >= 2 && t.front() == '"' && t.back() == '"') {
            return t.substr(1, t.size() - 2);
        }
        return t;
    }
};

template <>
struct ValueParser<bool> {
    static bool parse(const string &s) {
        string t = trim(s);
        for (auto &c : t) c = tolower(static_cast<unsigned char>(c));
        return (t == "true" || t == "1");
    }
};

template <typename T1, typename T2>
struct ValueParser<pair<T1, T2>> {
    static pair<T1, T2> parse(const string &s) {
        string clean = trim(s);
        if (!clean.empty() && (clean.front() == '[' || clean.front() == '(')) clean = clean.substr(1);
        if (!clean.empty() && (clean.back() == ']' || clean.back() == ')')) clean.pop_back();

        vector<string> parts = splitTokens(clean);
        if (parts.size() < 2) return {ValueParser<T1>::parse(""), ValueParser<T2>::parse("")};
        return {ValueParser<T1>::parse(parts[0]), ValueParser<T2>::parse(parts[1])};
    }
};

template <typename T>
struct ValueParser<vector<T>> {
    static vector<T> parse(const string &s) {
        vector<T> res;
        string clean = trim(s);
        if (!clean.empty() && clean.front() == '[') clean = clean.substr(1);
        if (!clean.empty() && clean.back() == ']') clean.pop_back();

        vector<string> tokens = splitTokens(clean);
        for (const auto &token : tokens) {
            if (!trim(token).empty()) res.push_back(ValueParser<T>::parse(token));
        }
        return res;
    }
};

template <>
struct ValueParser<ListNode*> {
    static ListNode* parse(const string &s) {
        vector<int> nums = ValueParser<vector<int>>::parse(s);
        ListNode dummy(0);
        ListNode *curr = &dummy;
        for (int v : nums) {
            curr->next = new ListNode(v);
            curr = curr->next;
        }
        return dummy.next;
    }
};

template <>
struct ValueParser<TreeNode*> {
    static TreeNode* parse(const string &s) {
        string clean = trim(s);
        if (!clean.empty() && clean.front() == '[') clean = clean.substr(1);
        if (!clean.empty() && clean.back() == ']') clean.pop_back();

        vector<string> tokens = splitTokens(clean);
        if (tokens.empty() || trim(tokens[0]).empty() || trim(tokens[0]) == "null") return nullptr;

        TreeNode *root = new TreeNode(stoi(trim(tokens[0])));
        queue<TreeNode*> q;
        q.push(root);
        size_t idx = 1;

        while (!q.empty() && idx < tokens.size()) {
            TreeNode *node = q.front();
            q.pop();

            if (idx < tokens.size()) {
                string valStr = trim(tokens[idx++]);
                if (!valStr.empty() && valStr != "null") {
                    node->left = new TreeNode(stoi(valStr));
                    q.push(node->left);
                }
            }

            if (idx < tokens.size()) {
                string valStr = trim(tokens[idx++]);
                if (!valStr.empty() && valStr != "null") {
                    node->right = new TreeNode(stoi(valStr));
                    q.push(node->right);
                }
            }
        }
        return root;
    }
};

template <typename T>
inline T parseValue(const string &raw) {
    return ValueParser<T>::parse(raw);
}

template <typename Tuple, size_t... Is>
Tuple parseArgsTuple(const vector<string> &tokens, index_sequence<Is...>) {
    return make_tuple(parseValue<decay_t<tuple_element_t<Is, Tuple>>>(tokens[Is])...);
}

// ============================================================================
// Pretty Printing Helpers (for Failure Diagnostics)
// ============================================================================

template <typename T>
inline string valueToString(const T &val);

template <typename T>
inline string valueToString(const vector<T> &vec) {
    string s = "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        s += valueToString(vec[i]);
        if (i + 1 < vec.size()) s += ", ";
    }
    s += "]";
    return s;
}

template <typename T1, typename T2>
inline string valueToString(const pair<T1, T2> &p) {
    return "(" + valueToString(p.first) + ", " + valueToString(p.second) + ")";
}

inline string valueToString(const string &s) {
    return "\"" + s + "\"";
}

inline string valueToString(char c) {
    return string("'") + c + "'";
}

inline string valueToString(bool b) {
    return b ? "true" : "false";
}

inline string valueToString(ListNode *head) {
    string s = "[";
    ListNode *curr = head;
    while (curr) {
        s += to_string(curr->val);
        if (curr->next) s += ", ";
        curr = curr->next;
    }
    s += "]";
    return s;
}

inline string valueToString(TreeNode *root) {
    if (!root) return "[]";
    vector<string> tokens;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode *curr = q.front();
        q.pop();
        if (curr) {
            tokens.push_back(to_string(curr->val));
            q.push(curr->left);
            q.push(curr->right);
        } else {
            tokens.push_back("null");
        }
    }
    while (!tokens.empty() && tokens.back() == "null") tokens.pop_back();

    string s = "[";
    for (size_t i = 0; i < tokens.size(); ++i) {
        s += tokens[i];
        if (i + 1 < tokens.size()) s += ", ";
    }
    s += "]";
    return s;
}

template <typename T>
inline string valueToString(const T &val) {
    if constexpr (is_arithmetic_v<T>) {
        return to_string(val);
    } else {
        return "<value>";
    }
}

// ============================================================================
// Equality Comparison Helpers
// ============================================================================

template <typename T>
bool areEqual(const T &got, const T &expected) {
    if constexpr (is_floating_point_v<T>) {
        return std::abs(got - expected) <= 1e-5;
    } else {
        return got == expected;
    }
}

template <typename T1, typename T2>
bool areEqual(const pair<T1, T2> &got, const pair<T1, T2> &expected) {
    return areEqual(got.first, expected.first) && areEqual(got.second, expected.second);
}

inline bool areEqual(ListNode *got, ListNode *expected) {
    ListNode *p1 = got;
    ListNode *p2 = expected;
    while (p1 && p2) {
        if (p1->val != p2->val) return false;
        p1 = p1->next;
        p2 = p2->next;
    }
    return p1 == nullptr && p2 == nullptr;
}

inline bool areEqual(TreeNode *got, TreeNode *expected) {
    if (!got && !expected) return true;
    if (!got || !expected) return false;
    if (got->val != expected->val) return false;
    return areEqual(got->left, expected->left) && areEqual(got->right, expected->right);
}

// Vector comparison (preserves order first; falls back to sorted for unordered problems)
template <typename T>
bool areEqual(vector<T> got, vector<T> expected) {
    if (got.size() != expected.size()) return false;
    if (got == expected) return true;

    // Try sorted comparison if elements are sortable
    vector<T> sortedGot = got;
    vector<T> sortedExpected = expected;
    sort(sortedGot.begin(), sortedGot.end());
    sort(sortedExpected.begin(), sortedExpected.end());
    return sortedGot == sortedExpected;
}

// ============================================================================
// Test Harness Core
// ============================================================================

template <typename Func, typename Ret, typename... Args>
bool executeHarness(istream &in, Func fn) {
    string line;
    if (!nextLine(in, line)) return false;
    int n = stoi(trim(line));
    int passed = 0;

    for (int i = 0; i < n; ++i) {
        string argsLine, expLine;
        if (!nextLine(in, argsLine) || !nextLine(in, expLine)) return false;

        vector<string> tokens;
        if constexpr (sizeof...(Args) == 1) {
            if constexpr (is_same_v<decay_t<tuple_element_t<0, tuple<Args...>>>, string>) {
                tokens.push_back(trim(argsLine));
            } else {
                tokens = splitTokens(argsLine);
            }
        } else {
            tokens = splitTokens(argsLine);
        }

        if (tokens.size() != sizeof...(Args)) {
            cerr << "Case " << (i + 1) << " ERROR: argument count mismatch (expected "
                 << sizeof...(Args) << ", got " << tokens.size() << ")\n";
            continue;
        }

        auto args = parseArgsTuple<tuple<decay_t<Args>...>>(tokens, index_sequence_for<Args...>{});

        if constexpr (is_void_v<Ret>) {
            // In-place method: check the modified first argument against expected
            apply(fn, args);
            using TargetType = decay_t<tuple_element_t<0, tuple<Args...>>>;
            TargetType expected = parseValue<TargetType>(expLine);
            bool ok = areEqual(get<0>(args), expected);
            if (ok) passed++;
            cout << "Case " << (i + 1) << ": " << (ok ? "PASS" : "FAIL");
            if (!ok) {
                cout << " (Got: " << valueToString(get<0>(args)) << ", Expected: " << valueToString(expected) << ")";
            }
            cout << "\n";
        } else {
            Ret got = apply(fn, args);
            Ret expected = parseValue<Ret>(expLine);

            bool ok = areEqual(got, expected);
            if (ok) passed++;
            cout << "Case " << (i + 1) << ": " << (ok ? "PASS" : "FAIL");
            if (!ok) {
                cout << " (Got: " << valueToString(got) << ", Expected: " << valueToString(expected) << ")";
            }
            cout << "\n";
        }
    }
    cout << passed << "/" << n << " passed\n";
    return passed == n;
}

// Overload for functions/lambdas returning a value
template <typename Ret, typename... Args>
bool runAuto(istream &in, Ret (*func)(Args...)) {
    return executeHarness<decltype(func), Ret, Args...>(in, func);
}

// Overload for functions/lambdas returning void (in-place modifications)
template <typename... Args>
bool runAuto(istream &in, void (*func)(Args...)) {
    return executeHarness<decltype(func), void, Args...>(in, func);
}

// Forward declare problem runner hook
bool runTests(istream &in);

// Central main entry point
#ifndef RUNNER_NO_MAIN
int main(int argc, char **argv) {
    string casesPath = (argc >= 2) ? argv[1] : "testcases.txt";

    ifstream in(casesPath);
    if (!in) {
        cerr << "Failed to open testcases file: " << casesPath << "\n";
        return 1;
    }

    return runTests(in) ? 0 : 1;
}
#endif