#include <gtest/gtest.h>

#include "avl_tree.h"

class AVLTreeTest : public ::testing::Test {
    protected:
        AVLNode *root = nullptr;

        void TearDown() override {
            avl_free_tree(&root);
        }
};

TEST_F(AVLTreeTest, Insert_NoRotation) {
    int values[]{10, 5, 15, 3, 7};
    root = avl_create_tree(values, 5);

    EXPECT_EQ(10, root->value);
    EXPECT_EQ(5, root->left->value);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(3, root->left->left->value);
    EXPECT_EQ(7, root->left->right->value);
    EXPECT_EQ(3, root->height);
    EXPECT_EQ(2, root->left->height);
    EXPECT_EQ(1, root->right->height);
    EXPECT_EQ(1, root->left->left->height);
    EXPECT_EQ(1, root->left->right->height);
}

TEST_F(AVLTreeTest, Insert_SingleRightRotation) {
    root = avl_create_node(10);
    avl_insert_node(&root, 5);
    avl_insert_node(&root, 1);    // trigger rotation at node 10

    EXPECT_EQ(5, root->value);
    EXPECT_EQ(1, root->left->value);
    EXPECT_EQ(10, root->right->value);
    EXPECT_EQ(2, root->height);
}

TEST_F(AVLTreeTest, Insert_SingleLeftRotation) {
    root = avl_create_node(10);
    avl_insert_node(&root, 15);
    avl_insert_node(&root, 11);    // trigger rotation at node 10

    EXPECT_EQ(11, root->value);
    EXPECT_EQ(10, root->left->value);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);
    EXPECT_EQ(1, root->left->height);
    EXPECT_EQ(1, root->right->height);
}

TEST_F(AVLTreeTest, Insert_DoubleRightLeftRotation) {
    int values[]{10, 5, 15, 18, 12};
    root = avl_create_tree(values, 5);

    EXPECT_EQ(3, root->height);

    avl_insert_node(&root, 13);    // trigger double rotation at node 10

    EXPECT_EQ(12, root->value);
    EXPECT_EQ(10, root->left->value);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(5, root->left->left->value);
    EXPECT_EQ(13, root->right->left->value);
    EXPECT_EQ(18, root->right->right->value);
    EXPECT_EQ(3, root->height);
    EXPECT_EQ(2, root->left->height);
    EXPECT_EQ(2, root->right->height);
    EXPECT_EQ(1, root->left->left->height);
    EXPECT_EQ(1, root->right->left->height);
    EXPECT_EQ(1, root->right->right->height);
}

TEST_F(AVLTreeTest, Insert_DoubleLeftRightRotation) {
    int values[]{10, 5, 15, 3, 7};
    root = avl_create_tree(values, 5);

    EXPECT_EQ(3, root->height);

    avl_insert_node(&root, 6);    // trigger double rotation at node 10

    EXPECT_EQ(7, root->value);
    EXPECT_EQ(5, root->left->value);
    EXPECT_EQ(10, root->right->value);
    EXPECT_EQ(nullptr, root->right->left);
    EXPECT_EQ(3, root->left->left->value);
    EXPECT_EQ(6, root->left->right->value);
    EXPECT_EQ(15, root->right->right->value);
    EXPECT_EQ(3, root->height);
    EXPECT_EQ(2, root->left->height);
    EXPECT_EQ(2, root->right->height);
    EXPECT_EQ(1, root->left->left->height);
    EXPECT_EQ(1, root->left->right->height);
    EXPECT_EQ(1, root->right->right->height);
}

