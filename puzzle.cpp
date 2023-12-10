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

    return get_steps_to_farthest_tile(get_steps_grid(grid, start));
}

int do_puzzle_2(std::ifstream &file) {
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

    return get_enclosed_tiles(grid, get_steps_grid(grid, start));
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

std::vector<std::vector<int>> get_steps_grid(const Grid &grid, const std::pair<int, int> &start) {
    int rows = grid.size();
    int cols = grid[0].size();

    std::vector<std::vector<int>> steps(rows, std::vector<int>(cols, -1));
    std::queue<std::pair<int, int>> q;

    q.push(start);

    steps[start.first][start.second] = 0;

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

            q.push(next);
        }
    }

    return steps;
}

int get_steps_to_farthest_tile(std::vector<std::vector<int>> steps_grid) {
    int max_steps = 0;

    for (auto &row: steps_grid) {
        for (auto &step: row) {
            if (step > max_steps) {
                max_steps = step;
            }
        }
    }

    return max_steps;
}

int get_enclosed_tiles(const Grid &grid, std::vector<std::vector<int>> steps_grid) {
    int enclosed_tiles = 0;

    std::vector<std::vector<int>> lgrid{};

    // blow up the grid, so that we can see the tiles and space between pipes that are close together
    // Then we can just fill in the space between the pipes and count the remaining enclosed
    for (int i = 0; i < steps_grid.size(); i++) {
        auto &row = steps_grid[i];
        std::vector<int> new_row1{};
        std::vector<int> new_row2{};
        std::vector<int> new_row3{};

        for (int j = 0; j < row.size(); j++) {
            auto &step = row[j];

            if (step == -1) {
                new_row1.insert(new_row1.end(), {-1, -1, -1});
                new_row2.insert(new_row2.end(), {-1, 0, -1});
                new_row3.insert(new_row3.end(), {-1, -1, -1});
                continue;
            }

            auto blown_vec = direction_blown.at(grid[i][j]);
            new_row1.insert(new_row1.end(), blown_vec[0].begin(), blown_vec[0].end());
            new_row2.insert(new_row2.end(), blown_vec[1].begin(), blown_vec[1].end());
            new_row3.insert(new_row3.end(), blown_vec[2].begin(), blown_vec[2].end());
        }

        lgrid.emplace_back(new_row1);
        lgrid.emplace_back(new_row2);
        lgrid.emplace_back(new_row3);
    }

    flood_fill(lgrid, {0, 0});

    for (auto &row: lgrid) {
        for (auto &step: row) {
//            fmt::print("{:2} ", step > 0 ? "*" : (step == 0 ? "0" : "."));
            if (step == 0) {
                enclosed_tiles++;
            }
        }

//        fmt::println("");
    }

    return enclosed_tiles;
}

void flood_fill(std::vector<std::vector<int>> &blown_grid, const std::pair<int, int> &start) {
    std::queue<std::pair<int, int>> q;

    q.push(start);

    blown_grid[start.first][start.second] = -2;

    while (!q.empty()) {
        auto cur = q.front();
        q.pop();

        for (auto &dir: get_offsets(Direction::ALL)) {
            auto next = std::make_pair(cur.first + dir.first, cur.second + dir.second);

            if (next.first < 0 || next.first >= blown_grid.size() || next.second < 0 ||
                next.second >= blown_grid[0].size()) {
                continue;
            }

            if (blown_grid[next.first][next.second] > 0 || blown_grid[next.first][next.second] == -2) {
                continue;
            }

            blown_grid[next.first][next.second] = -2;

            q.push(next);
        }
    }
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
