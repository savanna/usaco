#include <algorithm>
#include <array>
#include <cstdlib>
#include <memory>
#include <bitset>
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
#include <queue>

using namespace std;


int main() {
#define TESTING
//#define USING_FILE

#ifdef TESTING
#ifdef USING_FILE
  std::string input_file = "test_data/prob1_silver_open24/2.in";
  std::ifstream in;
  in.open(input_file);
#else //USING_FILE
  const char *test_input = R"(5 4
3 1
1 5
3 5
1 1
2 1 1 5
1 5 3 4
3 1 3 5
2 1 2 1
3 2 3 3
)";
  test_input = R"(2 8
1 1
1 2
0 2
0 3
0 0
0 1
2 3
2 0
1 1 2 1
1 0 1 3)";
  test_input = R"(1 12
0 0
2 0
2 1
1 1
1 2
3 2
3 3
1 3
1 4
2 4
2 5
0 5
2 2 0 2)";
  std::stringstream in(test_input);
#endif //USING_FILE

  // always output in console when testing
  std::ostream& out = std::cout;

#else  //TESTING

#ifdef USING_FILE
  std::string input_file = "prob1_silver_open24/4.in";
  std::ifstream in;
  in.open(input_file);

  std::string output_file = "ride.out";
  std::ofstream out;
  out.open(output_file);

#else //USING_FILE
  std::istream& in = std::cin;
  std::ostream& out = std::cout;

