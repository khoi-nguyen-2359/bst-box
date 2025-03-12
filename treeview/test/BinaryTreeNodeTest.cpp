#include <gtest/gtest.h>
#include "../source/BinaryTreeNode.h"

TEST(BinaryTreeNodeTest, Constructor) {
    BinaryTreeNode node(3);
    EXPECT_EQ(3, node.getValue());
    EXPECT_EQ(nullptr, node.getLeft());
    EXPECT_EQ(nullptr, node.getRight());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}