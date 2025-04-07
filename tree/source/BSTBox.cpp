#include "BSTBox.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <iomanip>
#include <queue>
#include <vector>

using std::endl;
using std::queue;
using std::vector;
using std::to_string;
using spdlog::debug;
using std::max;
using std::min;
using std::setw;
using std::setfill;
using std::ostream;
using std::wostream;
using std::string;

#define MIN_VALUE_WIDTH 3
#define MIN_ARM_LEN 2
#define BOX_BORDER 1
#define BOX_HEIGHT 3
#define MIN_TREE_WIDTH 11
#define ARM_HEIGHT 2

#define LINE_HORZ_3 L'━'
#define LINE_VERT_3 L'┃'
#define CORNER_TL_3 L'┏'
#define CORNER_TR_3 L'┓'
#define CORNER_BL_3 L'┗'
#define CORNER_BR_3 L'┛'

// ASCII
#define LINE_HORZ_2 L'_'
#define LINE_VERT_2 L'|'
#define CORNER_TL_2 L'#'
#define CORNER_TR_2 L'#'
#define CORNER_BL_2 L'#'
#define CORNER_BR_2 L'#'
#define CORNER_2 L'.'

#define LINE_HORZ_1 L'═'
#define LINE_VERT_1 L'║'
#define CORNER_TL_1 L'╔'
#define CORNER_TR_1 L'╗'
#define CORNER_BL_1 L'╚'
#define CORNER_BR_1 L'╝'

#define BOX_H_LINE LINE_HORZ_2
#define BOX_V_LINE LINE_VERT_2
#define BOX_TL_CORNER CORNER_2
#define BOX_TR_CORNER CORNER_2
#define BOX_BL_CORNER LINE_VERT_2
#define BOX_BR_CORNER LINE_VERT_2

#define ARM_H_LINE LINE_HORZ_2
#define ARM_V_LINE LINE_VERT_2
#define ARM_TL_CORNER CORNER_2
#define ARM_TR_CORNER CORNER_2
#define ARM_R_JUNCTION LINE_VERT_2
#define ARM_L_JUNCTION LINE_VERT_2
#define ARM_T_JUNCTION LINE_VERT_2

#pragma region Function Declarations
void measure(BSTBox* node);
void draw(char** buffer, int x, int y, BSTBox* parent, BSTBox* node);
void drawArm(char** buffer, int x, int y, BSTBox* parent, BSTBox* child);
void drawBox(char** buffer, int x, int y, BSTBox* parent, BSTBox* node);
int getWidth(BSTBox* node);
int getHeight(BSTBox* node);
#pragma endregion

/**
 * @brief Construct the BSTBox node based on the AVL tree hierarchy.
 */
BSTBox* createBSTBox(AVLNode* avlNode) {
    BSTBox* node = new BSTBox;
    node->value = avlNode->value;
    node->left = avlNode->left ? createBSTBox(avlNode->left) : nullptr;
    node->right = avlNode->right ? createBSTBox(avlNode->right) : nullptr;
    return node;
}

/**
 * @brief Delete entire BSTBox tree.
 */
void deleteBSTBox(BSTBox* root) {
    if (!root) return;
    deleteBSTBox(root->left);
    deleteBSTBox(root->right);
    delete root;
}

void drawBox(char** buffer, int x, int y, BSTBox* parent, BSTBox* node) {
    int boxStartX = x + node->boxX;
    int boxEndX = boxStartX + node->boxWidth - 1;
    int boxStartY = y;
    int boxEndY = y + BOX_HEIGHT - 1;
    // 4 box's corners
    buffer[y][boxStartX] = BOX_TL_CORNER;
    buffer[y][boxEndX] = BOX_TR_CORNER;
    buffer[boxEndY][boxStartX] = BOX_BL_CORNER;
    buffer[boxEndY][boxEndX] = BOX_BR_CORNER;

    // Draw horizontal lines on top and bottom
    memset(buffer[y] + boxStartX + BOX_BORDER, BOX_H_LINE, boxEndX - boxStartX - BOX_BORDER);
    memset(buffer[boxEndY] + boxStartX + BOX_BORDER, BOX_H_LINE, boxEndX - boxStartX - BOX_BORDER);

    // Draw vertical lines on two sides
    buffer[y + 1][boxStartX] = BOX_V_LINE;
    buffer[y + 1][boxEndX] = BOX_V_LINE;

    // If the box is a child node, show the connecting point with its parent's arm.
    if (parent) {
        buffer[boxStartY][boxStartX + node->boxWidth / 2] = ARM_T_JUNCTION;
    }

    // Draw the value
    int valueStartX = boxStartX + BOX_BORDER + (node->valueWidth - node->valueString.length()) / 2;
    for (int i = 0; i < node->valueString.length(); ++i) {
        buffer[(boxEndY + boxStartY) / 2][valueStartX + i] = node->valueString[i];
    }
    
    debug("Drawing box for node " + to_string(node->value) + ":");
    debug("    Box start: (" + to_string(boxStartX) + ", " + to_string(boxStartY) + ")");
    debug("    Box end: (" + to_string(boxEndX) + ", " + to_string(boxEndY) + ")");
    debug("    Box width: " + to_string(node->boxWidth));
    debug("    Value start: " + to_string(valueStartX));
}

/**
 * @brief Draw a node to 2-D buffer.
 * @param buffer 2-D buffer holding drawing characters.
 * @param x Offset x from the origin of the buffer.
 * @param y Offset y from the origin of the buffer.
 * @param node Tree's root to be drawn.
 * @param parent Parent node, for additional information while drawing.
 */
