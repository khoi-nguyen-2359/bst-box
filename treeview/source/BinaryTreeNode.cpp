#include "BinaryTreeNode.h"

BinaryTreeNode::BinaryTreeNode() : value(0), left(nullptr), right(nullptr) {}

BinaryTreeNode::BinaryTreeNode(int value) : value(value), left(nullptr), right(nullptr) {}

BinaryTreeNode::~BinaryTreeNode() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}

void BinaryTreeNode::insert(int value) {
    BinaryTreeNode* insertNode = new BinaryTreeNode(value);
    insert(insertNode);
}

void BinaryTreeNode::insert(BinaryTreeNode* node) {
    if (node->value < this->value) {
        if (left) {
            left->insert(node);
        } else {
            left = node;
        }
    } else {
        if (right) {
            right->insert(node);
        } else {
            right = node;
        }
    }
}
