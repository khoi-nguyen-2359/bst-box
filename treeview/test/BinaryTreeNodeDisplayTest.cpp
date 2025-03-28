#include <gtest/gtest.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "../source/BinaryTreeDisplay.h"
#include "../source/BinaryTreeNode.h"

TEST(BinaryTreeDisplayTest, ConstructorWithNode_NodesConstructedAtCorrectPositions) {
    BinaryTreeNode node(3);
    node.insert(2);
    node.insert(4);
    node.insert(1);
    node.insert(5);
    node.insert(6);
    node.insert(0);
    node.insert(7);
    node.insert(8);

    BinaryTreeDisplay display(&node);
    EXPECT_EQ(display.getValue(), 3);
    EXPECT_EQ(display.getLeft()->getValue(), 1);
    EXPECT_EQ(display.getRight()->getValue(), 5);
    EXPECT_EQ(display.getLeft()->getLeft()->getValue(), 0);
    EXPECT_EQ(display.getLeft()->getRight()->getValue(), 2);
    EXPECT_EQ(display.getRight()->getRight()->getValue(), 7);
    EXPECT_EQ(display.getRight()->getLeft()->getValue(), 4);
    EXPECT_EQ(display.getLeft()->getLeft()->getLeft(), nullptr);
    EXPECT_EQ(display.getRight()->getRight()->getRight()->getValue(), 8);
    EXPECT_EQ(display.getRight()->getRight()->getRight()->getRight(), nullptr);
    EXPECT_EQ(display.getRight()->getRight()->getLeft()->getValue(), 6);
}

TEST(BinaryTreeDisplayTest, CalculateWidth_TwoEqualSubtrees_CalculatesAccurateWidths) {
    BinaryTreeNode node(3);
    node.insert(2);
    node.insert(4);
    BinaryTreeDisplay display(&node);
    EXPECT_EQ(display.getWidth(), 5);
    EXPECT_EQ(display.getLeft()->getWidth(), 1);
    EXPECT_EQ(display.getRight()->getWidth(), 1);
}

TEST(BinaryTreeDisplayTest, CalculateWidth_TwoUnequalSubtrees_CalculatesAccurateWidths) {
    BinaryTreeNode node(3);
    node.insert(2);
    node.insert(444);
    BinaryTreeDisplay display(&node);
    EXPECT_EQ(display.getWidth(), 6);
    EXPECT_EQ(display.getLeft()->getWidth(), 1);
    EXPECT_EQ(display.getRight()->getWidth(), 3);
}

TEST(BinaryTreeDisplayTest, CalculateWidth_ThreeLevels_CalculatesAccurateWidths) {
    BinaryTreeNode node(3);
    node.insert(2);
    node.insert(1);
    node.insert(0);
    node.insert(444);
    BinaryTreeDisplay display(&node);
    EXPECT_EQ(display.getWidth(), 10);
    EXPECT_EQ(display.getLeft()->getWidth(), 3);
    EXPECT_EQ(display.getRight()->getWidth(), 4);
    EXPECT_EQ(display.getLeft()->getLeft()->getWidth(), 1);
}

TEST(BinaryTreeDisplayTest, CalculateWidth_ComplexTree_CalculatesAccurateWidths) {
    BinaryTreeNode node(111);
    node.insert(1);
    node.insert(33333);
    node.insert(0); 
    node.insert(100);
    node.insert(333);
    node.insert(44444);
    node.insert(222);
    node.insert(22222);
    // node.insert(55555);

    BinaryTreeDisplay display(&node);
    display.calculateWidth();
    
    EXPECT_EQ(display.getWidth(), 37);
    EXPECT_EQ(display.getLeft()->getWidth(), 14);
    EXPECT_EQ(display.getRight()->getWidth(), 19);
    EXPECT_EQ(display.getLeft()->getLeft()->getWidth(), 1);
    EXPECT_EQ(display.getLeft()->getRight()->getWidth(), 3);
    EXPECT_EQ(display.getRight()->getLeft()->getWidth(), 10);
    EXPECT_EQ(display.getRight()->getRight()->getWidth(), 5);
    EXPECT_EQ(display.getRight()->getLeft()->getLeft()->getWidth(), 3);
    EXPECT_EQ(display.getRight()->getLeft()->getRight()->getWidth(), 5);
}

TEST(BinaryTreeDisplayTest, CalculateWidth_CompleteFourLevelTree_CalculatesAccurateWidths) {
    BinaryTreeNode node(8);
    // Level 2
    node.insert(4);
    node.insert(12);
    // Level 3
    node.insert(2);
    node.insert(6);
    node.insert(10);
    node.insert(14);
    // Level 4
    node.insert(1);
    node.insert(3);
    node.insert(5);
    node.insert(7);
    node.insert(9);
    node.insert(11);
    node.insert(13);
    node.insert(15);

    BinaryTreeDisplay display(&node);
    display.calculateWidth();

    // Test root level
    EXPECT_EQ(display.getWidth(), 31);
    
    // Test level 2
    EXPECT_EQ(display.getLeft()->getWidth(), 14);
    EXPECT_EQ(display.getRight()->getWidth(), 14);
    
    // Test level 3 left subtree
    EXPECT_EQ(display.getLeft()->getLeft()->getWidth(), 6);
    EXPECT_EQ(display.getLeft()->getRight()->getWidth(), 6);
    
    // Test level 3 right subtree
    EXPECT_EQ(display.getRight()->getLeft()->getWidth(), 6);
    EXPECT_EQ(display.getRight()->getRight()->getWidth(), 6);
    
    // Test some level 4 nodes
    EXPECT_EQ(display.getLeft()->getLeft()->getLeft()->getWidth(), 1);
    EXPECT_EQ(display.getRight()->getRight()->getRight()->getWidth(), 3);
}

TEST(BinaryTreeDisplayTest, CalculateWidth_ThreeLevelsWithFourDigits_CalculatesAccurateWidths) {
    BinaryTreeNode node(1111);
    node.insert(2222);  // Left child of root
    node.insert(3333);  // Right child of root
    node.insert(4444);  // Right child of 3333
    
    BinaryTreeDisplay display(&node);
    display.calculateWidth();
    
    EXPECT_EQ(display.getWidth(), 14);
    EXPECT_EQ(display.getLeft()->getWidth(), 5);
    EXPECT_EQ(display.getRight()->getWidth(), 8);
    EXPECT_EQ(display.getRight()->getRight()->getWidth(), 5);
}

TEST(BinaryTreeDisplayTest, CalculateWidth_LongValueNumbersWithShortSubtrees_CalculatesAccurateWidths) {
    BinaryTreeNode node(555555);
    node.insert(33333);
    node.insert(888888);
    node.insert(1);    
    
    BinaryTreeDisplay display(&node);
    display.calculateWidth();
    
    EXPECT_EQ(display.getWidth(), 14); 
    EXPECT_EQ(display.getLeft()->getWidth(), 5); 
    EXPECT_EQ(display.getRight()->getWidth(), 7);
    EXPECT_EQ(display.getLeft()->getLeft()->getWidth(), 1);
}
