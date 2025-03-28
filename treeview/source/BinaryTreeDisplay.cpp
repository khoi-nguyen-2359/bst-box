#include "BinaryTreeDisplay.h"

#include <queue>
#include <iostream>
#include <iomanip>
#include <cmath>

const int BinaryTreeDisplay::MIN_SHOULDER_LEN;
const int BinaryTreeDisplay::MIN_VALUE_WIDTH;
const int BinaryTreeDisplay::BORDER_WIDTH;

#define NODE_HEIGHT 3

// #define LINE_HORZ_2 char(205) //L'═'
// #define LINE_VERT_2 char(186) //L'║'
// #define CORNER_TL_2 char(201) //L'╔'
// #define CORNER_TR_2 char(187) //L'╗'
// #define CORNER_BL_2 char(200) //L'╚'
// #define CORNER_BR_2 char(188) //L'╝'

// #define LINE_HORZ_2 L'═'
// #define LINE_VERT_2 L'║'
// #define CORNER_TL_2 L'╔'
// #define CORNER_TR_2 L'╗'
// #define CORNER_BL_2 L'╚'
// #define CORNER_BR_2 L'╝'

#define LINE_HORZ_2 '-'
#define LINE_VERT_2 '|'
#define CORNER_TL_2 '+'
#define CORNER_TR_2 '+'
#define CORNER_BL_2 '+'
#define CORNER_BR_2 '+'

#define LINE_HORZ_1 L'─'
#define LINE_VERT_1 L'│'
#define CORNER_TL_1 L'┌'
#define CORNER_TR_1 L'┐'
#define CORNER_BL_1 L'└'
#define CORNER_BR_1 L'┘'
#define BRANCH_R L'├'
#define BRANCH_L L'┤'
#define BRANCH_U L'┴'

#define SLASH_F L'/'
#define SLASH_B L'\\'

void repeat(std::ostream &out, const char* s, int n);

BinaryTreeDisplay::BinaryTreeDisplay(): BinaryTreeDisplay(nullptr, 0) {}

BinaryTreeDisplay::BinaryTreeDisplay(BinaryTreeNode* root) {
    initialize(root, 0);
}

BinaryTreeDisplay::BinaryTreeDisplay(BinaryTreeNode* node, int level): level(level) {
    initialize(node, level);
}

