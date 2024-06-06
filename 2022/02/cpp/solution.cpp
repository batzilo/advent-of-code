#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

std::string infile = "input.txt";

int input_to_vector(std::vector<std::pair<char, char>>& v) {
  std::ifstream input(infile);
  if (!input.is_open()) {
    std::cout << "Failed to open file: " << infile << std::endl;
    return -1;
  }

  std::string line;
  char them, us;
  while (getline(input, line)) {
    std::stringstream(line) >> them >> us;
    std::pair<char, char> p(them, us);
    v.push_back(p);
  }

  input.close();
  return 0;
}

enum Hand {
  Unknown,
  Rock,
  Paper,
  Scissors,
};

Hand make_hand(char c) {
  if (c == 'A' || c == 'X') return Hand::Rock;
  if (c == 'B' || c == 'Y') return Hand::Paper;
  if (c == 'C' || c == 'Z') return Hand::Scissors;
  return Hand::Unknown;
}

Hand infer_hand(Hand opp, char result) {
  if (result == 'Y') {
    // Draw
    return opp;
  } else if (result == 'X') {
    // We lose
    if (opp == Hand::Rock) return Hand::Scissors;
    if (opp == Hand::Paper) return Hand::Rock;
    if (opp == Hand::Scissors) return Hand::Paper;
    std::cout << "Unknown hand: " << opp << std::endl;
    return Hand::Unknown;
  } else if (result == 'Z') {
    // We win
    if (opp == Hand::Rock) return Hand::Paper;
    if (opp == Hand::Paper) return Hand::Scissors;
    if (opp == Hand::Scissors) return Hand::Rock;
    std::cout << "Unknown hand: " << opp << std::endl;
    return Hand::Unknown;
  } else {
    std::cout << "Unknown result: " << result << std::endl;
    return Hand::Unknown;
  }
}

int score(Hand them, Hand us) {
  int points = 0;
  if (us == them) {
    points += 3;
  }
  if (us == Hand::Rock) {
    points += 1;
    if (them == Hand::Scissors) points += 6;
  } else if (us == Hand::Paper) {
    points += 2;
    if (them == Hand::Rock) points += 6;
  } else if (us == Hand::Scissors) {
    points += 3;
    if (them == Hand::Paper) points += 6;
  } else {
    std::cout << "Unknown hand: " << us << std::endl;
  }
  return points;
}

int main() {
  std::vector<std::pair<char, char>> v;
  int err = input_to_vector(v);
  if (err) {
    return err;
  }

  int our_score = 0;
  for (auto p : v) {
    Hand them = make_hand(p.first);
    Hand us = make_hand(p.second);
    our_score += score(them, us);
  }
  std::cout << our_score << std::endl;

  our_score = 0;
  for (auto p : v) {
    Hand them = make_hand(p.first);
    Hand us = infer_hand(them, p.second);
    our_score += score(them, us);
  }
  std::cout << our_score << std::endl;
  return 0;
}
