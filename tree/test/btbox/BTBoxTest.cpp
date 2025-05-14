#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "avl_tree.h"
#include "bt_box.h"

using std::ifstream;
using std::string;
using std::stringstream;

string readFileContent(const std::string& path);

class BSTBoxTest : public ::testing::Test {
    protected:
        AVLNode* avl = nullptr;
        BTBox* box = nullptr;

        void TearDown() override {
            avl_free_tree(&avl);
            btbox_free_tree(box);
        }
};

TEST_F(BSTBoxTest, Draw_PerfectTree_2Levels) {
    avl = avl_create_node(1);
    avl_insert_node(&avl, 2);
    avl_insert_node(&avl, 3);

    box = btbox_create_tree(avl);
    char outputPath[] = "Draw_Valid_PerfectTree_2Levels.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Draw_Valid_PerfectTree_2Levels.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
}

TEST_F(BSTBoxTest, Draw_Valid_LargeValues) {
    avl = avl_create_node(1);
    avl_insert_node(&avl, 1000000000);
    avl_insert_node(&avl, 1000000001);
    avl_insert_node(&avl, -100000000);

    box = btbox_create_tree(avl);
    char outputPath[] = "Draw_Valid_LargeValues.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Draw_Valid_LargeValues.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_PerfectTree_2Levels) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_PerfectTree_2Levels.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    avl = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(avl, nullptr);
    EXPECT_EQ(avl->value, 222);
    EXPECT_NE(avl->left, nullptr);
    EXPECT_EQ(avl->left->value, 111);
    EXPECT_NE(avl->right, nullptr);
    EXPECT_EQ(avl->right->value, 333);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_LargeValues) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_LargeValues.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    avl = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(avl, nullptr);
    EXPECT_EQ(avl->value, 1000000000);
    EXPECT_NE(avl->left, nullptr);
    EXPECT_EQ(avl->left->value, 1);
    EXPECT_NE(avl->left->left, nullptr);
    EXPECT_EQ(avl->left->left->value, -100000000);
    EXPECT_NE(avl->right, nullptr);
    EXPECT_EQ(avl->right->value, 1000000001);
}

TEST_F(BSTBoxTest, RestoreTree_Valid_Randomized10Nodes) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_Randomized10Nodes.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    avl = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(avl, nullptr);
    EXPECT_EQ(avl->value, 2015);
    EXPECT_NE(avl->left, nullptr);
    EXPECT_EQ(avl->left->value, -362798);
    EXPECT_NE(avl->left->left, nullptr);
    EXPECT_EQ(avl->left->left->value, -103024993);
    EXPECT_NE(avl->left->right, nullptr);
    EXPECT_EQ(avl->left->right->value, 1);
    EXPECT_NE(avl->left->right->left, nullptr);
    EXPECT_EQ(avl->left->right->left->value, -3);
    EXPECT_EQ(avl->left->right->right, nullptr);
    EXPECT_NE(avl->right, nullptr);
    EXPECT_EQ(avl->right->value, 44678227);
    EXPECT_NE(avl->right->left, nullptr);
    EXPECT_EQ(avl->right->left->value, 69257);
    EXPECT_NE(avl->right->right, nullptr);
    EXPECT_EQ(avl->right->right->value, 245850917);
    EXPECT_NE(avl->right->left->right, nullptr);
    EXPECT_EQ(avl->right->left->right->value, 25774486);
}

TEST_F(BSTBoxTest, RestoreTree_Invalid_MissingRootValue) {
    const char* inputPath = "../tree/test/btbox/Restore_Invalid_MissingRootValue.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    avl = btbox_restore_tree(inputFile);
    fclose(inputFile);

    EXPECT_EQ(avl, nullptr) << "Tree should be null for malformed input with no root";
}

TEST_F(BSTBoxTest, RestoreTree_Valid_NotUniformed) {
    const char* inputPath = "../tree/test/btbox/Restore_Valid_NotUniformed.input";

    FILE* inputFile = fopen(inputPath, "r");
    ASSERT_NE(inputFile, nullptr) << "Failed to open input file";

    avl = btbox_restore_tree(inputFile);
    fclose(inputFile);

    ASSERT_NE(avl, nullptr);
    EXPECT_EQ(avl->value, 1);
    EXPECT_NE(avl->left, nullptr);
    EXPECT_EQ(avl->left->value, 0);
    EXPECT_EQ(avl->left->left, nullptr);
    EXPECT_EQ(avl->left->right, nullptr);
    EXPECT_NE(avl->right, nullptr);
    EXPECT_EQ(avl->right->value, 24454);
    EXPECT_NE(avl->right->left, nullptr);
    EXPECT_EQ(avl->right->left->value, 852);
    EXPECT_NE(avl->right->right, nullptr);
    EXPECT_EQ(avl->right->right->value, 1576992);
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