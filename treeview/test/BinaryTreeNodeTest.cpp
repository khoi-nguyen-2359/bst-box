#include <gtest/gtest.h>
#define UNIT_TEST 1
#include <spdlog/sinks/basic_file_sink.h>

#include "../source/BinaryTreeNode.h"

TEST(BinaryTreeNodeTest, ConstructorWithValue_CorrectMemberVariablesInitialized) {
    BinaryTreeNode node(3);
    EXPECT_EQ(3, node.getValue());
    EXPECT_EQ(nullptr, node.getLeft());
    EXPECT_EQ(nullptr, node.getRight());
}

TEST(BinaryTreeNodeTest, ConstructorWithoutValue_CorrectMemberVariablesInitialized) {
    BinaryTreeNode node;
    EXPECT_EQ(-1, node.getValue());
    EXPECT_EQ(nullptr, node.getLeft());
    EXPECT_EQ(nullptr, node.getRight());
}

TEST(BinaryTreeNodeTest, Insert_LeftAndRightChildrenInsertedCorrectly) {
    BinaryTreeNode root(5);
    root.insert(3);
    root.insert(7);

    EXPECT_NE(nullptr, root.getLeft());
    EXPECT_NE(nullptr, root.getRight());
    EXPECT_EQ(3, root.getLeft()->getValue());
    EXPECT_EQ(7, root.getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_DuplicateValueNotInserted) {
    BinaryTreeNode root(5);
    root.insert(3);
    root.insert(7);
    root.insert(3);
    root.insert(7);
 
    EXPECT_EQ(3, root.getLeft()->getValue());
    EXPECT_EQ(7, root.getRight()->getValue());
    EXPECT_EQ(nullptr, root.getLeft()->getLeft());
    EXPECT_EQ(nullptr, root.getLeft()->getRight());
    EXPECT_EQ(nullptr, root.getRight()->getLeft());
    EXPECT_EQ(nullptr, root.getRight()->getRight());
}

TEST(BinaryTreeNodeTest, Insert_ThreeLevels_NoBalancingNeeded) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(3);
    root.insert(7);
    root.insert(12);
    root.insert(18);

    EXPECT_EQ(10, root.getValue());
    EXPECT_EQ(5, root.getLeft()->getValue());
    EXPECT_EQ(15, root.getRight()->getValue());
    EXPECT_EQ(3, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(7, root.getLeft()->getRight()->getValue());
    EXPECT_EQ(12, root.getRight()->getLeft()->getValue());
    EXPECT_EQ(18, root.getRight()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_SingleRightRotation) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(3);
    root.insert(7);
    root.insert(12);
    root.insert(18);
    root.insert(1);

    EXPECT_EQ(4, root.getHeight());

    root.insert(0);  // This should trigger right rotation

    EXPECT_EQ(4, root.getHeight());

    EXPECT_EQ(10, root.getValue());
    EXPECT_EQ(5, root.getLeft()->getValue());
    EXPECT_EQ(15, root.getRight()->getValue());
    EXPECT_EQ(1, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(7, root.getLeft()->getRight()->getValue());
    EXPECT_EQ(12, root.getRight()->getLeft()->getValue());
    EXPECT_EQ(18, root.getRight()->getRight()->getValue());
    EXPECT_EQ(0, root.getLeft()->getLeft()->getLeft()->getValue());
    EXPECT_EQ(3, root.getLeft()->getLeft()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_DoubleRotationLeftRight) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(3);
    root.insert(7);

    EXPECT_EQ(3, root.getHeight());

    root.insert(6);  // This should trigger double rotation (left-right)

    EXPECT_EQ(3, root.getHeight());

    EXPECT_EQ(7, root.getValue());
    EXPECT_EQ(5, root.getLeft()->getValue());
    EXPECT_EQ(10, root.getRight()->getValue());
    EXPECT_EQ(nullptr, root.getRight()->getLeft());
    EXPECT_EQ(3, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(6, root.getLeft()->getRight()->getValue());
    EXPECT_EQ(15, root.getRight()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_SingleLeftRotation) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(12);
    root.insert(18);

    EXPECT_EQ(3, root.getHeight());

    root.insert(20);  // This should trigger single left rotation

    EXPECT_EQ(3, root.getHeight());

    EXPECT_EQ(15, root.getValue());
    EXPECT_EQ(10, root.getLeft()->getValue());
    EXPECT_EQ(18, root.getRight()->getValue());
    EXPECT_EQ(nullptr, root.getRight()->getLeft());
    EXPECT_EQ(20, root.getRight()->getRight()->getValue());
    EXPECT_EQ(5, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(12, root.getLeft()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_DoubleRotationRightLeft) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(18);
    root.insert(12);

    EXPECT_EQ(3, root.getHeight());
    
    root.insert(13);  // This should trigger double rotation (right-left)
    
    EXPECT_EQ(3, root.getHeight());

    EXPECT_EQ(12, root.getValue());
    EXPECT_EQ(10, root.getLeft()->getValue());
    EXPECT_EQ(15, root.getRight()->getValue());
    EXPECT_EQ(5, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(nullptr, root.getLeft()->getRight());
    EXPECT_EQ(13, root.getRight()->getLeft()->getValue());
    EXPECT_EQ(18, root.getRight()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_FourLevels_NoBalancingNeeded) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(3);
    root.insert(7);
    root.insert(12);
    root.insert(18);
    root.insert(1);
    root.insert(4);
    root.insert(6);
    root.insert(8);
    root.insert(11);
    root.insert(13);
    root.insert(17);
    root.insert(19);

    EXPECT_EQ(4, root.getHeight());
    EXPECT_EQ(10, root.getValue());
    EXPECT_EQ(5, root.getLeft()->getValue());
    EXPECT_EQ(15, root.getRight()->getValue());
    EXPECT_EQ(3, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(7, root.getLeft()->getRight()->getValue());
    EXPECT_EQ(12, root.getRight()->getLeft()->getValue());
    EXPECT_EQ(18, root.getRight()->getRight()->getValue());
    EXPECT_EQ(1, root.getLeft()->getLeft()->getLeft()->getValue());
    EXPECT_EQ(4, root.getLeft()->getLeft()->getRight()->getValue());
    EXPECT_EQ(6, root.getLeft()->getRight()->getLeft()->getValue());
    EXPECT_EQ(8, root.getLeft()->getRight()->getRight()->getValue());
    EXPECT_EQ(11, root.getRight()->getLeft()->getLeft()->getValue());
    EXPECT_EQ(13, root.getRight()->getLeft()->getRight()->getValue());
    EXPECT_EQ(17, root.getRight()->getRight()->getLeft()->getValue());
    EXPECT_EQ(19, root.getRight()->getRight()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_FourLevelsOnlyLeftBranch) {
    BinaryTreeNode root(0);
    root.insert(-1);

    EXPECT_EQ(2, root.getHeight());
    EXPECT_EQ(0, root.getValue());
    EXPECT_EQ(-1, root.getLeft()->getValue());

    root.insert(-2);
    
    EXPECT_EQ(2, root.getHeight());

    EXPECT_EQ(-1, root.getValue());
    EXPECT_EQ(-2, root.getLeft()->getValue());
    EXPECT_EQ(0, root.getRight()->getValue());

    root.insert(-3);

    EXPECT_EQ(3, root.getHeight());
    EXPECT_EQ(-1, root.getValue());
    EXPECT_EQ(-3, root.getLeft()->getLeft()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_FourLevelsOnlyRightBranch) {
    BinaryTreeNode root(0);
    root.insert(1);

    EXPECT_EQ(2, root.getHeight());
    EXPECT_EQ(0, root.getValue());
    EXPECT_EQ(1, root.getRight()->getValue());

    root.insert(2);

    EXPECT_EQ(2, root.getHeight());
    EXPECT_EQ(1, root.getValue());
    EXPECT_EQ(0, root.getLeft()->getValue());
    EXPECT_EQ(2, root.getRight()->getValue());

    root.insert(3);

    EXPECT_EQ(3, root.getHeight());
    EXPECT_EQ(1, root.getValue());
    EXPECT_EQ(0, root.getLeft()->getValue());
    EXPECT_EQ(2, root.getRight()->getValue());
    EXPECT_EQ(3, root.getRight()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Constructor_WithArray) {
    int values[] = {10, 5, 15, 3, 7, 12, 18};
    BinaryTreeNode root(values, 7);

    EXPECT_EQ(10, root.getValue());
    EXPECT_EQ(5, root.getLeft()->getValue());
    EXPECT_EQ(15, root.getRight()->getValue());
    EXPECT_EQ(3, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(7, root.getLeft()->getRight()->getValue());
    EXPECT_EQ(12, root.getRight()->getLeft()->getValue());
    EXPECT_EQ(18, root.getRight()->getRight()->getValue());
    EXPECT_EQ(3, root.getHeight());

    // Test with empty array
    int emptyArray[] = {};
    BinaryTreeNode emptyRoot(emptyArray, 0);
    EXPECT_EQ(0, emptyRoot.getValue());
    EXPECT_EQ(nullptr, emptyRoot.getLeft());
    EXPECT_EQ(nullptr, emptyRoot.getRight());
    EXPECT_EQ(1, emptyRoot.getHeight());

    // Test with single element
    int singleValue[] = {5};
    BinaryTreeNode singleRoot(singleValue, 1);
    EXPECT_EQ(5, singleRoot.getValue());
    EXPECT_EQ(nullptr, singleRoot.getLeft());
    EXPECT_EQ(nullptr, singleRoot.getRight());
    EXPECT_EQ(1, singleRoot.getHeight());

    // Test with unbalanced array
    int unbalancedValues[] = {1, 2, 3, 4, 5};
    BinaryTreeNode unbalancedRoot(unbalancedValues, 5);
    EXPECT_EQ(2, unbalancedRoot.getValue());
    EXPECT_EQ(1, unbalancedRoot.getLeft()->getValue());
    EXPECT_EQ(4, unbalancedRoot.getRight()->getValue());
    EXPECT_EQ(3, unbalancedRoot.getRight()->getLeft()->getValue());
    EXPECT_EQ(5, unbalancedRoot.getRight()->getRight()->getValue());
    EXPECT_EQ(3, unbalancedRoot.getHeight());
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
}