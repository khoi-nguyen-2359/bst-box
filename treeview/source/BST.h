#ifndef BST_H
#define BST_H


#include <iostream>
#include <spdlog/spdlog.h>

using std::cout;
using std::endl;
using spdlog::debug;
using std::vector;

struct BSTNode {
    int value = 0;
    int height = 1;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;
};

// Function declarations for working with BSTNode
BSTNode* createBSTNode(int value);
BSTNode* createBSTNode(int* values, int len);
void deleteBSTNode(BSTNode* root);

bool insertBSTNode(BSTNode* root, int value);
bool removeBSTNode(BSTNode*& root, int value);

#endif