TEST_F(AVLTreeTest, Remove_RightChildLeaf_LeftRotation) {
    int values[]{10, 5, 15, 7, 20, 6, 25, 30, 4, 8, 16, 22, 28, 35};
    root = avl_create_tree(values, 14);

    // trigger a single left rotation at node 20
    EXPECT_TRUE(avl_remove_node(&root, 16));

    EXPECT_EQ(10, root->value);
    EXPECT_EQ(6, root->left->value);
    EXPECT_EQ(25, root->right->value);
    EXPECT_EQ(5, root->left->left->value);
    EXPECT_EQ(7, root->left->right->value);
    EXPECT_EQ(20, root->right->left->value);
    EXPECT_EQ(30, root->right->right->value);
    EXPECT_EQ(4, root->left->left->left->value);
    EXPECT_EQ(8, root->left->right->right->value);
    EXPECT_EQ(15, root->right->left->left->value);
    EXPECT_EQ(22, root->right->left->right->value);
    EXPECT_EQ(28, root->right->right->left->value);
    EXPECT_EQ(35, root->right->right->right->value);
    EXPECT_EQ(nullptr, root->right->right->right->left);
    EXPECT_EQ(nullptr, root->right->right->right->right);
}

TEST_F(AVLTreeTest, Remove_LeftChildLeaf_DoubleLeftRightRotation) {
    int values[]{10, 5, 45, 7, 40, 6, 25, 30, 4, 8, 44, 22, 28, 35};
    root = avl_create_tree(values, 14);

    // trigger a left rotation at node 25 and a right rotation at node 40.
    EXPECT_TRUE(avl_remove_node(&root, 44));

    EXPECT_EQ(10, root->value);
    EXPECT_EQ(6, root->left->value);
    EXPECT_EQ(30, root->right->value);
    EXPECT_EQ(5, root->left->left->value);
    EXPECT_EQ(7, root->left->right->value);
    EXPECT_EQ(4, root->left->left->left->value);
    EXPECT_EQ(8, root->left->right->right->value);
    EXPECT_EQ(25, root->right->left->value);
    EXPECT_EQ(40, root->right->right->value);
    EXPECT_EQ(22, root->right->left->left->value);
    EXPECT_EQ(28, root->right->left->right->value);
    EXPECT_EQ(35, root->right->right->left->value);
    EXPECT_EQ(45, root->right->right->right->value);
    EXPECT_EQ(nullptr, root->right->right->right->left);
    EXPECT_EQ(nullptr, root->right->right->right->right);
}

TEST_F(AVLTreeTest, Remove_RightChildLeaf_DoubleRightLeftRotation) {
    int values[]{10, 5, 15, 7, 20, 6, 25, 30, 4, 8, 16, 22, 21, 23};
    root = avl_create_tree(values, 14);

    // trigger a right rotation at node 25 and a left rotation at node 20.
    EXPECT_TRUE(avl_remove_node(&root, 16));

    EXPECT_EQ(10, root->value);
    EXPECT_EQ(6, root->left->value);
    EXPECT_EQ(5, root->left->left->value);
    EXPECT_EQ(7, root->left->right->value);
    EXPECT_EQ(4, root->left->left->left->value);
    EXPECT_EQ(8, root->left->right->right->value);
    EXPECT_EQ(22, root->right->value);
    EXPECT_EQ(20, root->right->left->value);
    EXPECT_EQ(25, root->right->right->value);
    EXPECT_EQ(15, root->right->left->left->value);
    EXPECT_EQ(21, root->right->left->right->value);
    EXPECT_EQ(23, root->right->right->left->value);
    EXPECT_EQ(30, root->right->right->right->value);
    EXPECT_EQ(nullptr, root->right->right->left->left);
    EXPECT_EQ(nullptr, root->right->right->left->right);
}

TEST_F(AVLTreeTest, Remove_ReplacedByLeftLargestNode_NoBalancing) {
    int values[]{20, 10, 30, 5, 15, 25, 35, 3, 7, 12, 17, 22, 27, 32};
    root = avl_create_tree(values, 14);

    // find and replace with node 17
    EXPECT_TRUE(avl_remove_node(&root, 20));

    EXPECT_EQ(17, root->value);
    EXPECT_EQ(10, root->left->value);
    EXPECT_EQ(30, root->right->value);
    EXPECT_EQ(5, root->left->left->value);
    EXPECT_EQ(15, root->left->right->value);
    EXPECT_EQ(3, root->left->left->left->value);
    EXPECT_EQ(7, root->left->left->right->value);
    EXPECT_EQ(12, root->left->right->left->value);
    EXPECT_EQ(nullptr, root->left->right->right);
    EXPECT_EQ(25, root->right->left->value);
    EXPECT_EQ(35, root->right->right->value);
    EXPECT_EQ(22, root->right->left->left->value);
    EXPECT_EQ(27, root->right->left->right->value);
    EXPECT_EQ(32, root->right->right->left->value);
    EXPECT_EQ(nullptr, root->right->right->right);
    EXPECT_EQ(4, root->height);
}

