// Test runner.
// Usage: ./app [path/to/testcases.txt]
//   Default path is <INIT_DIR>/testcases.txt where INIT_DIR is baked in
//   at compile time via -DINIT_DIR="..." (see Makefile).
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifndef INIT_DIR
#define INIT_DIR "."
#endif

// To support `auto SIGNATURE = [](...) {...};` in init.cpp, we must include
// it directly since lambda closure types cannot be declared via `extern`.
// The Makefile sets `-I<problem_dir>` so "init.cpp" is found automatically.
// We temporarily rename SIGNATURE to avoid duplicate symbol linker errors
// if the Makefile also compiles and links init.o separately.
#define SIGNATURE LOCAL_SIGNATURE
#include "init.cpp"
#undef SIGNATURE

static std::string trim(const std::string &s) {
  size_t b = 0;
  while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b]))) b++;
  size_t e = s.size();
  while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) e--;
  return s.substr(b, e - b);
}

static std::string toLower(std::string s) {
  for (auto &c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  return s;
}

// Next non-empty line (allows blank lines in testcases.txt). Returns false on EOF.
static bool nextLine(std::ifstream &in, std::string &out) {
  while (std::getline(in, out)) {
    // Strip possible trailing '\r' (CRLF files).
    if (!out.empty() && out.back() == '\r') out.pop_back();
    if (!trim(out).empty()) return true;
  }
  return false;
}

// Parse "1, 0, 0, 1, -1, 3, 1" (commas and/or whitespace separated) into ints.
static bool parseInts(const std::string &line, std::vector<long long> &vals) {
  std::string copy = line;
  for (auto &c : copy)
    if (c == ',') c = ' ';
  std::istringstream ss(copy);
  long long v;
  vals.clear();
  while (ss >> v) vals.push_back(v);
  // Reject trailing garbage like "1 2 abc".
  return !ss.fail() || ss.eof();
}

static bool parseBool(const std::string &line, bool &out) {
  std::string t = toLower(trim(line));
  if (t == "true" || t == "1") {
    out = true;
    return true;
  }
  if (t == "false" || t == "0") {
    out = false;
    return true;
  }
  return false;
}

int main(int argc, char **argv) {
  // Uses testcases.txt directly from the problem directory.
  std::string casesPath =
      (argc >= 2) ? argv[1] : std::string(INIT_DIR) + "/testcases.txt";

  std::ifstream in(casesPath);
  if (!in) {
    std::cerr << "Failed to open testcases file: " << casesPath << "\n"
              << "Usage: " << argv[0] << " [path/to/testcases.txt]\n";
    return 1;
  }

  std::string line;
  if (!nextLine(in, line)) {
    std::cerr << "Empty testcases file: " << casesPath << "\n";
    return 1;
  }
  int n = 0;
  try {
    n = std::stoi(trim(line));
  } catch (...) {
    std::cerr << "Bad case count on line 1: '" << line << "'\n";
    return 1;
  }

  int passed = 0;
  for (int i = 0; i < n; i++) {
    std::string argsLine, expLine;
    if (!nextLine(in, argsLine)) {
      std::cerr << "Missing args line for case " << (i + 1) << "\n";
      return 1;
    }
    if (!nextLine(in, expLine)) {
      std::cerr << "Missing expected line for case " << (i + 1) << "\n";
      return 1;
    }

    std::vector<long long> vals;
    if (!parseInts(argsLine, vals) || vals.size() != 7) {
      std::cerr << "Case " << (i + 1) << ": expected 7 comma-separated ints, got '"
                << argsLine << "'\n";
      continue;
    }
    bool expected = false;
    if (!parseBool(expLine, expected)) {
      std::cerr << "Case " << (i + 1) << ": expected 'true'/'false', got '"
                << expLine << "'\n";
      continue;
    }

    // Call the lambda/auto function using the renamed local symbol
    bool got = LOCAL_SIGNATURE((int)vals[0], (int)vals[1], (int)vals[2], (int)vals[3],
                               (int)vals[4], (int)vals[5], (int)vals[6]);
    bool ok = (got == expected);
    if (ok) passed++;
    std::cout << "Case " << (i + 1) << ": " << (ok ? "PASS" : "FAIL")
              << " (got " << (got ? "true" : "false") << ", expected "
              << (expected ? "true" : "false") << ")\n";
  }

  std::cout << passed << "/" << n << " passed\n";
  return (passed == n) ? 0 : 1;
}