#include <gtest/gtest.h>

#include "avl.h"

class AVLNodeTest : public ::testing::Test {
    protected:
        AVLNode *root = nullptr;

        void TearDown() override {
            avl_delete_tree(&root);
        }
};

TEST_F(AVLNodeTest, Insert_NoRotation) {
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

TEST_F(AVLNodeTest, Insert_SingleRightRotation) {
    root = avl_create_node(10);
    avl_insert_node(&root, 5);
    avl_insert_node(&root, 1);    // trigger rotation at node 10

    EXPECT_EQ(5, root->value);
    EXPECT_EQ(1, root->left->value);
    EXPECT_EQ(10, root->right->value);
    EXPECT_EQ(2, root->height);
}

TEST_F(AVLNodeTest, Insert_SingleLeftRotation) {
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

TEST_F(AVLNodeTest, Insert_DoubleRightLeftRotation) {
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

TEST_F(AVLNodeTest, Insert_DoubleLeftRightRotation) {
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

TEST_F(AVLNodeTest, Remove_LeafNode) {
    int values[]{10, 5, 15, 3, 7};
    root = avl_create_tree(values, 5);
    
    EXPECT_EQ(7, root->left->right->value);
    
    EXPECT_TRUE(avl_remove_node(&root, 7));
    
    EXPECT_EQ(10, root->value);
    EXPECT_EQ(5, root->left->value); 
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(3, root->left->left->value);
    EXPECT_EQ(nullptr, root->left->right);
    EXPECT_EQ(3, root->height);
}

TEST_F(AVLNodeTest, Remove_RootNode) {
    int values[]{10, 5, 15};
    root = avl_create_tree(values, 3);
    
    EXPECT_TRUE(avl_remove_node(&root, 10));
    
    EXPECT_EQ(5, root->value);
    EXPECT_EQ(nullptr, root->left);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);
}

TEST_F(AVLNodeTest, Remove_OnlyRootNode) {
    root = avl_create_node(3);
    
    EXPECT_TRUE(avl_remove_node(&root, 3));
    
    EXPECT_EQ(nullptr, root);
}

TEST_F(AVLNodeTest, Remove_NodeWithTwoChildren_FourLevels) {
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

TEST_F(AVLNodeTest, Remove_NodeWithOnlyLeftChild_FourLevels) {
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

TEST_F(AVLNodeTest, Remove_NodeWithOnlyRightChild_FourLevels) {
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

TEST_F(AVLNodeTest, Remove_NonexistentNode) {
    int values[]{10, 5, 15};
    root = avl_create_tree(values, 3);
    
    EXPECT_FALSE(avl_remove_node(&root, 7));
    
    EXPECT_EQ(10, root->value);
    EXPECT_EQ(5, root->left->value);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
