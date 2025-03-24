#include "BinaryTreeNode.h"

BinaryTreeNode::BinaryTreeNode() : BinaryTreeNode(-1) {}

BinaryTreeNode::BinaryTreeNode(const int value) : value(value) {}

BinaryTreeNode::BinaryTreeNode(const int value, const int height) : value(value), height(height) {}

BinaryTreeNode::BinaryTreeNode(const int values[], const int length) {
    if (length == 0) {
        return;
    }
    this->value = values[0];
    for (int i = 1; i < length; i++) {
        debug("Inserting array value " + std::to_string(values[i]));
        insert(values[i]);
    }
}

BinaryTreeNode::BinaryTreeNode(const BinaryTreeNode& node) {
    copy(node);
}

BinaryTreeNode& BinaryTreeNode::operator=(const BinaryTreeNode& node) {
    return copy(node);
}

BinaryTreeNode& BinaryTreeNode::copy(const BinaryTreeNode& node) {
    this->value = node.value;
    this->height = node.height;
    if (node.left) {
        this->left = new BinaryTreeNode(*node.left);
    }
    if (node.right) {
        this->right = new BinaryTreeNode(*node.right);
    }
    return *this;
}

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
    debug("Inserting by value " + std::to_string(value));
    if (!insert(insertNode)) {
        delete insertNode;
    }
}

bool BinaryTreeNode::insert(BinaryTreeNode* node) {
    if (node->value == this->value) {
        return false;
    }

    bool inserted = false;
    if (node->value < this->value) {
        debug("Inserting left " + std::to_string(node->value));
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
        debug("Value inserted " + std::to_string(node->value));
        updateHeight();
        int oldNodeVal = this->value;
        bool exec = balance();
        if (exec) {
            spdlog::debug("Balanced at: " + std::to_string(oldNodeVal) + " when inserting: " + std::to_string(node->value));
        }
    }

    return inserted;
}

int BinaryTreeNode::updateHeight() {
    return height = std::max(getLeftHeight(), getRightHeight()) + 1;
}

void BinaryTreeNode::calculateHeights() {
    if (left) {
        left->calculateHeights();
    }
    if (right) {
        right->calculateHeights();
    }
    updateHeight();
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
    left ? left->updateHeight() : 0;
    right ? right->updateHeight() : 0;
    updateHeight();

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