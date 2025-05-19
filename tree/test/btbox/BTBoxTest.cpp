#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "bt_box.h"

using std::ifstream;
using std::string;
using std::stringstream;

string readFileContent(const std::string& path);

class BSTBoxTest : public ::testing::Test {
    protected:
        BTNode* tree = nullptr;
        BTBox* box = nullptr;

        void TearDown() override {
            btbox_free_node(tree);
            btbox_free_tree(box);
        }
};

TEST_F(BSTBoxTest, Print_Valid_CompleteTree_2Levels) {
    tree = btbox_create_node(2);
    tree->left = btbox_create_node(1);
    tree->right = btbox_create_node(3);

    box = btbox_create_tree(tree);
    char outputPath[] = "Print_Valid_CompleteTree_2Levels.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Print_Valid_CompleteTree_2Levels.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
}

TEST_F(BSTBoxTest, Print_Valid_LargeValues) {
    tree = btbox_create_node(1000000000);
    tree->left = btbox_create_node(1);
    tree->right = btbox_create_node(1000000001);
    tree->left->left = btbox_create_node(-100000000);

    box = btbox_create_tree(tree);
    char outputPath[] = "Print_Valid_LargeValues.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Print_Valid_LargeValues.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
}

TEST_F(BSTBoxTest, Print_10Nodes) {
    tree = btbox_create_node(285);
    tree->left = btbox_create_node(-188048);
    tree->left->left = btbox_create_node(-90012316);
    tree->left->left->left = btbox_create_node(-366852902);
    tree->left->right = btbox_create_node(-3311);
    tree->left->right->left = btbox_create_node(-38413);
    tree->left->right->right = btbox_create_node(29);
    tree->right = btbox_create_node(8157435);
    tree->right->left = btbox_create_node(210637);
    tree->right->right = btbox_create_node(34868604);

    box = btbox_create_tree(tree);
    char outputPath[] = "Print_10Nodes.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Print_10Nodes.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
}

TEST_F(BSTBoxTest, Print_OneNodeWithLeftChild) {
    tree = btbox_create_node(60192);
    tree->left = btbox_create_node(-2314590);

    box = btbox_create_tree(tree);
    char outputPath[] = "Print_OneNodeWithLeftChild.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Print_OneNodeWithLeftChild.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
}

TEST_F(BSTBoxTest, Print_OneNodeWithRightChild) {
    tree = btbox_create_node(-229184031);
    tree->right = btbox_create_node(339);

    box = btbox_create_tree(tree);
    char outputPath[] = "Print_OneNodeWithRightChild.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Print_OneNodeWithRightChild.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_PerfectTree_2Levels) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_PerfectTree_2Levels.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 222);
    EXPECT_NE(tree->left, nullptr);
    EXPECT_EQ(tree->left->value, 111);
    EXPECT_NE(tree->right, nullptr);
    EXPECT_EQ(tree->right->value, 333);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_LargeValues) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_LargeValues.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 1000000000);
    EXPECT_NE(tree->left, nullptr);
    EXPECT_EQ(tree->left->value, 1);
    EXPECT_NE(tree->left->left, nullptr);
    EXPECT_EQ(tree->left->left->value, -100000000);
    EXPECT_NE(tree->right, nullptr);
    EXPECT_EQ(tree->right->value, 1000000001);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_Randomized10Nodes) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_Randomized10Nodes.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 2015);
    EXPECT_NE(tree->left, nullptr);
    EXPECT_EQ(tree->left->value, -362798);
    EXPECT_NE(tree->left->left, nullptr);
    EXPECT_EQ(tree->left->left->value, -103024993);
    EXPECT_NE(tree->left->right, nullptr);
    EXPECT_EQ(tree->left->right->value, 1);
    EXPECT_NE(tree->left->right->left, nullptr);
    EXPECT_EQ(tree->left->right->left->value, -3);
    EXPECT_EQ(tree->left->right->right, nullptr);
    EXPECT_NE(tree->right, nullptr);
    EXPECT_EQ(tree->right->value, 44678227);
    EXPECT_NE(tree->right->left, nullptr);
    EXPECT_EQ(tree->right->left->value, 69257);
    EXPECT_NE(tree->right->right, nullptr);
    EXPECT_EQ(tree->right->right->value, 245850917);
    EXPECT_NE(tree->right->left->right, nullptr);
    EXPECT_EQ(tree->right->left->right->value, 25774486);
}

TEST_F(BSTBoxTest, RestoreTree_Invalid_MissingRootValue) {
    const char* inputPath = "../tree/test/btbox/Restore_Invalid_MissingRootValue.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    EXPECT_EQ(tree, nullptr) << "Tree should be null for malformed input with no root";
}

