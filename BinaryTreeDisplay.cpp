#include "BinaryTreeDisplay.h"

BinaryTreeDisplay::BinaryTreeDisplay(BinaryTreeNode* node, int level = 0): level(level) {
    this->node = node;
    this->left = nullptr;
    this->right = nullptr;
    if (node->left) {
        left = new BinaryTreeDisplay(node->left, level + 1);
    }
    if (node->right) {
        right = new BinaryTreeDisplay(node->right, level + 1);
    }
    width = calculateWidth();
}

BinaryTreeDisplay::~BinaryTreeDisplay() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}

void BinaryTreeDisplay::print() {
    int leftWidth = left ? left->width : 0;
    for (int i = 0; i < leftWidth; i++) {
        cout << " ";
    }
    cout << node->value;
    // for (int i = 0; i < right->width; i++) {
    //     cout << " ";
    // }

    queue<BinaryTreeDisplay*> q;
    q.push(this);
    vector<BinaryTreeDisplay*> bfsOrder;

    while (!q.empty()) {
        BinaryTreeDisplay* current = q.front();
        if (current == NULL) {
            continue;
        }
        q.pop();
        bfsOrder.push_back(current);

        q.push(current->left);
        q.push(current->right);
    }

    int prevLevel = -1;
    for (auto it = bfsOrder.rbegin(); it != bfsOrder.rend(); ++it) {
        cout << (*it)->node->value << " ";
        if (prevLevel == -1 || prevLevel != (*it)->level) {
            cout << endl;
            prevLevel = (*it)->level;
        }
    }
}

int BinaryTreeDisplay::getValueWidth() {
    int valueWidth = 0;
    if (node->value < 0) {
        valueWidth = 1; // For the negative sign
    }
    int nodeValue = node->value;
    do {
        valueWidth++;
        nodeValue /= 10;
    } while (nodeValue != 0);
    return valueWidth;
}

int BinaryTreeDisplay::calculateWidth() {
    int leftWidth = 0, rightWidth = 0;
    if (node->left) {
        leftWidth = left->calculateWidth();
    }
    if (node->right) {
        rightWidth = right->calculateWidth();
    }
    
    return leftWidth + getValueWidth() + rightWidth;
}