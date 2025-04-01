#ifndef BSTBOX_H
#define BSTBOX_H

#include <iostream>

#include "AVL.h"

using std::wostream;
using std::string;

struct BSTBox {
    int value = 0;
    string valueString;
    int valueWidth = 0;
    BSTBox* left = nullptr;
    BSTBox* right = nullptr;
    int width = 0;
    int leftWidth = 0;
    int rightWidth = 0;
    int height = 0;
    int boxX = 0;
    int boxWidth = 0;
};

BSTBox* createBSTBox(AVLNode* avlNode);
void deleteBSTBox(BSTBox* root);
void presentBSTBox(wostream& out, BSTBox* node);

#endif