TEST_F(BSTBoxTest, RestoreTree_Valid_NotUniformed) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_NotUniformed.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 1);
    EXPECT_NE(tree->left, nullptr);
    EXPECT_EQ(tree->left->value, 0);
    EXPECT_EQ(tree->left->left, nullptr);
    EXPECT_EQ(tree->left->right, nullptr);
    EXPECT_NE(tree->right, nullptr);
    EXPECT_EQ(tree->right->value, 24454);
    EXPECT_NE(tree->right->left, nullptr);
    EXPECT_EQ(tree->right->left->value, 852);
    EXPECT_NE(tree->right->right, nullptr);
    EXPECT_EQ(tree->right->right->value, 1576992);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_OneNode) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_OneNode.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 1);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_Randomized15Nodes) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_Randomized15Nodes.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);

    // Root node
    EXPECT_EQ(tree->value, -24);
    EXPECT_NE(tree->left, nullptr);
    EXPECT_NE(tree->right, nullptr);

    // Level 1
    EXPECT_EQ(tree->left->value, -117178);
    EXPECT_EQ(tree->right->value, 2679185);

    // Level 2
    EXPECT_NE(tree->left->left, nullptr);
    EXPECT_NE(tree->left->right, nullptr);
    EXPECT_NE(tree->right->left, nullptr);
    EXPECT_NE(tree->right->right, nullptr);
    EXPECT_EQ(tree->left->left->value, -16417570);
    EXPECT_EQ(tree->left->right->value, -1421);
    EXPECT_EQ(tree->right->left->value, 228);
    EXPECT_EQ(tree->right->right->value, 3810973);

    // Level 3
    EXPECT_NE(tree->left->left->left, nullptr);
    EXPECT_NE(tree->left->left->right, nullptr);
    EXPECT_NE(tree->left->right->left, nullptr);
    EXPECT_NE(tree->left->right->right, nullptr);
    EXPECT_NE(tree->right->left->left, nullptr);
    EXPECT_NE(tree->right->left->right, nullptr);
    EXPECT_NE(tree->right->right->left, nullptr);
    EXPECT_NE(tree->right->right->right, nullptr);
    EXPECT_EQ(tree->left->left->left->value, -26542876);
    EXPECT_EQ(tree->left->left->right->value, -200137);
    EXPECT_EQ(tree->left->right->left->value, -21472);
    EXPECT_EQ(tree->left->right->right->value, -1420);
    EXPECT_EQ(tree->right->left->left->value, 2);
    EXPECT_EQ(tree->right->left->right->value, 192395);
    EXPECT_EQ(tree->right->right->left->value, 2735221);
    EXPECT_EQ(tree->right->right->right->value, 474209835);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_OneNodeAndLeftChild) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_OneNodeAndLeftChild.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 1);
    EXPECT_EQ(tree->right, nullptr);
    EXPECT_NE(tree->left, nullptr);
    EXPECT_EQ(tree->left->value, 1);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_OneNodeAndRightChild) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_OneNodeAndRightChild.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 1);
    EXPECT_EQ(tree->left, nullptr);
    EXPECT_NE(tree->right, nullptr);
    EXPECT_EQ(tree->right->value, 1);
}

TEST_F(BSTBoxTest, RestoreTree_Invalid_EmptyTree) {
    const char* inputPath = "../tree/test/btbox/Restore_Invalid_EmptyTree.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    EXPECT_EQ(tree, nullptr) << "Tree should be null for empty input file";
}

TEST_F(BSTBoxTest, RestoreTree_Valid_IncompleteTree_WithRoot) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_IncompleteTree_WithRoot.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 1000000000);
    EXPECT_EQ(tree->left, nullptr);
    EXPECT_EQ(tree->right, nullptr);
}

TEST_F(BSTBoxTest, RestoreTree_Invalid_IncompleteTree_WithoutRoot) {
    const char* inputPath = "../tree/test/btbox/Restore_Invalid_IncompleteTree_WithoutRoot.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    EXPECT_EQ(tree, nullptr) << "Tree should be null for input file without root node";
}

TEST_F(BSTBoxTest, RestoreTree_Valid_IncompleteTree_2Levels) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_IncompleteTree_2Levels.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 2);
    EXPECT_EQ(tree->left, nullptr);
    EXPECT_EQ(tree->right, nullptr);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_MissingMidLevels) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_MissingMidLevels.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    tree = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->value, 2015);
    EXPECT_NE(tree->left, nullptr);
    EXPECT_EQ(tree->left->value, -103024993);
    EXPECT_NE(tree->right, nullptr);
    EXPECT_EQ(tree->right->value, 1);
    EXPECT_NE(tree->right->left, nullptr);
    EXPECT_EQ(tree->right->left->value, -3);
}

string readFileContent(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}