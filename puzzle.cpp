//
// Created by Vlad Isan on 05/12/2023.
//

#include "puzzle.h"

int puzzle_sample_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

int puzzle_sample_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

int puzzle_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

int puzzle_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

int do_puzzle_1(std::ifstream &file) {
    std::string line;

    Grid grid;
    std::pair<int, int> start;

    while (std::getline(file, line)) {
        GridRow row;

        for (auto c: line) {
            row.emplace_back(direction_map.at(c));

            if (direction_map.at(c) == Direction::START) {
                start = {grid.size(), row.size() - 1};
            }
        }

        grid.emplace_back(row);
    }

    calculate_start_directions(grid, start);

    return get_steps_to_farthest_tile(grid, start);
}

int do_puzzle_2(std::ifstream &file) {
    std::string line;

    while (std::getline(file, line)) {
        fmt::println("{}", line);
    }

    return 0;
}

void calculate_start_directions(Grid &grid, const std::pair<int, int> &start) {
    auto [row, col] = start;

    auto start_direction = grid[row][col];
    Direction dir = Direction::BLOCKED;

    if (row > 0 && (grid[row - 1][col] & Direction::SOUTH) != Direction::BLOCKED) {
        dir = dir | Direction::NORTH;
    }

    if (row < grid.size() - 1 && (grid[row + 1][col] & Direction::NORTH) != Direction::BLOCKED) {
        dir = dir | Direction::SOUTH;
    }

    if (col > 0 && (grid[row][col - 1] & Direction::EAST) != Direction::BLOCKED) {
        dir = dir | Direction::WEST;
    }

    if (col < grid[row].size() - 1 && (grid[row][col + 1] & Direction::WEST) != Direction::BLOCKED) {
        dir = dir | Direction::EAST;
    }

    grid[row][col] = dir;
}

int get_steps_to_farthest_tile(const Grid &grid, const std::pair<int, int> &start) {
    int rows = grid.size();
    int cols = grid[0].size();

    std::vector<std::vector<int>> steps(rows, std::vector<int>(cols, -1));
    std::queue<std::pair<int, int>> q;

    q.push(start);

    steps[start.first][start.second] = 0;

    int max_steps = 0;

    while (!q.empty()) {
       auto cur = q.front();
       q.pop();

        for (auto &dir: get_offsets(grid[cur.first][cur.second])) {
            auto next = std::make_pair(cur.first + dir.first, cur.second + dir.second);

            if (next.first < 0 || next.first >= rows || next.second < 0 || next.second >= cols) {
                continue;
            }

            if (steps[next.first][next.second] != -1) {
                continue;
            }

            auto next_steps = steps[cur.first][cur.second] + 1;
            steps[next.first][next.second] = next_steps;

            max_steps = std::max(max_steps, next_steps);

            q.push(next);
        }
    }

    return max_steps;
}

std::vector<std::pair<int, int>> get_offsets(Direction direction) {
    std::vector<std::pair<int, int>> offsets;

    if ((direction & Direction::NORTH) != Direction::BLOCKED) {
        offsets.emplace_back(direction_offset.at(Direction::NORTH));
    }

    if ((direction & Direction::EAST) != Direction::BLOCKED) {
        offsets.emplace_back(direction_offset.at(Direction::EAST));
    }

    if ((direction & Direction::SOUTH) != Direction::BLOCKED) {
        offsets.emplace_back(direction_offset.at(Direction::SOUTH));
    }

    if ((direction & Direction::WEST) != Direction::BLOCKED) {
        offsets.emplace_back(direction_offset.at(Direction::WEST));
    }

    return offsets;
}
