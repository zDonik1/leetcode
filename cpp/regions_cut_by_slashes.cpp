#include <queue>
#include <vector>
#include <string>

using namespace std;

// 959. Regions Cut By Slashes
// https://leetcode.com/problems/regions-cut-by-slashes
// Solved in: in progress = 2:19:05.146

enum PartitionDir { None, Pos, Neg };

struct Cell {
    PartitionDir partDir = PartitionDir::None;
    bool leftFilled = false;
    bool rightFilled = false;
};

class Grid
{
public:
    Grid(vector<string> &stringGrid)
    {
        auto numBackSlash = int {};
        for (auto const ch : stringGrid.at(0)) {
            if (ch == '\\')
                ++numBackSlash;
        }

        auto const gridSize = stringGrid.size() - numBackSlash / 2;
        grid.reserve(gridSize);

        for (auto const &strRow : stringGrid) {
            auto row = vector<Cell> {};
            row.reserve(gridSize);

            for (auto const ch : strRow) {
                switch (ch) {
                case ' ':
                    row.push_back(Cell {PartitionDir::None});
                    break;
                case '/':
                    row.push_back(Cell {PartitionDir::Pos});
                    break;
                case '\\':
                    row.push_back(Cell {PartitionDir::Neg});
                    break;
                }
            }

            grid.push_back(std::move(row));
        }
    }

    void fill(int rowIndex, int colIndex, bool rightPart)
    {
        auto partQue = queue<tuple<int, int, bool>> {};
        partQue.push({rowIndex, colIndex, rightPart});

        auto const pushQue = [&partQue](tuple<int, int, bool> const &elem) { partQue.push(elem); };

        while (!partQue.empty()) {
            auto [row, col, rp] = partQue.front();
            partQue.pop();
            auto &cell = grid[row][col];
            auto &partFilled = rp ? cell.rightFilled : cell.leftFilled;

            if (partFilled)
                continue;

            partFilled = true;

            if (cell.partDir == PartitionDir::None)
                pushQue({row, col, !rp});

            if (rp) {
                if (col + 1 < grid.size())
                    pushQue({row, col + 1, false});
            } else {
                if (col - 1 >= 0)
                    pushQue({row, col - 1, true});
            }

            if (rp && cell.partDir != PartitionDir::Neg
                || !rp && cell.partDir == PartitionDir::Neg) {
                if (row + 1 < grid.size())
                    pushQue({row + 1, col, grid[row + 1][col].partDir == PartitionDir::Neg});
            } else {
                if (row - 1 >= 0)
                    pushQue({row - 1, col, grid[row - 1][col].partDir == PartitionDir::Pos});
            }
        }
    }

    auto regions() -> int
    {
        auto numRegions = 0;
        for (auto row = 0; row < grid.size(); ++row) {
            for (auto col = 0; col < grid.size(); ++col) {
                auto const &cell = grid[row][col];
                if (!cell.leftFilled) {
                    fill(row, col, false);
                    ++numRegions;
                }
                if (!cell.rightFilled) {
                    fill(row, col, true);
                    ++numRegions;
                }
            }
        }
        return numRegions;
    }

private:
    vector<vector<Cell>> grid;
};

class Solution
{
public:
    int regionsBySlashes(vector<string> &stringGrid)
    {
        if (stringGrid.empty())
            return 0;

        auto grid = Grid {stringGrid};
        return grid.regions();
    }
};

#include "doctest.h"

TEST_CASE("no grid")
{
    auto grid = vector<string> {};
    CHECK(Solution {}.regionsBySlashes(grid) == 0);
}

TEST_CASE("one partition")
{
    auto grid = vector<string> {"/"};
    CHECK(Solution {}.regionsBySlashes(grid) == 2);
}

TEST_CASE("basic")
{
    // clang-format off
    auto grid = vector<string> {
        " /",
        "/ ",
    };
    // clang-format on
    CHECK(Solution {}.regionsBySlashes(grid) == 2);
}

TEST_CASE("edge")
{
    // clang-format off
    auto grid = vector<string> {
        " /",
        "  ",
    };
    // clang-format on
    CHECK(Solution {}.regionsBySlashes(grid) == 1);
}

TEST_CASE("harder")
{
    // clang-format off
    auto grid = vector<string> {
        "/\\",
        "\\/",
    };
    // clang-format on
    CHECK(Solution {}.regionsBySlashes(grid) == 5);
}

TEST_CASE("harder but weird")
{
    // clang-format off
    auto grid = vector<string> {
        "\\/",
        "/\\",
    };
    // clang-format on
    CHECK(Solution {}.regionsBySlashes(grid) == 4);
}

TEST_CASE("just one region")
{
    // clang-format off
    auto grid = vector<string> {
        " /",
        "  ",
    };
    // clang-format on
    CHECK(Solution {}.regionsBySlashes(grid) == 1);
}
