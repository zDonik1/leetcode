#include <cctype>
#include <algorithm>
#include <stack>
#include <numeric>
#include <string>
#include <map>
#include <vector>

#include "doctest.h"

using namespace std;

// 726. Number of Atoms
// https://leetcode.com/problems/number-of-atoms/
// Solved in: N/A

class Solution
{
public:
    string countOfAtoms(string formula)
    {
        stack<map<string, int>> radicalStack {};
        radicalStack.push({});

        auto name = string {};
        auto count = 0;
        auto inNumber = false;
        auto radicalFinished = false;

        auto const reset = [&] {
            name = string {};
            count = 0;
            inNumber = false;
        };

        auto const addElement = [&] {
            radicalStack.top()[name] += max(1, count);
            reset();
        };

        auto const tryFinishRadical = [&] {
            if (!radicalFinished)
                return false;

            auto const radical = std::move(radicalStack.top());
            radicalStack.pop();

            count = max(1, count);
            for (auto const &pair : radical) {
                radicalStack.top()[pair.first] += pair.second * count;
            }
            radicalFinished = false;
            reset();
            return true;
        };

        for (auto const &ch : formula) {
            if (isdigit(ch)) {
                count = count * 10 + ch - '0';
                inNumber = true;
                continue;
            }

            if (!tryFinishRadical() && !islower(ch) && !name.empty())
                addElement();

            if (ch == '(') {
                radicalStack.push({});
                continue;
            } else if (ch == ')') {
                radicalFinished = true;
                continue;
            }

            name += ch;
        }
        if (!tryFinishRadical())
            addElement();

        return accumulate(cbegin(radicalStack.top()), cend(radicalStack.top()), string {},
                          [](auto const &acc, auto const &pair) {
                              return acc + pair.first
                                   + (pair.second == 1 ? string {} : to_string(pair.second));
                          });
    }
};

TEST_CASE("simple")
{
    CHECK(Solution {}.countOfAtoms("H2O") == "H2O");
}

TEST_CASE("unordered")
{
    CHECK(Solution {}.countOfAtoms("O2H") == "HO2");
}

TEST_CASE("multi character element")
{
    CHECK(Solution {}.countOfAtoms("MgOH") == "HMgO");
}

TEST_CASE("single radical")
{
    CHECK(Solution {}.countOfAtoms("(OH2)") == "H2O");
}

TEST_CASE("stacked radicals")
{
    CHECK(Solution {}.countOfAtoms("(OH2)2") == "H4O2");
}

TEST_CASE("radical with element after")
{
    CHECK(Solution {}.countOfAtoms("(OH)2Mg") == "H2MgO2");
}

TEST_CASE("nested radicals")
{
    CHECK(Solution {}.countOfAtoms("K4(ON(SO3)2)2") == "K4N2O14S4");
}

TEST_CASE("same element")
{
    CHECK(Solution {}.countOfAtoms("H2OH") == "H3O");
}
