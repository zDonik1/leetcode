#include <memory>
#include <numeric>
#include <vector>

using namespace std;

// 2641. Cousins in Binary Tree II
// https://leetcode.com/problems/cousins-in-binary-tree-ii
// Solved in: 56:13

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode()
        : val(0)
        , left(nullptr)
        , right(nullptr)
    {
    }

    TreeNode(int x)
        : val(x)
        , left(nullptr)
        , right(nullptr)
    {
    }

    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x)
        , left(left)
        , right(right)
    {
    }
};

class Solution
{
public:
    TreeNode *replaceValueInTree(TreeNode *root)
    {
        auto nodeBuckets = vector<TreeNode *> {};
        nodeBuckets.push_back({root});
        root->val = 0;
        while (!nodeBuckets.empty()) {
            auto sumBuckets = vector<int> {};

            for (auto bucket : nodeBuckets) {
                sumBuckets.push_back((bucket->left ? bucket->left->val : 0)
                                     + (bucket->right ? bucket->right->val : 0));
            }

            auto const sum = accumulate(begin(sumBuckets), end(sumBuckets), 0);

            for (auto i = 0; i < nodeBuckets.size(); ++i) {
                auto currentBucket = nodeBuckets[i];
                auto const bucketResult = sum - sumBuckets.at(i);
                if (currentBucket->left)
                    currentBucket->left->val = bucketResult;
                if (currentBucket->right)
                    currentBucket->right->val = bucketResult;
            }

            auto newNodeBuckets = vector<TreeNode *> {};
            for (auto const bucket : nodeBuckets) {
                if (bucket->left)
                    newNodeBuckets.push_back(bucket->left);
                if (bucket->right)
                    newNodeBuckets.push_back(bucket->right);
            }
            nodeBuckets = newNodeBuckets;
        }
        return root;
    }
};

#include "doctest.h"

TEST_CASE("test")
{
    auto gchild1 = make_unique<TreeNode>(1);
    auto gchild2 = make_unique<TreeNode>(10);
    auto gchild4 = make_unique<TreeNode>(7);
    auto child1 = make_unique<TreeNode>(4, gchild1.get(), gchild2.get());
    auto child2 = make_unique<TreeNode>(9, nullptr, gchild4.get());
    auto root = make_unique<TreeNode>(5, child1.get(), child2.get());

    auto resRoot = Solution {}.replaceValueInTree(root.get());
    CHECK(resRoot->val == 0);
    CHECK(resRoot->left->val == 0);
    CHECK(resRoot->right->val == 0);
    CHECK(resRoot->left->left->val == 7);
    CHECK(resRoot->left->right->val == 7);
    CHECK(resRoot->right->left == nullptr);
    CHECK(resRoot->right->right->val == 11);
}
