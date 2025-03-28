#include "BinaryTreeNode.h"

BinaryTreeNode::BinaryTreeNode() : BinaryTreeNode(-1) {}

BinaryTreeNode::BinaryTreeNode(const int value) : value(value) {}

BinaryTreeNode::BinaryTreeNode(const int value, const int height) : value(value), height(height) {}

BinaryTreeNode::BinaryTreeNode(const int values[], const int size) {
    if (size == 0) {
        return;
    }
    this->value = values[0];
    for (int i = 1; i < size; i++) {
        debug("Inserting array value " + std::to_string(values[i]));
        insert(values[i]);
    }
}

BinaryTreeNode::BinaryTreeNode(const vector<int>& values) {
    if (values.empty()) {
        return;
    }
    this->value = values[0];
    for (size_t i = 1; i < values.size(); i++) {
        debug("Inserting vector value " + std::to_string(values[i]));
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

int BinaryTreeNode::removeMax(BinaryTreeNode* parent, BinaryTreeNode* current) {
    int removedValue = 0;
    if (current->right) {
        removedValue = removeMax(current, current->right);
        current->updateHeight();
        current->balance();
    } else {
        removedValue = current->value;
        parent->right = current->left;
        current->left = nullptr;
        current->right = nullptr;
        spdlog::debug("Removing max " + std::to_string(removedValue));
        delete current;
    }
    return removedValue;
}

int BinaryTreeNode::removeMin(BinaryTreeNode* parent, BinaryTreeNode* current) {
    int removedValue = 0;
    if (current->left) {
        removedValue = removeMin(current, current->left);
        current->updateHeight();
        current->balance();
    } else {
        removedValue = current->value;
        if (parent) {
            parent->left = current->right;
        }
        current->right = nullptr;
        current->left = nullptr;
        spdlog::debug("Removing min " + std::to_string(removedValue));
        delete current;
    }
    return removedValue;
}

bool BinaryTreeNode::remove(int value) {
    bool removed = false;
    if (value < this->value) {
        if (left) {
            removed = left->remove(value);
        }
    } else if (value > this->value) {
        if (right) {
            removed = right->remove(value);
        }
    } else {
        // Node found
        if ((left ? left->height : 0) > (right ? right->height : 0)) {
            if (left->right) {
                int removedValue = removeMax(left, left->right);
                this->value = removedValue;
            } else {
                this->value = left->value;
                BinaryTreeNode* temp = left;
                left = left->left;
                temp->left = nullptr;
                temp->right = nullptr;
                delete temp;
            }
        } else if (right) {
            if (right->left) {
                int removedValue = removeMin(right, right->left);
                this->value = removedValue;
            } else {
                this->value = right->value;
                BinaryTreeNode* temp = right;
                right = right->right;
                temp->left = nullptr;
                temp->right = nullptr;
                delete temp;
            }
        } else {
            // No children
            delete this;
        }
        removed = true;
    }
    if (removed) {
        updateHeight();
        balance();
    }
    return removed;
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