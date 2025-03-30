#ifndef BSTBOXY_H
#define BSTBOXY_H


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

struct BSTBoxyNode {
    int value = 0;
    string valueString;
    int valueWidth = 0;
    BSTBoxyNode* left = nullptr;
    BSTBoxyNode* right = nullptr;
    int width = 0;
    int height = 0;
    int boxX = 0;
    int boxW = 0;
};

BSTBoxyNode* createBoxyNode(BSTNode* bstNode);
void deleteBoxyTree(BSTBoxyNode* root);
void present(wostream& out, BSTBoxyNode* node);
void measure(BSTBoxyNode* node);

#endif