TEST_F(AVLTreeTest, Remove_ReplacedByLeftLargestNode_LeftRotationAtReplacedPosition) {
    int values[]{1, -22, 22, 20, 25};
    root = avl_create_tree(values, 5);

    // Remove node 1 (root), which will be replaced by -22 (largest in left subtree), then trigger a left rotation at replaced position.
    EXPECT_TRUE(avl_remove_node(&root, 1));

    EXPECT_EQ(22, root->value);
    EXPECT_EQ(-22, root->left->value);
    EXPECT_EQ(20, root->left->right->value);
    EXPECT_EQ(25, root->right->value);
    EXPECT_EQ(3, root->height);
    EXPECT_EQ(1, root->right->height);
    EXPECT_EQ(2, root->left->height);
    EXPECT_EQ(1, root->left->right->height);
}

TEST_F(AVLTreeTest, Remove_ReplacedByLeftLargestNode_RightRotationAtParentNode) {
    int values[]{40, 25, 60, 12, 30, 50, 70, 8, 27, 28, 45, 55, 65, 9, 26};
    root = avl_create_tree(values, 15);

    // replace by node 30 and trigger a single right rotation at node 28
    EXPECT_TRUE(avl_remove_node(&root, 40));

    EXPECT_EQ(30, root->value);
    EXPECT_EQ(25, root->left->value);
    EXPECT_EQ(60, root->right->value);
    EXPECT_EQ(9, root->left->left->value);
    EXPECT_EQ(27, root->left->right->value);
    EXPECT_EQ(8, root->left->left->left->value);
    EXPECT_EQ(12, root->left->left->right->value);
    EXPECT_EQ(26, root->left->right->left->value);
    EXPECT_EQ(28, root->left->right->right->value);
    EXPECT_EQ(50, root->right->left->value);
    EXPECT_EQ(70, root->right->right->value);
    EXPECT_EQ(45, root->right->left->left->value);
    EXPECT_EQ(55, root->right->left->right->value);
    EXPECT_EQ(65, root->right->right->left->value);
    EXPECT_EQ(nullptr, root->right->right->right);
    EXPECT_EQ(4, root->height);
}

TEST_F(AVLTreeTest, Remove_OnlyRootNode) {
    root = avl_create_node(3);
    
    EXPECT_TRUE(avl_remove_node(&root, 3));
    
    EXPECT_EQ(nullptr, root);
}

TEST_F(AVLTreeTest, Remove_NodeWithTwoChildren_FourLevels) {
    int values[]{20, 10, 30, 5, 15, 25, 35, 3, 7, 12, 17, 22, 27, 32, 37};
    root = avl_create_tree(values, 15);
    
    EXPECT_TRUE(avl_remove_node(&root, 10));  // Remove node with 2 children from second level
    
    EXPECT_EQ(20, root->value);
    EXPECT_EQ(7, root->left->value);  // 7 should move up to replace 10
    EXPECT_EQ(30, root->right->value);
    EXPECT_EQ(5, root->left->left->value);
    EXPECT_EQ(15, root->left->right->value);
    EXPECT_EQ(3, root->left->left->left->value);
    EXPECT_EQ(12, root->left->right->left->value);
    EXPECT_EQ(17, root->left->right->right->value);
    EXPECT_EQ(25, root->right->left->value);
    EXPECT_EQ(35, root->right->right->value);
    EXPECT_EQ(22, root->right->left->left->value);
    EXPECT_EQ(27, root->right->left->right->value);
    EXPECT_EQ(32, root->right->right->left->value);
    EXPECT_EQ(37, root->right->right->right->value);
    EXPECT_EQ(4, root->height);
}

