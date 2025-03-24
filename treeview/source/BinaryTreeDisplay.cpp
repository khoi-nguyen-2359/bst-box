#include "BinaryTreeDisplay.h"

#include <queue>
#include <iostream>
#include <iomanip>
#include <cmath>

const int BinaryTreeDisplay::MIN_SHOULDER_LEN;
const int BinaryTreeDisplay::MIN_VALUE_WIDTH;

BinaryTreeDisplay::BinaryTreeDisplay(): BinaryTreeDisplay(nullptr, 0) {}

BinaryTreeDisplay::BinaryTreeDisplay(BinaryTreeNode* root) {
    initialize(root, 0);
}

BinaryTreeDisplay::BinaryTreeDisplay(BinaryTreeNode* node, int level): level(level) {
    initialize(node, level);
}

void BinaryTreeDisplay::initialize(BinaryTreeNode* node, int level) {
    this->value = node->value;
    calculateValueWidth();
    if (node->left) {
        left = new BinaryTreeDisplay(node->left, level + 1);
        left->isLeft = true;
    }
    if (node->right) {
        right = new BinaryTreeDisplay(node->right, level + 1);
    }
    updateWidth();
    debug("Init (node, level) = " + to_string(node->getValue()) + ", " + to_string(level));
}

BinaryTreeDisplay::~BinaryTreeDisplay() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}

void BinaryTreeDisplay::updateWidth() {
    leftShoulderLen = 0;
    if (left) {
        leftShoulderLen = max(MIN_SHOULDER_LEN, left->rightWidth + 1 - valueWidth / 2);
    }

    rightShoulderLen = 0;
    if (right) {
        rightShoulderLen = max(MIN_SHOULDER_LEN, right->leftWidth + 1 - valueWidth / 2);
    }

    leftWidth = leftShoulderLen + valueWidth / 2 + (left ? left->leftWidth : 0);
    rightWidth = rightShoulderLen + valueWidth / 2 + (right ? right->rightWidth : 0);

    width = leftWidth + rightWidth + 1;

    debug("node " + to_string(value) + " level " + to_string(level));
    debug("    width " + to_string(width));
    debug("    valueWidth " + to_string(valueWidth));
    debug("    left shoulder " + to_string(leftShoulderLen));
    debug("    right shoulder " + to_string(rightShoulderLen));
    debug("    leftWidth " + to_string(leftWidth));
    debug("    rightWidth " + to_string(rightWidth));
}

int BinaryTreeDisplay::calculateWidth() {
    left ? left->calculateWidth() : 0;
    right ? right->calculateWidth() : 0;
    updateWidth();
    return width;
}

int BinaryTreeDisplay::calculateValueWidth() {
    valueWidth = 1;
    int i = abs(this->value);
    while (i >= 10) {
        i /= 10;
        valueWidth++;
    }
    if (this->value < 0) {
        valueWidth++;
    }
    if (valueWidth % 2 == 0) {
        valueWidth++;
    }
    return valueWidth = max(valueWidth, MIN_VALUE_WIDTH);
}

void BinaryTreeDisplay::present(ostream &out) {
    queue<BinaryTreeDisplay*> queue;
    this->center = this->leftWidth;
    queue.push(this);
    int currentLevel = 0;
    int cursor = 0;
    while (!queue.empty()) {
        BinaryTreeDisplay* current = queue.front();
        queue.pop();
        if (current->level > currentLevel) {
            out << endl;
            // out << endl;
            
            currentLevel = current->level;
            cursor = 0;
        }

        int leadingSpaces = current->center - current->valueWidth / 2 - current->leftShoulderLen - cursor;

        out << setfill(' ') << setw(leadingSpaces) << "";
        if (current->leftShoulderLen > 0) {
            out << "\u250C";
            out << setfill('-') << setw(current->leftShoulderLen - 1) << "";
        }

        out << setfill(' ') << setw(current->valueWidth) << current->value;

        if (current->rightShoulderLen > 0) {
            out << setfill('-') << setw(current->rightShoulderLen - 1) << "";
            out << "\u2510";
        }

        cursor += leadingSpaces + current->leftShoulderLen + current->valueWidth + current->rightShoulderLen;

        if (current->left) {
            current->left->center = current->center - current->valueWidth / 2 - current->leftShoulderLen;
            queue.push(current->left);
        }
        if (current->right) {
            current->right->center = current->center + current->valueWidth / 2 + current->rightShoulderLen;
            queue.push(current->right);
        }

        debug("Printing node " + to_string(current->getValue()));
        debug("    Leading spaces " + to_string(leadingSpaces));
        debug("    Left shoulder " + to_string(current->leftShoulderLen));
        debug("    Value width " + to_string(current->valueWidth));
        debug("    Right shoulder " + to_string(current->rightShoulderLen));
        debug("    Last cursor " + to_string(cursor));
    }
}

void BinaryTreeDisplay::measure() {
    // calculateWidth();
    // int rightLeftWidth = right ? right->left->width : 0;
    // int leftRightWidth = left ? left->right->width : 0;
    // branchLength = max(rightLeftWidth, leftRightWidth);
    // if (left) {
    //     left->measure();
    // }
    // if (right) {
    //     right->measure();
    // }
}