#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <ranges>
#include <numeric>
#include <set>
#include <span>
#include <unordered_set>
#include <vector>

using namespace std;

// 2461. Maximum Sum of Distinct Subarrays With Length K
// https://leetcode.com/problems/maximum-sum-of-distinct-subarrays-with-length-k
// Solved in: 55:53.476 + 27:01.600 + 79:10.810

// start with unique span
// slide span and find the new elem in the span from end
// if the new elem is there again, then move span to itr+1
//   recheck last duplicate for all newly added elements
//   for any elem that is duplicate, slide span again and update itrs to keep checking where left

class Solution
{
public:
    auto isUnique(span<int> const &nums) -> bool
    {
        return unordered_set<int> {cbegin(nums), cend(nums)}.size() == nums.size();
    }

    auto unreverse(auto &&range, auto &&revItr)
    {
        return revItr == rend(range) ? cend(range) : next(revItr).base();
    }

    auto findDuplicateFromEnd(span<int> const &nums, int elem)
    {
        auto const cont = nums | views::reverse;
        auto const itr = ranges::find_if(cont, [&elem, foundFirst = false](auto const &e) mutable {
            if (e == elem) {
                if (foundFirst)
                    return true;

                foundFirst = true;
            }
            return false;
        });
        return unreverse(nums, itr);
    }

    auto firstUniqueSpan(span<int> const &nums, size_t size)
    {
        auto wind = span<int> {begin(nums), size};
        while (true) {
            auto noDuplicates = true;
            auto reversed = wind | views::reverse;
            // TODO: can be optimized by setting the end cond to be where the begin itr of previous
            // iteration
            for (auto itr = begin(reversed); itr != end(reversed); ++itr) {
                auto dupItr = ranges::find(next(itr), end(reversed), *itr);
                if (dupItr == end(reversed))
                    continue;

                auto const windBegin = next(unreverse(wind, dupItr));
                if (distance(windBegin, cend(nums)) < size)
                    return span<int> {};

                wind = span<int> {windBegin, size};
                noDuplicates = false;
                break;
            }
            if (noDuplicates)
                break;
        }
        return wind;
    }

    long long maximumSubarraySum(vector<int> &nums, int k)
    {
        auto maxSum = 0;
        auto sp = span<int> {nums};
        while (sp.size() >= k) {
            auto firstUnique = firstUniqueSpan(sp, k);
            cout << firstUnique.front() << endl;
            maxSum = max(accumulate(begin(firstUnique), end(firstUnique), 0), maxSum);
            sp = sp.last(distance(next(begin(firstUnique)), end(sp)));
        }
        return maxSum;
    }

    long long maximumSubarraySum_back(vector<int> &nums, int k)
    {
        auto maxSum = 0;
        auto const checkMaxSum = [&](auto itr, int sum) {
            auto const unique = isUnique(span<int> {itr, next(itr, k)});
            if (sum > maxSum && unique)
                maxSum = sum;
        };

        auto sum = 0;
        for (auto itr = begin(nums); itr != cend(nums) && next(itr, k - 1) != cend(nums); ++itr) {
            if (itr == begin(nums))
                sum = accumulate(itr, next(itr, k), 0);
            else
                sum += *next(itr, k - 1);

            checkMaxSum(itr, sum);
            sum -= *itr;
        }

        return maxSum;
    }
};

#include "doctest.h"

TEST_CASE("normal")
{
    auto code = vector<int> {1, 5, 4, 2, 9, 9, 9};
    CHECK(Solution {}.maximumSubarraySum(code, 3) == 15);
}

TEST_CASE("duplicates")
{
    auto code = vector<int> {4, 4, 4};
    CHECK(Solution {}.maximumSubarraySum(code, 3) == 0);
}

TEST_CASE("findDuplicateFromEnd")
{

    auto nums = vector<int> {1, 5, 4, 2, 4};
    auto sp = span<int> {nums};

    SUBCASE("finds duplicate")
    {
        auto const itr = next(begin(sp), 2);
        CHECK(Solution {}.findDuplicateFromEnd(sp, 4) == itr);
    }

    SUBCASE("non existent element")
    {
        auto const itr = cend(sp);
        CHECK(Solution {}.findDuplicateFromEnd(sp, 3) == itr);
    }
}

TEST_CASE("findDuplicateFromEnd")
{

    auto nums = vector<int> {5, 1, 4, 5, 3, 4};
    auto sp = span<int> {nums};

    SUBCASE("first is unique")
    {
        auto const wind = sp.subspan(0, 3);
        CHECK(ranges::equal(Solution {}.firstUniqueSpan(sp, 3), wind));
    }

    SUBCASE("first not unique")
    {
        auto const wind = sp.subspan(1, 4);
        CHECK(ranges::equal(Solution {}.firstUniqueSpan(sp, 4), wind));
    }

    SUBCASE("none unique")
    {
        auto const wind = span<int> {};
        CHECK(ranges::equal(Solution {}.firstUniqueSpan(sp, 5), wind));
    }
}
