/**
ID: savanna2
PROG: ride
LANG: C++
*/
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cmath>
#include <optional>

using namespace std;


int main() {
//#define TESTING

#ifdef TESTING
  const char *test_input = R"(COMETQ
HVNGAT
)";

  std::stringstream in(test_input);
  std::ostream& out = std::cout;

#else  //!TESTING

#define USING_FILE

#ifdef USING_FILE
  std::string input_file = "ride.in";
  std::string output_file = "ride.out";
  std::ofstream out;
  out.open(output_file);
  std::ifstream in;
  in.open(input_file);

#else
  std::istream& in = std::cin;
  std::ostream& out = std::cout;

#endif
#endif

  std::string s;
  in >> s;

  int num1 = 1;
  for(char c : s) {
    num1 *= c - 'A' + 1;
    num1 %= 47;
  }

  in >> s;
  int num2 = 1;
  for(char c : s) {
    num2 *= c - 'A' + 1;
    num2 %= 47;
  }

  out << (num1 == num2 ? "GO" : "STAY") << std::endl;
}

