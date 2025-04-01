#include <iostream>
#include <spdlog/spdlog.h>

#include "AVL.h"

using std::max;
using std::cout;
using std::endl;
using spdlog::debug;
using std::vector;

AVLNode* createAVLNode(int value) {
    AVLNode* node = new AVLNode();
    node->value = value;
    return node;
}

void updateHeight(AVLNode* node);
int getHeight(AVLNode* node);

void balance(AVLNode* root);
inline void rotateLeft(AVLNode* root);
inline void rotateRight(AVLNode* root);

int removeMax(AVLNode* parent, AVLNode* node);

void deleteAVLNode(AVLNode*& root) {
    if (!root) return;
    deleteAVLNode(root->left);
    deleteAVLNode(root->right);
    delete root;
}

AVLNode* createAVLTree(const int* values, int len) {
    AVLNode* root = nullptr;
    insertAVLNodes(root, values, len);
    return root;
}

void insertAVLNodes(AVLNode*& root, const int* values, int len) {
    for (int i = 0; i < len; ++i) {
        insertAVLNode(root, values[i]);
    }
}

int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

void updateHeight(AVLNode* node) {
    if (node) {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }
}

inline void rotateLeft(AVLNode* root) {
    debug("Rotating left at node " + std::to_string(root->value));
    AVLNode* newLeft = createAVLNode(root->value);
    newLeft->left = root->left;
    newLeft->right = root->right->left;
    root->left = newLeft;
    root->value = root->right->value;
    AVLNode* temp = root->right;
    root->right = root->right->right;
    delete temp;
}

inline void rotateRight(AVLNode* root) {
    debug("Rotating right at node " + std::to_string(root->value));
    AVLNode* newRight = createAVLNode(root->value);
    newRight->right = root->right;
    newRight->left = root->left->right;
    root->right = newRight;
    root->value = root->left->value;
    AVLNode* temp = root->left;
    root->left = root->left->left;
    delete temp;
}

void balance(AVLNode* root) {
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

bool insertAVLNode(AVLNode*& root, int value) {
    if (!root) {
        root = createAVLNode(value);
    }
    if (value == root->value) {
        return false; // Duplicate value
    }
    
    bool inserted = false;
    if (value < root->value) {
        if (root->left) {
            inserted = insertAVLNode(root->left, value);
        } else {
            root->left = createAVLNode(value);
            inserted = true;
        }
    } else if (value > root->value) {
        if (root->right) {
            inserted = insertAVLNode(root->right, value);
        } else {
            root->right = createAVLNode(value);
            inserted = true;
        }
    }

    if (inserted) {
        updateHeight(root);
        balance(root);
    }
    return inserted;
}

void replace(AVLNode* dest, AVLNode* src) {
    AVLNode* tempLeft = dest->left;
    AVLNode* tempRight = dest->right;
    dest->value = src->value;
    dest->left = src->left;
    dest->right = src->right;
    delete src;
    delete tempLeft;
    delete tempRight;
}

bool removeAVLNode(AVLNode*& root, int value) {
    debug("Removing " + std::to_string(value) + " from node " + std::to_string(root->value));
    bool removed = false;
    if (value < root->value) {
        removed = root->left ? removeAVLNode(root->left, value) : false;
    } else if (value > root->value) {
        removed = root->right ? removeAVLNode(root->right, value) : false;
    } else {
        // Node found
        if (!root->left && !root->right) {
            delete root;
            root = nullptr;
        } else if (!root->left) {  // move up the right child
            root->value = root->right->value;
            AVLNode* tempRight = root->right;
            root->right = root->right->right;
            delete tempRight;
        } else if (root->left->right) {
            int removedValue = removeMax(root->left, root->left->right);
            root->value = removedValue;
        } else {    // move up the left child
            root->value = root->left->value;
            AVLNode* tempLeft = root->left;
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

int removeMax(AVLNode* parent, AVLNode* current) {
    int removedValue = 0;
    if (current->right) {
        removedValue = removeMax(current, current->right);
    } else {
        removedValue = current->value;
        AVLNode* currentLeft = current->left;
        spdlog::debug("Removing max " + std::to_string(removedValue));
        delete current;
        parent->right = currentLeft;
    }

    updateHeight(parent);
    balance(parent);

    return removedValue;
}

void printInOrder(AVLNode* root) {
    if (!root) return;
    printInOrder(root->left);
    std::cout << root->value << " ";
    printInOrder(root->right);
}