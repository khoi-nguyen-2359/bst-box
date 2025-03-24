#ifndef BINARY_TREE_DISPLAY_H
#define BINARY_TREE_DISPLAY_H

#include "BinaryTreeNode.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>

using std::queue;
using std::cout;
using std::vector;
using std::to_string;
using spdlog::debug;
using std::max;
using std::min;
using std::setw;
using std::setfill;
using std::ostream;
using std::string;

class BinaryTreeDisplay {
    private:
        static const int MIN_SHOULDER_LEN = 2;
        static const int MIN_VALUE_WIDTH = 3;

        int value = 0;
        string valueString;
        BinaryTreeDisplay* left = nullptr;
        BinaryTreeDisplay* right = nullptr;
        int width = 0;
        int leftWidth = 0;
        int rightWidth = 0;
        int valueWidth = 0;
        int level = 0;
        int leftShoulderLen = 0;
        int rightShoulderLen = 0;
        bool isLeft = false;
        int center = 0;

        inline int calculateValueWidth();
        void updateWidth();
    
        BinaryTreeDisplay(BinaryTreeNode* node, int level);
        void initialize(BinaryTreeNode* node, int level);
    public:
        BinaryTreeDisplay();
        BinaryTreeDisplay(BinaryTreeNode* node);
        ~BinaryTreeDisplay();

        int calculateWidth();
        int getValue() { return value; }
        BinaryTreeDisplay* getLeft() { return left; }
        BinaryTreeDisplay* getRight() { return right; }
        int getWidth() { return width; }

        void present(ostream &out = cout);
        void measure();
};

#endif