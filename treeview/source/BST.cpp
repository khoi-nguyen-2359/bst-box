#include <algorithm>
#include <iostream>
#include <spdlog/spdlog.h>

#include "BST.h"

using std::max;
using spdlog::debug;

void updateHeight(BSTNode* node);
int getHeight(BSTNode* node);

void balance(BSTNode* root);
inline void rotateLeft(BSTNode* root);
inline void rotateRight(BSTNode* root);

int removeMax(BSTNode* parent, BSTNode* node);

BSTNode* createBSTNode(int value) {
    BSTNode* node = new BSTNode;
    node->value = value;
    node->height = 1;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

BSTNode* createBSTNode(int* values, int len) {
    if (len == 0) {
        return nullptr;
    }
    BSTNode* root = createBSTNode(values[0]);
    for (int i = 1; i < len; i++) {
        insertBSTNode(root, values[i]);
    }
    return root;
}

void deleteBSTNode(BSTNode* root) {
    if (!root) return;
    deleteBSTNode(root->left);
    deleteBSTNode(root->right);
    delete root;
}

int getHeight(BSTNode* node) {
    return node ? node->height : 0;
}

void updateHeight(BSTNode* node) {
    if (node) {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }
}

inline void rotateLeft(BSTNode* root) {
    debug("Rotating left at node " + std::to_string(root->value));
    BSTNode* newLeft = createBSTNode(root->value);
    newLeft->left = root->left;
    newLeft->right = root->right->left;
    root->left = newLeft;
    root->value = root->right->value;
    BSTNode* temp = root->right;
    root->right = root->right->right;
    delete temp;
}

inline void rotateRight(BSTNode* root) {
    debug("Rotating right at node " + std::to_string(root->value));
    BSTNode* newRight = createBSTNode(root->value);
    newRight->right = root->right;
    newRight->left = root->left->right;
    root->right = newRight;
    root->value = root->left->value;
    BSTNode* temp = root->left;
    root->left = root->left->left;
    delete temp;
}

void balance(BSTNode* root) {
    int heightDiff = getHeight(root->left) - getHeight(root->right);
    if (heightDiff >= -1 && heightDiff <= 1) {
        return;
    }
    debug("Balancing at node " + std::to_string(root->value) + ":");
    debug("    left: " + std::to_string(getHeight(root->left)));
    debug("    right: " + std::to_string(getHeight(root->right)));

    if (heightDiff > 1) {
        if (getHeight(root->left->right) > getHeight(root->left->left)) {
            rotateLeft(root->left);
        }
        rotateRight(root);
    } else if (heightDiff < -1) {
        if (getHeight(root->right->left) > getHeight(root->right->right)) {
            rotateRight(root->right);
        }
        rotateLeft(root);
    }

    updateHeight(root->left);
    updateHeight(root->right);
    updateHeight(root);
}

bool insertBSTNode(BSTNode* root, int value) {
    if (value == root->value) {
        return false; // Duplicate value
    }
    
    bool inserted = false;
    if (value < root->value) {
        if (root->left) {
            inserted = insertBSTNode(root->left, value);
        } else {
            root->left = createBSTNode(value);
            inserted = true;
        }
    } else if (value > root->value) {
        if (root->right) {
            inserted = insertBSTNode(root->right, value);
        } else {
            root->right = createBSTNode(value);
            inserted = true;
        }
    }

    if (inserted) {
        updateHeight(root);
        balance(root);
    }
    return inserted;
}

void replace(BSTNode* dest, BSTNode* src) {
    BSTNode* tempLeft = dest->left;
    BSTNode* tempRight = dest->right;
    dest->value = src->value;
    dest->left = src->left;
    dest->right = src->right;
    delete src;
    delete tempLeft;
    delete tempRight;
}

bool removeBSTNode(BSTNode*& root, int value) {
    debug("Removing " + std::to_string(value) + " from node " + std::to_string(root->value));
    bool removed = false;
    if (value < root->value) {
        removed = root->left ? removeBSTNode(root->left, value) : false;
    } else if (value > root->value) {
        removed = root->right ? removeBSTNode(root->right, value) : false;
    } else {
        // Node found
        if (!root->left && !root->right) {
            delete root;
            root = nullptr;
        } else if (!root->left) {  // move up the right child
            root->value = root->right->value;
            BSTNode* tempRight = root->right;
            root->right = root->right->right;
            delete tempRight;
        } else if (root->left->right) {
            int removedValue = removeMax(root->left, root->left->right);
            root->value = removedValue;
        } else {    // move up the left child
            root->value = root->left->value;
            BSTNode* tempLeft = root->left;
            root->left = root->left->left;
            delete tempLeft;
        }
        removed = true;
        debug("Removed " + std::to_string(value));
    }

    if (removed && root) {
        updateHeight(root);
        balance(root);
    }
    return removed;
}

int removeMax(BSTNode* parent, BSTNode* current) {
    int removedValue = 0;
    if (current->right) {
        removedValue = removeMax(current, current->right);
    } else {
        removedValue = current->value;
        BSTNode* currentLeft = current->left;
        spdlog::debug("Removing max " + std::to_string(removedValue));
        delete current;
        parent->right = currentLeft;
    }

    updateHeight(parent);
    balance(parent);

    return removedValue;
}

void printInOrder(BSTNode* root) {
    if (!root) return;
    printInOrder(root->left);
    std::cout << root->value << " ";
    printInOrder(root->right);
}