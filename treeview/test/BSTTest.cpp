#include <gtest/gtest.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "../source/BST.h"

TEST(BSTTest, Insert_NoRotation) {
    BSTNode* root = createNode((int[]){10, 5, 15, 3, 7}, 5);

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

    deleteTree(root);
}

TEST(BSTTest, Insert_SingleRightRotation) {
    BSTNode* root = createNode(10);
    insert(root, 5);
    insert(root, 1);    // trigger rotation at node 10

    EXPECT_EQ(5, root->value);
    EXPECT_EQ(1, root->left->value);
    EXPECT_EQ(10, root->right->value);
    EXPECT_EQ(2, root->height);

    deleteTree(root);
}

TEST(BSTTest, Insert_SingleLeftRotation) {
    BSTNode* root = createNode(10);
    insert(root, 15);
    insert(root, 11);    // trigger rotation at node 10

    EXPECT_EQ(11, root->value);
    EXPECT_EQ(10, root->left->value);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);
    EXPECT_EQ(1, root->left->height);
    EXPECT_EQ(1, root->right->height);

    deleteTree(root);
}

TEST(BSTTest, Insert_DoubleRightLeftRotation) {
    BSTNode* root = createNode((int[]){10, 5, 15, 18, 12}, 5);

    EXPECT_EQ(3, root->height);

    insert(root, 13);    // trigger double rotation at node 10

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

    deleteTree(root);
}

TEST(BSTTest, Insert_DoubleLeftRightRotation) {
    BSTNode* root = createNode((int[]){10, 5, 15, 3, 7}, 5);

    EXPECT_EQ(3, root->height);

    insert(root, 6);    // trigger double rotation at node 10

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

    deleteTree(root);
}

TEST(BSTTest, Remove_LeafNode) {
    BSTNode* root = createNode((int[]){10, 5, 15, 3, 7}, 5);
    
    EXPECT_EQ(7, root->left->right->value);
    
    EXPECT_TRUE(remove(root, 7));
    
    EXPECT_EQ(10, root->value);
    EXPECT_EQ(5, root->left->value); 
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(3, root->left->left->value);
    EXPECT_EQ(nullptr, root->left->right);
    EXPECT_EQ(3, root->height);

    deleteTree(root);
}

TEST(BSTTest, Remove_RootNode) {
    BSTNode* root = createNode((int[]){10, 5, 15}, 3);
    
    EXPECT_TRUE(remove(root, 10));
    
    EXPECT_EQ(5, root->value);
    EXPECT_EQ(nullptr, root->left);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);

    deleteTree(root);
}

TEST(BSTTest, Remove_OnlyRootNode) {
    BSTNode* root = createNode(3);
    
    EXPECT_TRUE(remove(root, 3));
    
    EXPECT_EQ(nullptr, root);
}

TEST(BSTTest, Remove_NodeWithTwoChildren_FourLevels) {
    BSTNode* root = createNode((int[]){20, 10, 30, 5, 15, 25, 35, 3, 7, 12, 17, 22, 27, 32, 37}, 15);
    
    EXPECT_TRUE(remove(root, 10));  // Remove node with 2 children from second level
    
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

    deleteTree(root);
}

TEST(BSTTest, Remove_NodeWithOnlyLeftChild_FourLevels) {
    BSTNode* root = createNode((int[]){20, 10, 30, 5, 15, 25, 35, 3, 12, 17, 22, 27, 32, 37}, 14);
    
    EXPECT_TRUE(remove(root, 5));  // Remove node with only left child from third level
    
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

    deleteTree(root);
}

TEST(BSTTest, Remove_NodeWithOnlyRightChild_FourLevels) {
    BSTNode* root = createNode((int[]){20, 10, 30, 5, 15, 25, 35, 3, 7, 12, 17, 27, 32, 37}, 14);
    
    EXPECT_TRUE(remove(root, 25));  // Remove node with only left child from third level
    
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

    deleteTree(root);
}

TEST(BSTTest, Remove_NonexistentNode) {
    BSTNode* root = createNode((int[]){10, 5, 15}, 3);
    
    EXPECT_FALSE(remove(root, 7));
    
    EXPECT_EQ(10, root->value);
    EXPECT_EQ(5, root->left->value);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);

    deleteTree(root);
}

void initLogging();

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    initLogging();
    return RUN_ALL_TESTS();
}

void initLogging() {
    auto file_logger = spdlog::basic_logger_mt("TreeViewTest", "TreeViewTestLogs.txt");
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);
    file_logger->flush_on(spdlog::level::debug);
    debug("Logging initialized");
}