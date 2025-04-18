#include <gtest/gtest.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "../source/AVL.h"

using spdlog::debug;

class AVLNodeTest : public ::testing::Test {
    protected:
        AVLNode *root = nullptr;

        void TearDown() override {
            deleteAVLNode(root);
        }
};

TEST_F(AVLNodeTest, Insert_NoRotation) {
    int values[]{10, 5, 15, 3, 7};
    root = createAVLTree(values, 5);

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
    root = createAVLNode(10);
    insertAVLNode(root, 5);
    insertAVLNode(root, 1);    // trigger rotation at node 10

    EXPECT_EQ(5, root->value);
    EXPECT_EQ(1, root->left->value);
    EXPECT_EQ(10, root->right->value);
    EXPECT_EQ(2, root->height);
}

TEST_F(AVLNodeTest, Insert_SingleLeftRotation) {
    root = createAVLNode(10);
    insertAVLNode(root, 15);
    insertAVLNode(root, 11);    // trigger rotation at node 10

    EXPECT_EQ(11, root->value);
    EXPECT_EQ(10, root->left->value);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);
    EXPECT_EQ(1, root->left->height);
    EXPECT_EQ(1, root->right->height);
}

TEST_F(AVLNodeTest, Insert_DoubleRightLeftRotation) {
    int values[]{10, 5, 15, 18, 12};
    root = createAVLTree(values, 5);

    EXPECT_EQ(3, root->height);

    insertAVLNode(root, 13);    // trigger double rotation at node 10

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
    root = createAVLTree(values, 5);

    EXPECT_EQ(3, root->height);

    insertAVLNode(root, 6);    // trigger double rotation at node 10

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
    root = createAVLTree(values, 5);
    
    EXPECT_EQ(7, root->left->right->value);
    
    EXPECT_TRUE(removeAVLNode(root, 7));
    
    EXPECT_EQ(10, root->value);
    EXPECT_EQ(5, root->left->value); 
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(3, root->left->left->value);
    EXPECT_EQ(nullptr, root->left->right);
    EXPECT_EQ(3, root->height);
}

TEST_F(AVLNodeTest, Remove_RootNode) {
    int values[]{10, 5, 15};
    root = createAVLTree(values, 3);
    
    EXPECT_TRUE(removeAVLNode(root, 10));
    
    EXPECT_EQ(5, root->value);
    EXPECT_EQ(nullptr, root->left);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);
}

TEST_F(AVLNodeTest, Remove_OnlyRootNode) {
    root = createAVLNode(3);
    
    EXPECT_TRUE(removeAVLNode(root, 3));
    
    EXPECT_EQ(nullptr, root);
}

TEST_F(AVLNodeTest, Remove_NodeWithTwoChildren_FourLevels) {
    int values[]{20, 10, 30, 5, 15, 25, 35, 3, 7, 12, 17, 22, 27, 32, 37};
    root = createAVLTree(values, 15);
    
    EXPECT_TRUE(removeAVLNode(root, 10));  // Remove node with 2 children from second level
    
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
    root = createAVLTree(values, 14);
    
    EXPECT_TRUE(removeAVLNode(root, 5));  // Remove node with only left child from third level
    
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
    root = createAVLTree(values, 14);
    
    EXPECT_TRUE(removeAVLNode(root, 25));  // Remove node with only left child from third level
    
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
    root = createAVLTree(values, 3);
    
    EXPECT_FALSE(removeAVLNode(root, 7));
    
    EXPECT_EQ(10, root->value);
    EXPECT_EQ(5, root->left->value);
    EXPECT_EQ(15, root->right->value);
    EXPECT_EQ(2, root->height);
}

void initLogging();

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    initLogging();
    return RUN_ALL_TESTS();
}

void initLogging() {
    auto file_logger = spdlog::basic_logger_mt("TreeViewTest", "TreeViewTestLogs.log");
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);
    file_logger->flush_on(spdlog::level::debug);
    debug("Logging initialized");
}