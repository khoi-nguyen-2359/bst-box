#include "BinaryTreeNode.h"

BinaryTreeNode::BinaryTreeNode() : BinaryTreeNode(-1) {}

BinaryTreeNode::BinaryTreeNode(int value) : value(value), height(1), left(nullptr), right(nullptr) {}

BinaryTreeNode::BinaryTreeNode(int value, int height) : value(value), height(height), left(nullptr), right(nullptr) {}

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

bool BinaryTreeNode::insert(BinaryTreeNode* node) {
    if (node->value == this->value) {
        delete node;
        return false;
    }

    bool inserted = false;
    if (node->value < this->value) {
        if (left) {
            inserted = left->insert(node);
        } else {
            left = node;
            inserted = true;
        }
    } else if (node->value > this->value) {
        if (right) {
            inserted = right->insert(node);
        } else {
            right = node;
            inserted = true;
        }
    }

    if (inserted) {
        updateHeight();
        int oldNodeVal = this->value;
        bool exec = balance();
        if (exec) {
            spdlog::debug("Balanced at: " + std::to_string(oldNodeVal) + " when inserting: " + std::to_string(node->value));
            updateHeight();
        }
    }

    return inserted;
}

void BinaryTreeNode::updateHeight() {
    this->height = std::max(getLeftHeight(), getRightHeight()) + 1;
}

bool BinaryTreeNode::balance() {
    int heightDiff = getLeftHeight() - getRightHeight();
    if (heightDiff >= -1 && heightDiff <= 1) {
        return false;
    }

    if (heightDiff < -1) {  // left is shorter, rotate left
        if (this->right->getLeftHeight() > this->right->getRightHeight()) {
            right->rotateRight();
            rotateLeft();
        } else {
            rotateLeft();
        }
    } else if (heightDiff > 1) {  // right is shorter, rotate right
        if (this->left->getRightHeight() > this->left->getLeftHeight()) {
            left->rotateLeft();
            rotateRight();
        } else {
            rotateRight();
        }
    }

    return true;
}

void BinaryTreeNode::rotateLeft() {
    spdlog::debug("Rotated left " + std::to_string(this->value));
    BinaryTreeNode* newLeft = new BinaryTreeNode(value);
    newLeft->left = this->left;
    newLeft->right = this->right->left;
    this->value = this->right->value;
    this->left = newLeft;
    this->right = this->right->right;
}

void BinaryTreeNode::rotateRight() {
    spdlog::debug("Rotated right " + std::to_string(this->value));
    BinaryTreeNode* newRight = new BinaryTreeNode(value);
    newRight->right = this->right;
    newRight->left = this->left->right;
    this->value = this->left->value;
    this->right = newRight;
    this->left = this->left->left;
}