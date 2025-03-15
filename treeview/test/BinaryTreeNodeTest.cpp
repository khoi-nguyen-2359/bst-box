#include <gtest/gtest.h>
#define UNIT_TEST 1
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

TEST(BinaryTreeNodeTest, Insert_BalancingWithSingleRightRotation) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(3);
    root.insert(7);
    root.insert(12);
    root.insert(18);
    root.insert(1);
    root.insert(0);  // This should trigger balancing

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

TEST(BinaryTreeNodeTest, Insert_BalancingWithDoubleRotationLeftRight) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(3);
    root.insert(7);
    root.insert(6);  // This should trigger double rotation (left-right)

    EXPECT_EQ(7, root.getValue());
    EXPECT_EQ(5, root.getLeft()->getValue());
    EXPECT_EQ(10, root.getRight()->getValue());
    EXPECT_EQ(nullptr, root.getRight()->getLeft());
    EXPECT_EQ(3, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(6, root.getLeft()->getRight()->getValue());
    EXPECT_EQ(15, root.getRight()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_BalancingWithSingleLeftRotation) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(12);
    root.insert(18);
    root.insert(20);  // This should trigger single left rotation

    EXPECT_EQ(15, root.getValue());
    EXPECT_EQ(10, root.getLeft()->getValue());
    EXPECT_EQ(18, root.getRight()->getValue());
    EXPECT_EQ(nullptr, root.getRight()->getLeft());
    EXPECT_EQ(20, root.getRight()->getRight()->getValue());
    EXPECT_EQ(5, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(12, root.getLeft()->getRight()->getValue());
}

TEST(BinaryTreeNodeTest, Insert_BalancingWithDoubleRotationRightLeft) {
    BinaryTreeNode root(10);
    root.insert(5);
    root.insert(15);
    root.insert(18);
    root.insert(12);
    root.insert(13);  // This should trigger double rotation (right-left)

    EXPECT_EQ(12, root.getValue());
    EXPECT_EQ(10, root.getLeft()->getValue());
    EXPECT_EQ(15, root.getRight()->getValue());
    EXPECT_EQ(5, root.getLeft()->getLeft()->getValue());
    EXPECT_EQ(nullptr, root.getLeft()->getRight());
    EXPECT_EQ(13, root.getRight()->getLeft()->getValue());
    EXPECT_EQ(18, root.getRight()->getRight()->getValue());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}