#include <gtest/gtest.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "../source/BinaryTreeDisplay.h"
#include "../source/BinaryTreeNode.h"

TEST(BinaryTreeDisplayTest, ConstructorWithValue_TreeCorrectlyConstructed) {
    BinaryTreeNode node(3);
    node.insert(2);
    node.insert(4);
    node.insert(1);
    node.insert(5);
    BinaryTreeDisplay display(&node);
    EXPECT_EQ(3, display.getNode()->getValue());
    EXPECT_EQ(2, display.getLeft()->getNode()->getValue());
    EXPECT_EQ(4, display.getRight()->getNode()->getValue());
    EXPECT_EQ(1, display.getLeft()->getLeft()->getNode()->getValue());
    EXPECT_EQ(5, display.getRight()->getRight()->getNode()->getValue());
}

// void initLogging();

// int main(int argc, char **argv) {
//     testing::InitGoogleTest(&argc, argv);
//     initLogging();
//     return RUN_ALL_TESTS();
// }

// void initLogging() {
//   spdlog::set_level(spdlog::level::debug);
//   auto file_logger = spdlog::basic_logger_mt("BinaryTreeDisplayTest", "BinaryTreeDisplayTestLogs.txt");
//   spdlog::set_default_logger(file_logger);
// }