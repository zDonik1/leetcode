#include <memory>

using namespace std;

// 951. Flip Equivalent Binary Trees
// https://leetcode.com/problems/flip-equivalent-binary-trees
// Solved in: 39:55

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
    bool same(TreeNode *node1, TreeNode *node2)
    {
        if (static_cast<bool>(node1) != static_cast<bool>(node2))
            return false;

        // either both nullptr or both have values
        if (node1 == nullptr)
            return true;

        return node1->val == node2->val;
    }

    bool sameChildren(TreeNode *node1, TreeNode *node2)
    {
        return same(node1->left, node2->left) && same(node1->right, node2->right);
    }

    bool flippedChildren(TreeNode *node1, TreeNode *node2)
    {
        return same(node1->left, node2->right) && same(node1->right, node2->left);
    }

    bool flipDfs(TreeNode *root1, TreeNode *root2)
    {
        // either both nullptr or both have values
        if (root1 == nullptr)
            return true;

        if (!sameChildren(root1, root2) && !flippedChildren(root1, root2))
            return false;

        if (sameChildren(root1, root2)) {
            return flipDfs(root1->left, root2->left) && flipDfs(root1->right, root2->right);
        } else {
            return flipDfs(root1->left, root2->right) && flipDfs(root1->right, root2->left);
        }
    }

    bool flipEquiv(TreeNode *root1, TreeNode *root2)
    {
        if (!same(root1, root2))
            return false;

        return flipDfs(root1, root2);
    }
};

#include "doctest.h"

TEST_CASE("test")
{
    auto tree1GGChild1 = TreeNode {7};
    auto tree1GGChild2 = TreeNode {8};
    auto tree1GChild1 = TreeNode {4};
    auto tree1GChild2 = TreeNode {5, &tree1GGChild1, &tree1GGChild2};
    auto tree1GChild3 = TreeNode {6};
    auto tree1Child1 = TreeNode {2, &tree1GChild1, &tree1GChild2};
    auto tree1Child2 = TreeNode {3, &tree1GChild3, nullptr};
    auto tree1Root = TreeNode {1, &tree1Child1, &tree1Child2};

    auto tree2GGChild1 = TreeNode {8};
    auto tree2GGChild2 = TreeNode {7};
    auto tree2GChild1 = TreeNode {6};
    auto tree2GChild2 = TreeNode {4};
    auto tree2GChild3 = TreeNode {5, &tree2GGChild1, &tree2GGChild2};
    auto tree2Child1 = TreeNode {3, &tree2GChild1, nullptr};
    auto tree2Child2 = TreeNode {2, &tree2GChild2, &tree2GChild3};
    auto tree2Root = TreeNode {1, &tree2Child1, &tree2Child2};

    CHECK(Solution {}.flipEquiv(&tree1Root, &tree2Root) == true);
}