TEST_F(AVLTreeTest, Remove_NodeWithOnlyLeftChild_FourLevels) {
    int values[]{20, 10, 30, 5, 15, 25, 35, 3, 12, 17, 22, 27, 32, 37};
    root = avl_create_tree(values, 14);
    
    EXPECT_TRUE(avl_remove_node(&root, 5));  // Remove node with only left child from third level
    
    EXPECT_EQ(20, root->value);
    EXPECT_EQ(10, root->left->value);
    EXPECT_EQ(30, root->right->value);
    EXPECT_EQ(3, root->left->left->value); // 3 should move up to replace 5
    EXPECT_EQ(nullptr, root->left->left->left); // node is null after shifted up
    EXPECT_EQ(15, root->left->right->value);
    EXPECT_EQ(12, root->left->right->left->value);
    EXPECT_EQ(17, root->left->right->right->value);
    EXPECT_EQ(25, root->right->left->value);
    EXPECT_EQ(35, root->right->right->value);
    EXPECT_EQ(22, root->right->left->left->value);
    EXPECT_EQ(27, root->right->left->right->value);
    EXPECT_EQ(32, root->right->right->left->value);
    EXPECT_EQ(37, root->right->right->right->value);
    EXPECT_EQ(4, root->height);
}

TEST_F(AVLTreeTest, Remove_NodeWithOnlyRightChild_FourLevels) {
    int values[]{20, 10, 30, 5, 15, 25, 35, 3, 7, 12, 17, 27, 32, 37};
    root = avl_create_tree(values, 14);
    
    EXPECT_TRUE(avl_remove_node(&root, 25));  // Remove node with only left child from third level
    
    EXPECT_EQ(20, root->value);
    EXPECT_EQ(10, root->left->value);
    EXPECT_EQ(30, root->right->value);
    EXPECT_EQ(5, root->left->left->value);
    EXPECT_EQ(3, root->left->left->left->value);
    EXPECT_EQ(7, root->left->left->right->value);
    EXPECT_EQ(15, root->left->right->value);
    EXPECT_EQ(12, root->left->right->left->value);
    EXPECT_EQ(17, root->left->right->right->value);
    EXPECT_EQ(27, root->right->left->value); // 27 should move up to replace 25
    EXPECT_EQ(35, root->right->right->value);
    EXPECT_EQ(nullptr, root->right->left->right);
    EXPECT_EQ(32, root->right->right->left->value);
    EXPECT_EQ(37, root->right->right->right->value);
    EXPECT_EQ(4, root->height);
}

TEST_F(AVLTreeTest, Remove_NonexistentNode) {
    int values[]{20, 10, 30, 5, 15, 25, 35, 3, 7, 27, 32};
    root = avl_create_tree(values, 11);

    EXPECT_FALSE(avl_remove_node(&root, 100)); // 100 does not exist

    EXPECT_EQ(20, root->value);
    EXPECT_EQ(10, root->left->value);
    EXPECT_EQ(30, root->right->value);
    EXPECT_EQ(5, root->left->left->value);
    EXPECT_EQ(15, root->left->right->value);
    EXPECT_EQ(25, root->right->left->value);
    EXPECT_EQ(35, root->right->right->value);
    EXPECT_EQ(3, root->left->left->left->value);
    EXPECT_EQ(7, root->left->left->right->value);
    EXPECT_EQ(nullptr, root->left->right->left);
    EXPECT_EQ(nullptr, root->left->right->right);
    EXPECT_EQ(nullptr, root->right->left->left);
    EXPECT_EQ(27, root->right->left->right->value);
    EXPECT_EQ(32, root->right->right->left->value);
    EXPECT_EQ(nullptr, root->right->right->right);
    EXPECT_EQ(4, root->height);
}

TEST_F(AVLTreeTest, UpdateTreeHeight_SimpleTree) {
    root = avl_create_node(10);
    root->left = avl_create_node(5);
    root->right = avl_create_node(15);
    root->left->left = avl_create_node(3);
    root->left->right = avl_create_node(7);
    root->right->left = avl_create_node(13);
    root->right->right = avl_create_node(17);

    // Manually set wrong heights
    root->height = 1;
    root->left->height = 1;
    root->right->height = 1;
    root->left->left->height = 1;
    root->left->right->height = 1;
    root->right->left->height = 1;
    root->right->right->height = 1;

    avl_update_tree_height(root);

    EXPECT_EQ(3, root->height);
    EXPECT_EQ(2, root->left->height);
    EXPECT_EQ(2, root->right->height);
    EXPECT_EQ(1, root->left->left->height);
    EXPECT_EQ(1, root->left->right->height);
    EXPECT_EQ(1, root->right->left->height);
    EXPECT_EQ(1, root->right->right->height);
}

