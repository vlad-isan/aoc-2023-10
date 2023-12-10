//
// Created by Vlad Isan on 05/12/2023.
//

#ifndef PUZZLE_H
#define PUZZLE_H

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <ranges>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "fmt/core.h"

enum class Direction {
    START = -1,
    BLOCKED = 0,
    NORTH = 1,
    EAST = 2,
    SOUTH = 4,
    WEST = 8
};

// Overload | operator.
constexpr Direction operator|(Direction a, Direction b) {
    return static_cast<Direction>(static_cast<int>(a) | static_cast<int>(b));
}

// Overload & operator.
constexpr Direction operator&(Direction a, Direction b) {
    return static_cast<Direction>(static_cast<int>(a) & static_cast<int>(b));
}

const std::unordered_map<char, Direction> direction_map{
        {'S', Direction::START},
        {'.', Direction::BLOCKED},
        {'|', Direction::NORTH | Direction::SOUTH},
        {'-', Direction::WEST | Direction::EAST},
        {'L', Direction::NORTH | Direction::EAST},
        {'J', Direction::NORTH | Direction::WEST},
        {'7', Direction::SOUTH | Direction::WEST},
        {'F', Direction::SOUTH | Direction::EAST}
};

const std::unordered_map<Direction, std::pair<int, int>> direction_offset{
        {Direction::NORTH, {-1, 0}},
        {Direction::EAST,  {0,  1}},
        {Direction::SOUTH, {1,  0}},
        {Direction::WEST,  {0,  -1}}
};

using GridRow = std::vector<Direction>;
using Grid = std::vector<GridRow>;


int puzzle_sample_1(const std::string &base_file_path);

int puzzle_sample_2(const std::string &base_file_path);

int puzzle_1(const std::string &base_file_path);

int puzzle_2(const std::string &base_file_path);

int do_puzzle_1(std::ifstream &file);

int do_puzzle_2(std::ifstream &file);

void calculate_start_directions(Grid &grid, const std::pair<int, int> &start);

int get_steps_to_farthest_tile(const Grid &grid, const std::pair<int, int> &start);

std::vector<std::pair<int, int>> get_offsets(Direction direction);

#endif //PUZZLE_H