void BinaryTreeDisplay::initialize(BinaryTreeNode* node, int level) {
    this->value = node->value;
    calculateValueWidth2();
    if (node->left) {
        left = new BinaryTreeDisplay(node->left, level + 1);
    }
    if (node->right) {
        right = new BinaryTreeDisplay(node->right, level + 1);
    }
    updateWidth2();
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

void BinaryTreeDisplay::updateWidth2() {
    leftShoulderLen = 0;
    if (left) {
        leftShoulderLen = max(MIN_SHOULDER_LEN, left->rightWidth - BORDER_WIDTH - valueWidth / 2);
    }

    rightShoulderLen = 0;
    if (right) {
        rightShoulderLen = max(MIN_SHOULDER_LEN, right->leftWidth - BORDER_WIDTH - valueWidth / 2);
    }

    if (left && right) {
        leftShoulderLen = rightShoulderLen 
            = max(max(leftShoulderLen, rightShoulderLen), MIN_SHOULDER_LEN);
    }

    leftWidth = BORDER_WIDTH + leftShoulderLen + valueWidth / 2 + (left ? left->leftWidth + 1 : 0);
    rightWidth = BORDER_WIDTH + rightShoulderLen + valueWidth / 2 + (right ? right->rightWidth + 1 : 0);

    width = leftWidth + rightWidth + 1;

    height = NODE_HEIGHT + max(leftShoulderLen - 1, 0) + max(left ? left->height : 0, right ? right->height : 0) + 1;

    debug("node " + to_string(value) + " level " + to_string(level));
    debug("    width " + to_string(width));
    debug("    valueWidth " + to_string(valueWidth));
    debug("    left shoulder " + to_string(leftShoulderLen));
    debug("    right shoulder " + to_string(rightShoulderLen));
    debug("    leftWidth " + to_string(leftWidth));
    debug("    rightWidth " + to_string(rightWidth));
    debug("    height " + to_string(height));
}

void BinaryTreeDisplay::updateWidth() {
    leftShoulderLen = 0;
    if (left) {
        leftShoulderLen = max(MIN_SHOULDER_LEN, left->rightWidth + 1 - BORDER_WIDTH - valueWidth / 2);
    }

    rightShoulderLen = 0;
    if (right) {
        rightShoulderLen = max(MIN_SHOULDER_LEN, right->leftWidth + 1 - BORDER_WIDTH - valueWidth / 2);
    }

    leftWidth = BORDER_WIDTH + leftShoulderLen + valueWidth / 2 + (left ? left->leftWidth : 0);
    rightWidth = BORDER_WIDTH + rightShoulderLen + valueWidth / 2 + (right ? right->rightWidth : 0);

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
    updateWidth2();
    return width;
}

int BinaryTreeDisplay::calculateValueWidth2() {
    valueString = to_string(this->value);
    valueWidth = valueString.length();
    if (valueString.length() < MIN_VALUE_WIDTH) {
        valueWidth = MIN_VALUE_WIDTH;
    }
    if (valueWidth % 2 == 0) {
        valueWidth++;
    }
    return valueWidth;
}

int BinaryTreeDisplay::calculateValueWidth() {
    valueWidth = this->value < 0 ? 1 : 0;
    int i = this->value;
    do {
        valueWidth++;
        i /= 10;
    } while (i != 0);
    if (valueWidth % 2 == 0) {
        valueWidth++;
    }
    return valueWidth = max(valueWidth, MIN_VALUE_WIDTH);
}

void repeat(ostream &out, const char* s, int n) {
    for (int i = 0; i < n; i++) {
        out << s;
    }
}

void repeat(char* row, const char c, int start, int end) {
    for (int i = start; i < end; i++) {
        row[i] = c;
    }
}

void BinaryTreeDisplay::render(char** output, const int centerX, const int centerY) {
    debug("Rendering node " + to_string(value) + " at (" + to_string(centerX) + ", " + to_string(centerY) + ")");
    const int startX = centerX - valueWidth / 2 - BORDER_WIDTH;
    const int endX = centerX + valueWidth / 2 + BORDER_WIDTH;
    const int startY = centerY;
    int y = startY;
    output[y][startX] = CORNER_TL_2;
    repeat(output[y], LINE_HORZ_2, startX + 1, endX);
    output[y][endX] = CORNER_TR_2;
    ++y;
    output[y][startX] = LINE_VERT_2;
    for (int i = 0; i < valueWidth; i++) {
        output[y][startX + 1 + i] = i < valueString.length() ? valueString[i] : ' ';
    }
    output[y][endX] = LINE_VERT_2;
    ++y;
    output[y][startX] = CORNER_BL_2;
    repeat(output[y], LINE_HORZ_2, startX + 1, endX);
    output[y][endX] = CORNER_BR_2;

    debug("Rendering shoulders: left " + to_string(leftShoulderLen) + ", right " + to_string(rightShoulderLen));
    if (left) {
        for (int i = 0; i < leftShoulderLen; ++i) {
            output[y + i][startX - 1 - i] = SLASH_F;
        }
    }
    if (right) {
        for (int i = 0; i < rightShoulderLen; ++i) {
            output[y + i][endX + 1 + i] = SLASH_B;
        }
    }

    if (left) {
        left->render(
            output, 
            centerX - valueWidth / 2 - BORDER_WIDTH - leftShoulderLen - 1, 
            y + leftShoulderLen
        );
    }
    if (right) {
        right->render(
            output, 
            centerX + valueWidth / 2 + BORDER_WIDTH + rightShoulderLen + 1, 
            y + rightShoulderLen
        );
    }
}

char** BinaryTreeDisplay::present2(wostream &out) {
    char** output = new char*[height];
    for (int i = 0; i < height; ++i) {
        output[i] = new char[width + 1];
        memset(output[i], ' ', width * sizeof(char));
    }

    debug("Buffer size: width " + to_string(width) + ", height " + to_string(height));

    // for (int i = 0; i < height; ++i) {
    //     output[i][width] = L'\0';
    //     out << i << *output[i] << endl;
    // }

    render(output, this->leftWidth, 0);

    for (int i = 0; i < height; ++i) {
        output[i][width] = L'\0';
        out << output[i] << endl;
    }

    return output;
}

void BinaryTreeDisplay::present(ostream &out) {
    // queue<BinaryTreeDisplay*> queue;
    // this->center = this->leftWidth;
    // queue.push(this);
    // int cursor = 0;
    // vector<BinaryTreeDisplay*> row;
    // while (!queue.empty()) {
    //     row.clear();
    //     while (!queue.empty()) {
    //         row.push_back(queue.front());
    //         queue.pop();
    //     }
    //     for (BinaryTreeDisplay *node : row) {
    //         if (node->left) {
    //             node->left->center = node->center - node->valueWidth / 2 - BORDER_WIDTH - node->leftShoulderLen;
    //         }
    //         if (node->right) {
    //             node->right->center = node->center + node->valueWidth / 2 + BORDER_WIDTH + node->rightShoulderLen;
    //         }
    //     }
    //     /* First line */
    //     cursor = 0;
    //     for (BinaryTreeDisplay* node : row) {
    //         int spaces = node->center - node->valueWidth / 2 - BORDER_WIDTH - cursor;
    //         repeat(out, " ", spaces);
    //         out << CORNER_TL_2;
    //         repeat(out, LINE_HORZ_2, node->valueWidth + 2*BORDER_WIDTH - 2);
    //         out << CORNER_TR_2;
    //         cursor = node->center + node->valueWidth / 2 + BORDER_WIDTH + 1;
    //     }
    //     out << endl;

    //     /* Second line */
    //     cursor = 0;
    //     for (BinaryTreeDisplay* node : row) {
    //         int spaces = node->center - node->valueWidth / 2 - BORDER_WIDTH - node->leftShoulderLen - cursor;
    //         repeat(out, " ", spaces);
    //         if (node->leftShoulderLen > 0) {
    //             out << CORNER_TL_1;
    //             repeat(out, LINE_HORZ_1, node->leftShoulderLen - 1);
    //         }
    //         out << LINE_VERT_2;
    //         out << setfill(' ') << setw(node->valueWidth) << node->value;
    //         out << LINE_VERT_2;
    //         if (node->rightShoulderLen > 0) {
    //             repeat(out, LINE_HORZ_1, node->rightShoulderLen - 1);
    //             out << CORNER_TR_1;
    //         }
    //         cursor = node->center + node->valueWidth / 2 + BORDER_WIDTH + node->rightShoulderLen + 1;
    //     }
    //     out << endl;

    //     /* Third line */
    //     cursor = 0;
    //     for (BinaryTreeDisplay* node : row) {
    //         int spaces = 0;
    //         if (node->left) {
    //             spaces = node->left->center - cursor;
    //             out << setfill(' ') << setw(spaces) << "";
    //             out << LINE_VERT_1;
    //             cursor = node->left->center + 1;
    //         }
            
    //         spaces = node->center - node->valueWidth / 2 - BORDER_WIDTH - cursor;
    //         repeat(out, " ", spaces);
    //         out << CORNER_BL_2;
    //         repeat(out, LINE_HORZ_2, node->valueWidth + 2*BORDER_WIDTH - 2);
    //         out << CORNER_BR_2;
    //         cursor = node->center + node->valueWidth / 2 + BORDER_WIDTH + 1;

    //         if (node->right) {
    //             repeat(out, " ", node->right->center - cursor);
    //             out << LINE_VERT_1;
    //             cursor = node->right->center + 1;
    //         } else {
    //             cursor = node->center + node->valueWidth / 2 + BORDER_WIDTH + 1;
    //         }
    //     }
    //     out << endl;

    //     for (BinaryTreeDisplay *node : row) {
    //         if (node->left) {
    //             queue.push(node->left);
    //         }
    //         if (node->right) {
    //             queue.push(node->right);
    //         }

    //         debug("Printing node " + to_string(node->getValue()));
    //         debug("    Center " + to_string(node->center));
    //         debug("    Left shoulder " + to_string(node->leftShoulderLen));
    //         debug("    Value width " + to_string(node->valueWidth));
    //         debug("    Right shoulder " + to_string(node->rightShoulderLen));
    //     }
    // }
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