#ifndef BINARY_TREE_DISPLAY_H
#define BINARY_TREE_DISPLAY_H

#include "BinaryTreeNode.h"

#include <iostream>
#include <queue>
#include <vector>

using std::queue;
using std::cout;
using std::vector;

class BinaryTreeDisplay {
    private:
        BinaryTreeNode* node;
        BinaryTreeDisplay* left;
        BinaryTreeDisplay* right;
        int width;
        int level;

        int calculateWidth();
        int getValueWidth();

        int getLeftWidth();
        int getRightWidth();
        
    public:
        BinaryTreeDisplay(BinaryTreeNode* node, int level = 0);
        ~BinaryTreeDisplay();

        void print();
};

#endif