#include <algorithm>
#include <bitset>
#include <numeric>
#include <queue>
#include <ranges>
#include <vector>

using namespace std;

// 1568. Minimum Number of Days to Disconnect Island
// https://leetcode.com/problems/minimum-number-of-days-to-disconnect-island
// Solved in: 31:03.566 + 1:01:57.000 + 0:17:00

class Solution
{
public:
    int countLand(vector<vector<int>> const &grid)
    {
        return accumulate(cbegin(grid), cend(grid), 0, [](int val, vector<int> const &row) {
            return val + ranges::count(row, 1);
        });
    }

    int checkCell(vector<vector<int>> const &grid, int row, int col)
    {
        if (row < 0 || row >= grid.size() || col < 0 || col >= grid.front().size())
            return 0;

        return grid[row][col];
    }

    bool fill(vector<vector<int>> &grid, int tile, int row, int col)
    {
        auto isEdge = false;
        auto cellsToVisit = queue<pair<int, int>> {};
        cellsToVisit.push({row, col});
        while (!cellsToVisit.empty()) {
            auto const cell = cellsToVisit.front();
            cellsToVisit.pop();

            if (grid[cell.first][cell.second] == tile)
                continue;

            if (cell.first == 0 || cell.first == grid.size() - 1)
                isEdge = true;
            if (cell.second == 0 || cell.second == grid.front().size() - 1)
                isEdge = true;

            grid[cell.first][cell.second] = tile;

            if (cell.first - 1 >= 0)
                cellsToVisit.push({cell.first - 1, cell.second});
            if (cell.first + 1 < grid.size())
                cellsToVisit.push({cell.first + 1, cell.second});
            if (cell.second - 1 >= 0)
                cellsToVisit.push({cell.first, cell.second - 1});
            if (cell.second + 1 < grid.front().size())
                cellsToVisit.push({cell.first, cell.second + 1});
        }
        return isEdge;
    }

    int minDays(vector<vector<int>> &grid)
    {
        auto const sizeSum = grid.size() + grid.front().size();
        if (sizeSum <= 3) // 1x2 or 2x1 grid
            return countLand(grid);

        // check if there are at least 2 islands already
        {
            auto waterFillGrid = grid;
            auto wasFilled = false;
            for (auto const row : views::iota(0u, waterFillGrid.size())) {
                for (auto const col : views::iota(0u, waterFillGrid.front().size())) {
                    auto const cell = waterFillGrid[row][col];
                    if (cell == 0)
                        continue;

                    if (wasFilled)
                        return 0;

                    fill(waterFillGrid, 0, row, col);
                    wasFilled = true;
                }
            }
        }

        auto foundBridge = false;
        for (auto const row : views::iota(0u, grid.size())) {
            for (auto const col : views::iota(0u, grid.front().size())) {
                auto const cell = grid[row][col];
                if (cell == 0)
                    continue;

                auto land = bitset<8> {};
                if (checkCell(grid, row, col + 1) == 1)
                    land.set(0);
                if (checkCell(grid, row - 1, col + 1) == 1)
                    land.set(1);
                if (checkCell(grid, row - 1, col) == 1)
                    land.set(2);
                if (checkCell(grid, row - 1, col) == 1)
                    land.set(2);
                if (checkCell(grid, row, col - 1) == 1)
                    land.set(2);
                if (checkCell(grid, row + 1, col) == 1)
                    land.set(3);

                if (land.count() == 1)
                    return 1;

                if ((land.count() == 2 && land[0] == land[2]) ||)
                    foundBridge = true;
            }
        }

        {
            auto landFillGrid = grid;
            auto wasFilled = false;
            for (auto const row : views::iota(0u, landFillGrid.size())) {
                for (auto const col : views::iota(0u, landFillGrid.front().size())) {
                    auto const cell = landFillGrid[row][col];
                    if (cell == 1)
                        continue;

                    if (wasFilled)
                        return 2;

                    if (!fill(landFillGrid, 1, row, col))
                        wasFilled = true;
                }
            }

            if (wasFilled)
                return 2;
        }

        return foundBridge ? 1 : 2;
    }
};

#include "doctest.h"

TEST_CASE("1x1")
{
    auto grid = vector<vector<int>> {{1}};
    CHECK(Solution {}.minDays(grid) == 1);
}

TEST_CASE("1x2 1 land")
{
    auto grid = vector<vector<int>> {{1, 0}};
    CHECK(Solution {}.minDays(grid) == 1);
}

TEST_CASE("1x2 2 land")
{
    auto grid = vector<vector<int>> {{1, 1}};
    CHECK(Solution {}.minDays(grid) == 2);
}

TEST_CASE("test 1")
{
    // clang-format off
    auto grid = vector<vector<int>> {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };
    // clang-format on
    CHECK(Solution {}.minDays(grid) == 2);
}

TEST_CASE("failing test 1")
{
    // clang-format off
    auto grid = vector<vector<int>> {
        {1, 1, 0, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1}
    };
    // clang-format on
    CHECK(Solution {}.minDays(grid) == 1);
}

TEST_CASE("failing test 2")
{
    // clang-format off
    auto grid = vector<vector<int>> {
        {1, 1, 0, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 0, 1, 1},
        {1, 1, 1, 1, 1}
    };
    // clang-format on
    CHECK(Solution {}.minDays(grid) == 2);
}

TEST_CASE("failing test 3")
{
    // clang-format off
    auto grid = vector<vector<int>> {
        {1, 0, 1, 0}
    };
    // clang-format on
    CHECK(Solution {}.minDays(grid) == 0);
}

TEST_CASE("failing test 4")
{
    // clang-format off
    auto grid = vector<vector<int>> {
        {1, 1, 0},
        {1, 1, 1},
        {0, 1, 1}
    };
    // clang-format on
    CHECK(Solution {}.minDays(grid) == 1);
}
