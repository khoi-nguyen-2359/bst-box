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
BSTNode* createNode(int value);
BSTNode* createNode(int* values, int len);
void deleteTree(BSTNode* root);

bool insert(BSTNode* root, int value);
bool remove(BSTNode*& root, int value);

#endif