TEST_F(AVLTreeTest, UpdateTreeHeight_SingleNode) {
    root = avl_create_node(42);
    root->height = 0;
    avl_update_tree_height(root);
    EXPECT_EQ(1, root->height);
}

TEST_F(AVLTreeTest, UpdateTreeHeight_AfterInsert) {
    int values[]{10, 5, 15, 3, 7, 13};
    root = avl_create_tree(values, 6);

    // Manually mess up heights
    root->height = 1;
    root->left->height = 1;
    root->right->height = 1;
    root->left->left->height = 1;
    root->left->right->height = 1;
    root->right->left->height = 1;

    avl_update_tree_height(root);

    EXPECT_EQ(3, root->height);
    EXPECT_EQ(2, root->left->height);
    EXPECT_EQ(2, root->right->height);
    EXPECT_EQ(1, root->left->left->height);
    EXPECT_EQ(1, root->left->right->height);
    EXPECT_EQ(1, root->right->left->height);
}

TEST_F(AVLTreeTest, UpdateTreeHeight_AfterRemove) {
    int values[]{10, 5, 15, 3, 7, 13, 17, 1};
    root = avl_create_tree(values, 8);

    avl_remove_node(&root, 10);

    // Mess up heights
    root->height = 1;
    root->left->height = 1;
    root->right->height = 1;
    root->left->left->height = 1;
    root->left->right->height = 1;
    root->right->left->height = 1;
    root->right->right->height = 1;

    avl_update_tree_height(root);

    EXPECT_EQ(3, root->height);
    EXPECT_EQ(2, root->left->height);
    EXPECT_EQ(2, root->right->height);
    EXPECT_EQ(1, root->left->left->height);
    EXPECT_EQ(1, root->left->right->height);
    EXPECT_EQ(1, root->right->left->height);
    EXPECT_EQ(1, root->right->right->height);
}

TEST_F(AVLTreeTest, UpdateTreeHeight_NotChangingCorrectHeights) {
    int values[]{0, 53, 13, 1, 27, 60, 7, 99};
    root = avl_create_tree(values, 8);

    avl_update_tree_height(root);

    EXPECT_EQ(4, root->height);
    EXPECT_EQ(2, root->left->height);
    EXPECT_EQ(3, root->right->height);
    EXPECT_EQ(1, root->left->left->height);
    EXPECT_EQ(1, root->left->right->height);
    EXPECT_EQ(1, root->right->left->height);
    EXPECT_EQ(2, root->right->right->height);
    EXPECT_EQ(1, root->right->right->right->height);
}

TEST_F(AVLTreeTest, UpdateTreeHeight_HeightTenTree) {
    // Build a complete AVL tree of height 10 (1023 nodes)
    const int N = 1023;
    int values[N];
    for (int i = 0; i < N; ++i) {
        values[i] = i + 1;
    }
    root = avl_create_tree(values, N);

    // Mess up all heights to 1
    std::function<void(AVLNode*)> set_heights = [&](AVLNode* node) {
        if (!node) return;
        node->height = 1;
        set_heights(node->left);
        set_heights(node->right);
    };
    set_heights(root);

    avl_update_tree_height(root);

    EXPECT_EQ(10, root->height);

    // Check a few nodes at different levels
    EXPECT_EQ(9, root->left->height);
    EXPECT_EQ(9, root->right->height);
    EXPECT_EQ(8, root->left->left->height);
    EXPECT_EQ(8, root->right->right->height);
    EXPECT_EQ(1, root->left->left->left->left->left->left->left->left->left->height); // leftmost leaf
    EXPECT_EQ(1, root->right->right->right->right->right->right->right->right->right->height); // rightmost leaf
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
