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

using namespace std;

// --- Basic String Utilities ---
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

// --- Generic Value Parser ---
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

template <typename T>
struct ValueParser<vector<T>> {
    static vector<T> parse(const string &s) {
        vector<T> res;
        string clean = trim(s);
        if (!clean.empty() && clean.front() == '[') clean = clean.substr(1);
        if (!clean.empty() && clean.back() == ']') clean.pop_back();

        stringstream ss(clean);
        string token;
        while (getline(ss, token, ',')) {
            if (!trim(token).empty()) res.push_back(ValueParser<T>::parse(token));
        }
        return res;
    }
};

template <typename T>
inline T parseValue(const string &raw) {
    return ValueParser<T>::parse(raw);
}

// --- Argument Splitting ---
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

template <typename Tuple, size_t... Is>
Tuple parseArgsTuple(const vector<string> &tokens, index_sequence<Is...>) {
    return make_tuple(parseValue<decay_t<tuple_element_t<Is, Tuple>>>(tokens[Is])...);
}

// --- Equality Comparison Helpers ---
template <typename T>
bool areEqual(const T &got, const T &expected) {
    return got == expected;
}

// Vector comparison (supports order-independent lists like string arrays)
template <typename T>
bool areEqual(vector<T> got, vector<T> expected) {
    if (got.size() != expected.size()) return false;
    sort(got.begin(), got.end());
    sort(expected.begin(), expected.end());
    return got == expected;
}

// --- Test Harness Core ---
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
            if constexpr (is_same_v<tuple_element_t<0, tuple<Args...>>, string>) {
                tokens.push_back(trim(argsLine));
            } else {
                tokens = splitTokens(argsLine);
            }
        } else {
            tokens = splitTokens(argsLine);
        }

        if (tokens.size() != sizeof...(Args)) {
            cerr << "Case " << (i + 1) << " ERROR: argument count mismatch\n";
            continue;
        }

        auto args = parseArgsTuple<tuple<Args...>>(tokens, index_sequence_for<Args...>{});
        Ret got = apply(fn, args);
        Ret expected = parseValue<Ret>(expLine);

        bool ok = areEqual(got, expected);
        if (ok) passed++;
        cout << "Case " << (i + 1) << ": " << (ok ? "PASS" : "FAIL") << "\n";
    }
    cout << passed << "/" << n << " passed\n";
    return passed == n;
}

template <typename Ret, typename... Args>
bool runAuto(istream &in, Ret (*func)(Args...)) {
    return executeHarness<decltype(func), Ret, Args...>(in, func);
}

// Forward declare problem runner hook
bool runTests(istream &in);

// Central main entry point
int main(int argc, char **argv) {
    string casesPath = (argc >= 2) ? argv[1] : "testcases.txt";

    ifstream in(casesPath);
    if (!in) {
        cerr << "Failed to open testcases file: " << casesPath << "\n";
        return 1;
    }

    return runTests(in) ? 0 : 1;
}