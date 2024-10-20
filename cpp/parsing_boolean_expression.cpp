#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <vector>

using namespace std;

// 1106. Parsing A Boolean Expression
// https://leetcode.com/problems/parsing-a-boolean-expression
// Solved in: 1:31:49

class Solution
{
public:
    enum class Op { And, Or, Not, Invalid };

    Op parseOp(char c)
    {
        switch (c) {
        case '&':
            return Op::And;
        case '|':
            return Op::Or;
        case '!':
            return Op::Not;
        }
        return Op::Invalid;
    }

    bool parseOper(char c) { return c == 't'; }

    bool eval(Op op, vector<bool> const &opers)
    {
        switch (op) {
        case Op::And:
            return reduce(next(cbegin(opers)), cend(opers), opers.front(), logical_and<bool> {});
        case Op::Or:
            return reduce(next(cbegin(opers)), cend(opers), opers.front(), logical_or<bool> {});
        case Op::Not:
            return !opers.front();
        default:
            return false;
        }
    }

    pair<bool, uint> parseTillClosingParen(string_view sv)
    {
        auto const op = parseOp(sv.front());
        auto operands = vector<bool> {};
        sv = sv.substr(2);
        for (auto i = 0u;; ++i) {
            auto const c = sv.at(i);
            switch (c) {
            case 't':
            case 'f':
                operands.push_back(parseOper(c));
                break;
            case ',':
            case '(':
                break;
            case ')':
                return {eval(op, operands), i + 2};
            default: // '!', '&', '|'
                auto const [res, index] = parseTillClosingParen(sv.substr(i));
                i += index;
                operands.push_back(res);
            }
        }
    }

    bool parseBoolExpr(string expression)
    {
        if (expression == "t")
            return true;
        if (expression == "f")
            return false;
        return parseTillClosingParen(expression).first;
    }
};

#include "doctest.h"

TEST_CASE("true")
{
    CHECK(Solution {}.parseBoolExpr("t") == true);
}

TEST_CASE("false")
{
    CHECK(Solution {}.parseBoolExpr("f") == false);
}

TEST_CASE("not true")
{
    CHECK(Solution {}.parseBoolExpr("!(t)") == false);
}

TEST_CASE("not false")
{
    CHECK(Solution {}.parseBoolExpr("!(f)") == true);
}

TEST_CASE("and true")
{
    CHECK(Solution {}.parseBoolExpr("&(t,t,t)") == true);
}

TEST_CASE("and false")
{
    CHECK(Solution {}.parseBoolExpr("&(t,f,t)") == false);
}

TEST_CASE("or true")
{
    CHECK(Solution {}.parseBoolExpr("|(t,f,f)") == true);
}

TEST_CASE("or false")
{
    CHECK(Solution {}.parseBoolExpr("|(f,f,f)") == false);
}

TEST_CASE("complex")
{
    CHECK(Solution {}.parseBoolExpr("|(f,&(t,!(f)))") == true);
}

TEST_CASE("more complex")
{
    CHECK(Solution {}.parseBoolExpr("|(&(t,f))") == false);
}

TEST_CASE("even more complex")
{
    CHECK(Solution {}.parseBoolExpr("|(&(t,f,t),t)") == true);
}

TEST_CASE("even more more complex")
{
    CHECK(Solution {}.parseBoolExpr("|(&(t,f,t),!(t))") == false);
}
