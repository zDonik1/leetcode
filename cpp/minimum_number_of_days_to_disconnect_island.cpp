#include <algorithm>
#include <bitset>
#include <numeric>
#include <queue>
#include <ranges>
#include <vector>

using namespace std;

// 1568. Minimum Number of Days to Disconnect Island
// https://leetcode.com/problems/minimum-number-of-days-to-disconnect-island
// Solved in: 03:00:37.678 (IN PROGRESS)

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

        auto const landCount = [&grid] {
            auto landCount = 0;
            for (auto const row : grid) {
                for (auto const cell : row) {
                    if (cell == 1)
                        ++landCount;

                    if (landCount == 3)
                        return 3;
                }
            }
            return landCount;
        }();

        if (landCount <= 2)
            return landCount;

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
                if (checkCell(grid, row - 1, col - 1) == 1)
                    land.set(3);
                if (checkCell(grid, row, col - 1) == 1)
                    land.set(4);
                if (checkCell(grid, row + 1, col - 1) == 1)
                    land.set(5);
                if (checkCell(grid, row + 1, col) == 1)
                    land.set(6);
                if (checkCell(grid, row + 1, col + 1) == 1)
                    land.set(7);

                if ((land & bitset<8> {0b01010101}).count() == 1)
                    return 1;

                auto foundWater = false;
                for (auto index : views::iota(0u, land.size())) {
                    if (land[index] == 1)
                        continue;

                    if (foundWater) {
                        foundBridge = true;
                        break;
                    }

                    foundWater = true;

                    // fill with land

                    if (index == 0 && land[land.size() - 1] == 0) {
                        for (auto fillIndex = land.size() - 1; fillIndex >= 0; --fillIndex) {
                            if (land[fillIndex] == 1)
                                break;
                            land[fillIndex] = 1;
                        }
                    }

                    for (auto fillIndex : views::iota(index, land.size())) {
                        if (land[fillIndex] == 1)
                            break;
                        land[fillIndex] = 1;
                    }
                }
            }
        }

        {
            auto landFillGrid = grid;
            auto wasEdgeFilled = false;
            auto wasInsideFilled = false;
            for (auto const row : views::iota(0u, landFillGrid.size())) {
                for (auto const col : views::iota(0u, landFillGrid.front().size())) {
                    if (wasEdgeFilled && wasInsideFilled)
                        return 2;

                    auto const cell = landFillGrid[row][col];
                    if (cell == 1)
                        continue;

                    if (wasInsideFilled) // inside was filled and found more water
                        return 2;

                    (fill(landFillGrid, 1, row, col) ? wasEdgeFilled : wasInsideFilled) = true;
                }
            }
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

TEST_CASE("failing test 5")
{
    // clang-format off
    auto grid = vector<vector<int>> {
        {0, 0, 0}, 
        {0, 1, 0}, 
        {0, 0, 0}
    };
    // clang-format on
    CHECK(Solution {}.minDays(grid) == 1);
}

TEST_CASE("failing test 6")
{
    // clang-format off
    auto grid = vector<vector<int>> {
        {1, 1, 1, 1, 1, 1, 1}, 
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1}, 
        {1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1}
    };
    // clang-format on
    CHECK(Solution {}.minDays(grid) == 1);
}

TEST_CASE("failing test 7")
{
    // clang-format off
    auto grid = vector<vector<int>> {
        {1, 1, 1, 1}, 
        {1, 1, 0, 1}, 
        {1, 1, 1, 1}, 
        {1, 1, 1, 1}
    };
    // clang-format on
    CHECK(Solution {}.minDays(grid) == 2);
}
