#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <numeric>
#include <vector>

using namespace std;

// 1652. Defuse the Bomb
// https://leetcode.com/problems/defuse-the-bomb
// Solved in: 00:40:18.155

class Solution
{
public:
    vector<int> decrypt(vector<int> &code, int k)
    {
        if (k == 0)
            return vector<int>(code.size(), 0);

        auto result = vector<int> {};
        auto newKey = k;
        result.reserve(code.size());
        if (k < 0) {
            ranges::reverse(code);
            newKey = abs(newKey);
        }

        auto const firstIter = next(begin(code));
        auto sum = accumulate(firstIter, next(firstIter, newKey), 0);
        result.push_back(sum);
        for (auto i = 1; i < code.size(); ++i) {
            result.push_back(sum += code[(i + newKey) % code.size()] - code[i]);
        }

        if (k < 0) {
            ranges::reverse(result);
        }
        return result;
    }
};

#include "doctest.h"

TEST_CASE("0")
{
    auto code = vector<int> {1, 2, 3, 4};
    CHECK(Solution {}.decrypt(code, 0) == vector<int> {0, 0, 0, 0});
}

TEST_CASE("positive")
{
    auto code = vector<int> {1, 2, 3, 4};
    CHECK(Solution {}.decrypt(code, 2) == vector<int> {5, 7, 5, 3});
}

TEST_CASE("negative")
{
    auto code = vector<int> {1, 2, 3, 4};
    CHECK(Solution {}.decrypt(code, -2) == vector<int> {7, 5, 3, 5});
}
