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
    char outputPath[] = "Draw_PerfectTree_2Levels.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Draw_PerfectTree_2Levels.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
}

TEST_F(BSTBoxTest, Draw_LargeValues) {
    avl = avl_create_node(1);
    avl_insert_node(&avl, 1000000000);
    avl_insert_node(&avl, 1000000001);
    avl_insert_node(&avl, -100000000);

    box = btbox_create_tree(avl);
    char outputPath[] = "Draw_LargeValues.output";
    FILE *outputFile = fopen(outputPath, "w");

    btbox_print(outputFile, box);

    string output = readFileContent(outputPath);
    string expect = readFileContent("../tree/test/btbox/Draw_LargeValues.expect");

    EXPECT_EQ(output, expect);

    fclose(outputFile);
    remove(outputPath);
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