void draw(char** buffer, int x, int y, BSTBox* parent, BSTBox* node) {
    drawBox(buffer, x, y, parent, node);

    // Draw the arms
    if (node->left) {
        drawArm(buffer, x, y, node, node->left);
    }
    if (node->right) {
        drawArm(buffer, x, y, node, node->right);
    }

    if (node->left) {
        draw(buffer, x, y + BOX_HEIGHT, node, node->left);
    }
    
    if (node->right) {
        draw(buffer, x + node->leftWidth + 1, y + BOX_HEIGHT, node, node->right);
    }
}

/**
 * @brief Draw the connecting line from parent node to child node.
 * @param buffer 2-D drawing buffer
 * @param parent Parent node
 * @param child Child node
 * @param x Starting x position of the child from the drawing origin.
 * @param y Starting y position of the child from the drawing origin.
 */
void drawArm(char** buffer, int x, int y, BSTBox* parent, BSTBox* child) {
    int startX, endX;
    int startY = y + BOX_HEIGHT / 2;
    int endY = startY + ARM_HEIGHT - 1;
    char corner;
    if (parent->left == child) {
        startX = x + parent->boxX - 1;
        endX = x + child->boxX + child->boxWidth / 2;
        corner = ARM_TL_CORNER;
        buffer[startY][startX + 1] = ARM_L_JUNCTION;
    } else if (parent->right == child) {
        startX = x + parent->boxX + parent->boxWidth;
        endX = x + parent->leftWidth + 1 + child->boxX + child->boxWidth / 2;
        corner = ARM_TR_CORNER;
        buffer[startY][startX - 1] = ARM_R_JUNCTION;
    }
    int minX = min(startX, endX);
    int maxX = max(startX, endX);
    memset(buffer[startY] + minX, ARM_H_LINE, maxX - minX + 1);
    buffer[startY + 1][endX] = ARM_V_LINE;
    buffer[startY][endX] = corner;
    
    debug("Drawing arm for node " + to_string(parent->value) + ":");
    debug("    Arm start: (" + to_string(startX) + ", " + to_string(startY) + ")");
    debug("    Arm end: (" + to_string(endX) + ", " + to_string(endY) + ")");
}

/**
 * @brief Print the tree content into an output stream.
 * @param out The output stream to print the result
 * @param node Tree's root.
 */
void presentBSTBox(ostream& out, BSTBox* node) {
    // Do measurement before drawing
    measure(node);
    
    // Allocate memory for drawing buffer based on tree's measured width and height
    char* buffer[node->height];
    for (int i = 0; i < node->height; ++i) {
        buffer[i] = new char[node->width + 1]; // plus 1 for end of line character
        memset(buffer[i], L' ', node->width);
    }

    debug("Buffer initialized, size: width " + to_string(node->width) + ", height " + to_string(node->height));

    draw(buffer, 0, 0, nullptr, node);

    debug("Printing tree " + to_string(node->value));
    for (int i = 0; i < node->height; ++i) {
        buffer[i][node->width] = L'\0';
        out << buffer[i] << endl;
    }

    debug("Freeing buffer memory");
    for (int i = 0; i < node->height; ++i) {
        delete[] buffer[i];
    }
}

/**
 * @brief Calculate dimensions and sizes needed for drawing for all nodes in the tree.
 * @param node Tree's root node.
 */
void measure(BSTBox* node) {
    // Parent node's sizes are summed up from children's size, so measure children first
    if (node->left) {
        measure(node->left);
    }
    if (node->right) {
        measure(node->right);
    }

    // calculate width of display value
    node->valueString = to_string(node->value);
    node->valueWidth = max((int)node->valueString.length(), MIN_VALUE_WIDTH);
    node->valueWidth += 1 - node->valueWidth % 2;

    // Calculate width of the tree:

    // Width of the bounding box
    node->boxWidth = node->valueWidth + 2 * BOX_BORDER;
    // Plain width in case there's no child nodes, can be larger than the box
    int widthAsLeaf = max(node->boxWidth, MIN_TREE_WIDTH);
    node->leftWidth = (node->left ? node->left->width : widthAsLeaf / 2);
    node->rightWidth = (node->right ? node->right->width : widthAsLeaf / 2);
    node->width = node->leftWidth + node->rightWidth + 1;

    // Calculate x position so that the node is center-aligned between its children
    if (node->left && node->right) {
        int childDistance = node->right->boxX + node->left->width + 1 - node->left->boxX;
        node->boxX = node->left->boxX + node->left->boxWidth / 2 + childDistance / 2 - node->boxWidth / 2;
    } else {
        node->boxX = node->leftWidth - node->boxWidth / 2;
    }

    node->height = BOX_HEIGHT + max(getHeight(node->left), getHeight(node->right));

    debug("Measured node " + to_string(node->value) + ":");
    debug("    Width: " + to_string(node->width));
    debug("    Height: " + to_string(node->height));
    debug("    Box width: " + to_string(node->boxWidth));
    debug("    Box X: " + to_string(node->boxX));
    debug("    Value width: " + to_string(node->valueWidth));
    debug("    Value string: " + node->valueString);
    debug("    Left width: " + to_string(getWidth(node->left)));
    debug("    Right width: " + to_string(getWidth(node->right)));
}

// Return width of the node, or zero if node is null.
int getWidth(BSTBox* node) {
    return node ? node->width : 0;
}

// Return height of the node, or zero if node is null.
int getHeight(BSTBox* node) {
    return node ? node->height : 0;
}