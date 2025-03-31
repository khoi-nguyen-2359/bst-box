#ifndef BSTBOX_H
#define BSTBOX_H


#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>

#include "BST.h"

using std::queue;
using std::vector;
using std::to_string;
using spdlog::debug;
using std::max;
using std::min;
using std::setw;
using std::setfill;
using std::ostream;
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

BSTBox* createBSTBox(BSTNode* bstNode);
void deleteBSTBox(BSTBox* root);
void presentBSTBox(wostream& out, BSTBox* node);

#endif