#endif //USING_FILE
#endif //TESTING

  int N, P;
  in >> N >> P;

  // posts
  struct Point {
    int64_t x;
    int64_t y;
  };

  struct Post {
    bool IsBad() {
      return link_count != 2;
    }

    Point position;
    int link_count = 0;
    int hit_count = 0;
    Post *left;
    Post *right;
    Post *up;
    Post *down;
  };

  std::vector<Post> posts;
  for (int i = 0; i < P; ++i) {
    Post p;
    in >> p.position.x >> p.position.y;
    posts.push_back(std::move(p));
  }

  // iterate on vector again to make sure no memory movement and stable pointer
  std::map<int64_t, std::map<int64_t, Post*>> x_y_posts;
  std::map<int64_t, std::map<int64_t, Post*>> y_x_posts;
  for (Post& p : posts) {
    x_y_posts[p.position.x].emplace(p.position.y, &p);
    y_x_posts[p.position.y].emplace(p.position.x, &p);
  }

  // start/end points
  struct Cow {
    bool IsForward(int64_t total_distance) {
      if (start_distance < end_distance) {
        return (end_distance - start_distance) < total_distance / 2;
      }
      return (start_distance - end_distance) > total_distance /2;
    }

    Point start;
    Point end;
    int64_t start_distance = -1;
    int64_t end_distance = -1;
  };

  std::vector<Cow> cows;
  for (int i =0; i < N; ++i) {
    Cow c;
    in >> c.start.x >> c.start.y;
    in >> c.end.x >> c.end.y;
    cows.push_back(std::move(c));
  }

  // iterate on vector again to make sure no memory movement and stable pointer
  std::map<int64_t, std::map<int64_t, std::set<Cow*>>> x_y_cows;
  std::map<int64_t, std::map<int64_t, std::set<Cow*>>> y_x_cows;
  std::map<int64_t, std::map<int64_t, std::set<Cow*>>> x_y_cows_end;
  std::map<int64_t, std::map<int64_t, std::set<Cow*>>> y_x_cows_end;
  for (Cow& p : cows) {
    x_y_cows[p.start.x][p.start.y].emplace(&p);
    y_x_cows[p.start.y][p.start.x].emplace(&p);
    x_y_cows_end[p.end.x][p.end.y].emplace(&p);
    y_x_cows_end[p.end.y][p.end.x].emplace(&p);
  }

  // connect posts
  for (auto itx : x_y_posts) {
    Post* pre_post = nullptr;
    for (auto ity : itx.second) {
      if (pre_post) {
        pre_post->up = ity.second;
        pre_post->link_count++;

        ity.second->down = pre_post;
        ity.second->link_count++;
      }

      pre_post = ity.second;
    }
  }

  for (auto ity : y_x_posts) {
    Post* pre_post = nullptr;
    for (auto itx : ity.second) {
      if (pre_post) {
        pre_post->right = itx.second;
        pre_post->link_count++;

        itx.second->left = pre_post;
        itx.second->link_count++;
      }

      pre_post = itx.second;
    }
  }

  // find posts with 3 connections, remove 1 conection from them and keep every
  // post with 2 connections
  std::set<Post*> bad_posts;
  for (Post& p : posts) {
    if (p.link_count > 2) {
      bad_posts.insert(&p);
    }
  }

  while (!bad_posts.empty()) {
    auto first = bad_posts.begin();
    if (!(*first)->IsBad()) {
      bad_posts.erase((*first));
      continue;
    }

    int connected_bad = 0;
    if((*first)->left != nullptr && (*first)->left->IsBad()) connected_bad++;
    if((*first)->right != nullptr && (*first)->right->IsBad()) connected_bad++;
    if((*first)->up != nullptr && (*first)->up->IsBad()) connected_bad++;
    if((*first)->down != nullptr && (*first)->down->IsBad()) connected_bad++;

    // same number of connected bad, we can just remove those bad connections
    if ((connected_bad + 2) == (*first)->link_count) {
      if ((*first)->left != nullptr && (*first)->left->IsBad()) {
        (*first)->left->right = nullptr;
        (*first)->left->link_count--;

        (*first)->left = nullptr;
        (*first)->link_count--;
      }
      if ((*first)->right != nullptr && (*first)->right->IsBad()) {
        (*first)->right->left = nullptr;
        (*first)->right->link_count--;

        (*first)->right = nullptr;
        (*first)->link_count--;
      }
      if ((*first)->up != nullptr && (*first)->up->IsBad()) {
        (*first)->up->down = nullptr;
        (*first)->up->link_count--;

        (*first)->up = nullptr;
        (*first)->link_count--;
      }
      if ((*first)->down != nullptr && (*first)->down->IsBad()) {
        (*first)->down->up = nullptr;
        (*first)->down->link_count--;

        (*first)->down = nullptr;
        (*first)->link_count--;
      }
    }
  }

  // put start and end points on the fence
  // calculate the 1 dimension coordinates for all posts and start/end points
  Post* first_post = &posts[0];
  Post *next_post = first_post;
  Post *pre_post = nullptr;
  Post *current_post = nullptr;

  int64_t distance = 0;

  std::map<int64_t, Post *> distance_posts;
  std::map<int64_t, std::set<Cow*>> distance_cows;
  while (current_post == nullptr || next_post != first_post) {
    current_post = next_post;

    // try left
    if (current_post->left && current_post->left != pre_post) {
      next_post = current_post->left;
      pre_post = current_post;
      distance_posts.emplace(distance, current_post);

      //find the cows starting in between
      for (auto itx: y_x_cows[current_post->position.y]) {
        if (itx.first < next_post->position.x)
          continue;
        if (itx.first > current_post->position.x)
          break;

        for (auto cow : itx.second) {
          if (cow->start_distance >= 0)
            continue;
          cow->start_distance = distance - itx.first + current_post->position.x;
          distance_cows[cow->start_distance].emplace(cow);
        }
      }

      //find the cows end in between
      for (auto itx: y_x_cows_end[current_post->position.y]) {
        if (itx.first < next_post->position.x)
          continue;
        if (itx.first > current_post->position.x)
          break;

        for (auto cow : itx.second) {
          if (cow->end_distance >= 0)
            continue;
          cow->end_distance = distance - itx.first + current_post->position.x;
        }
      }
    } // try right
    else if (current_post->right && current_post->right != pre_post) {
      next_post = current_post->right;
      pre_post = current_post;
      distance_posts.emplace(distance, current_post);

      //find the cows starting in between
      for (auto itx: y_x_cows[current_post->position.y]) {
        if (itx.first < current_post->position.x)
          continue;
        if (itx.first > next_post->position.x)
          break;

        for (auto cow : itx.second) {
          if (cow->start_distance >= 0)
            continue;
          cow->start_distance = distance + itx.first - current_post->position.x;
          distance_cows[cow->start_distance].emplace(cow);
        }
      }

      //find the cows end in between
      for (auto itx: y_x_cows_end[current_post->position.y]) {
        if (itx.first < current_post->position.x)
          continue;
        if (itx.first > next_post->position.x)
          break;

        for (auto cow : itx.second) {
          if (cow->end_distance >= 0)
            continue;
          cow->end_distance = distance + itx.first - current_post->position.x;
        }
      }
    } // try up
    else if (current_post->up && current_post->up != pre_post) {
      next_post = current_post->up;
      pre_post = current_post;
      distance_posts.emplace(distance, current_post);

      //find the cows starting in between
      for (auto itx: x_y_cows[current_post->position.x]) {
        if (itx.first < current_post->position.y)
          continue;
        if (itx.first > next_post->position.y)
          break;

        for (auto cow : itx.second) {
          if (cow->start_distance >= 0)
            continue;
          cow->start_distance = distance + itx.first - current_post->position.y;
          distance_cows[cow->start_distance].emplace(cow);
        }
      }

      //find the cows end in between
      for (auto itx: x_y_cows_end[current_post->position.x]) {
        if (itx.first < current_post->position.y)
          continue;
        if (itx.first > next_post->position.y)
          break;

        for (auto cow : itx.second) {
          if (cow->end_distance >= 0)
            continue;
          cow->end_distance = distance + itx.first - current_post->position.y;
        }
      }
    } // try down
    else if (current_post->down && current_post->down != pre_post) {
      next_post = current_post->down;
      pre_post = current_post;
      distance_posts.emplace(distance, current_post);

      //find the cows starting in between
      for (auto itx: x_y_cows[current_post->position.x]) {
        if (itx.first < next_post->position.y)
          continue;
        if (itx.first > current_post->position.y)
          break;

        for (auto cow : itx.second) {
          if (cow->start_distance >= 0)
            continue;
          cow->start_distance = distance - itx.first + current_post->position.y;
          distance_cows[cow->start_distance].emplace(cow);
        }
      }

      //find the cows end in between
      for (auto itx: x_y_cows_end[current_post->position.x]) {
        if (itx.first < next_post->position.y)
          continue;
        if (itx.first > current_post->position.y)
          break;

        for (auto cow : itx.second) {
          if (cow->end_distance >= 0)
            continue;
          cow->end_distance = distance - itx.first + current_post->position.y;
        }
      }
    }

    distance += std::abs(current_post->position.x - next_post->position.x) +
                std::abs(current_post->position.y - next_post->position.y);
  }

  // walk the 1 dimension list and update the posts/cows along the way
  // walk the forward direction with cows and count the forward moving ones
  // until all cows are done


  int cow_number = 0;
  auto cow_iter = distance_cows.begin();
  auto post_iter = distance_posts.begin();

  std::map<int64_t, int> end_positions;
  std::map<int64_t, int> end_positions_next_round;
  while (cow_iter != distance_cows.end() && post_iter != distance_posts.end()) {
    while (cow_iter != distance_cows.end() &&
           cow_iter->first <= post_iter->first) {
      for (auto cow : cow_iter->second) {
        if (cow->IsForward(distance)) {
          cow_number++;
          if (cow->end_distance <
              cow->start_distance) {
            // end after traverse the full distance and wraps around
            if (end_positions_next_round.count(
                    cow->end_distance)) {
              end_positions_next_round[cow->end_distance] += 1;
            } else {
              end_positions_next_round[cow->end_distance] = 1;
            }
          } else {
            // current round going forward end positions
            if (end_positions.count(cow->end_distance)) {
              end_positions[cow->end_distance] += 1;
            } else {
              end_positions[cow->end_distance] = 1;
            }
          }
        }
      }
      cow_iter++;
    }

    while (post_iter != distance_posts.end() &&
           cow_iter != distance_cows.end() &&
           post_iter->first < cow_iter->first) {

        // remove already ended cows
        while (!end_positions.empty() &&
              //TODO: may be <=
               end_positions.begin()->first < post_iter->first) {
            cow_number -= end_positions.begin()->second;
            end_positions.erase(end_positions.begin()->first);
        }
        post_iter->second->hit_count += cow_number;
        post_iter++;
    }
  }

  while (cow_iter != distance_cows.end()) {
    for (auto cow : cow_iter->second) {
      if (cow->IsForward(distance)) {
        cow_number++;
        if (cow->end_distance < cow->start_distance) {
          // end after traverse the full distance and wraps around
          if (end_positions_next_round.count(cow->end_distance)) {
            end_positions_next_round[cow->end_distance] += 1;
          } else {
            end_positions_next_round[cow->end_distance] = 1;
          }
        } else {
          // current round going forward end positions
          // no need since there is no post in this round till the end.
          cow_number--;
        }
      }
    }
    cow_iter++;
  }

  while (post_iter != distance_posts.end()) {
    // remove already ended cows
    while (!end_positions.empty() &&
           // TODO: may be <=
           end_positions.begin()->first < post_iter->first) {
      cow_number -=end_positions.begin()->second;
      end_positions.erase(end_positions.begin()->first);
    }
    post_iter->second->hit_count += cow_number;
    post_iter++;
  }

  // start the next round to handle the wrap arounds
  post_iter = distance_posts.begin();
  while (cow_number > 0 && post_iter != distance_posts.end()) {
    // remove already ended cows
    while (!end_positions_next_round.empty() &&
           // TODO: may be <=
           end_positions_next_round.begin()->first < post_iter->first) {
      cow_number--;
      end_positions_next_round.erase(end_positions_next_round.begin()->first);
    }
    post_iter->second->hit_count += cow_number;
    post_iter++;
  }

  // walk the cows backwards and count the backward moving ones until all cows
  // are done

  cow_number = 0;
  auto cow_riter = distance_cows.rbegin();
  auto post_riter = distance_posts.rbegin();


  end_positions.clear();
  end_positions_next_round.clear();
  while (cow_riter != distance_cows.rend() && post_riter != distance_posts.rend()) {
    while (cow_riter != distance_cows.rend() &&
           cow_riter->first >= post_riter->first) {
      for (auto cow : cow_riter->second) {
        if (!cow->IsForward(distance)) {
          cow_number++;
          if (cow->end_distance > cow->start_distance) {
            // end after traverse the full distance and wraps around
            if (end_positions_next_round.count(
                    cow->end_distance)) {
              end_positions_next_round[cow->end_distance] += 1;
            } else {
              end_positions_next_round[cow->end_distance] = 1;
            }
          } else {
            // current round going backward end positions
            if (end_positions.count(cow->end_distance)) {
              end_positions[cow->end_distance] += 1;
            } else {
              end_positions[cow->end_distance] = 1;
            }
          }
        }
      }
      cow_riter++;
    }

    while (post_riter != distance_posts.rend() &&
           cow_riter != distance_cows.rend() &&
           post_riter->first > cow_riter->first) {

        // remove already ended cows
        while (!end_positions.empty() &&
              //TODO: may be <=
               end_positions.rbegin()->first > post_riter->first) {
            cow_number -= end_positions.rbegin()->second;
            end_positions.erase(end_positions.rbegin()->first);
        }
        post_riter->second->hit_count += cow_number;
        post_riter++;
    }
  }

  while (cow_riter != distance_cows.rend()) {
    for (auto cow : cow_riter->second) {
      if (!cow->IsForward(distance)) {
        cow_number++;
        if (cow->end_distance < cow->start_distance) {
          // end after traverse the full distance and wraps around
          if (end_positions_next_round.count(cow->end_distance)) {
            end_positions_next_round[cow->end_distance] += 1;
          } else {
            end_positions_next_round[cow->end_distance] = 1;
          }
        } else {
          // current round going forward end positions
          // no need since there is no post in this round till the end.
          cow_number--;
        }
      }
    }
    cow_riter++;
  }

  while (post_riter != distance_posts.rend()) {
    // remove already ended cows
    while (!end_positions.empty() &&
           // TODO: may be <=
           end_positions.rbegin()->first > post_riter->first) {
      cow_number -= end_positions.rbegin()->second;
      end_positions.erase(end_positions.rbegin()->first);
    }
    post_riter->second->hit_count += cow_number;
    post_riter++;
  }

  // start the next round to handle the wrap arounds
  post_riter = distance_posts.rbegin();
  while (cow_number > 0 && post_riter != distance_posts.rend()) {
    // remove already ended cows
    while (!end_positions_next_round.empty() &&
           // TODO: may be <=
           end_positions_next_round.rbegin()->first > post_riter->first) {
      auto end = end_positions_next_round.rbegin();
      cow_number -= end->second;
      end_positions_next_round.erase(end->first);
    }
    post_riter->second->hit_count += cow_number;
    post_riter++;
  }

  // output the result
  for (auto& p : posts) {
    out << p.hit_count << "\n";
  }
}