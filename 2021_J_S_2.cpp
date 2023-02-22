#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//#define TESTING

#ifdef TESTING
const char *test_input = R"(8 2
ABBAABCB
3 6
1 4
)";

std::stringstream io(test_input);

#else
std::istream& io = std::cin;
#endif


int main() {
  int N, Q;
  io >> N >> Q;

  std::string pattern;
  io >> pattern;

  int current_count = 0;
  std::set<char> connectable;

  // preprocess the data
  // from left to right, get the min value and visited set
  int left_to_right[N];
  for (int n = 0; n < N; ++n) {
    char current_char = pattern[n];
    if(connectable.count(current_char)) {
      // no need to increment count;
    } else {
      // cannot connect with previous one
      current_count++;
    }

    while(connectable.rbegin() != connectable.rend()) {
      char current_max = *connectable.rbegin();
      if (current_max > current_char) {
        connectable.erase(current_max);
      } else {
        break;
      }
    }

    connectable.insert(current_char);
    left_to_right[n] = current_count;
  }

  // from right to left, get the min value and visited set
  int right_to_left[N];
  current_count = 0;
  connectable.clear();
  for (int n = N-1; n >= 0; --n) {
    char current_char = pattern[n];
    if(connectable.count(current_char)) {
      // no need to increment count;
    } else {
      // cannot connect with previous one
      current_count++;
    }

    while(connectable.rbegin() != connectable.rend()) {
      char current_max = *connectable.rbegin();
      if (current_max > current_char) {
        connectable.erase(current_max);
      } else {
        break;
      }
    }

    connectable.insert(current_char);
    right_to_left[n] = current_count;
  }


  // run query
  for (int q = 0; q < Q; ++q) {
    int a,b;
    io >> a >> b;
    a--;
    b--;

    int total = 0;

    if (a > 0) {
      total += left_to_right[a-1];
    }

    if (b < N-1) {
      total += right_to_left[b+1];
    }

    std::cout<< total << std::endl;
  }

  return 0;
}

