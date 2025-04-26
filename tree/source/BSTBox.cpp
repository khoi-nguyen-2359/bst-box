#include "BSTBox.h"
#include "../../log/source/L.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>

using std::endl;
using std::queue;
using std::vector;
using std::to_string;
using std::max;
using std::min;
using std::setw;
using std::setfill;
using std::ostream;
using std::wostream;
using std::string;

#define BOX_BORDER 1
#define BOX_PADDING 1
#define BOX_HEIGHT 4
#define BOX_V_MARGIN 1
#define BOX_H_MARGIN 2
#define ARM_MIN_WIDTH 3

#define LINE_HORZ_3 L'━'
#define LINE_VERT_3 L'┃'
#define CORNER_TL_3 L'┏'
#define CORNER_TR_3 L'┓'
#define CORNER_BL_3 L'┗'
#define CORNER_BR_3 L'┛'

// ASCII
#define LINE_HORZ_2 '_'
#define LINE_VERT_2 '|'
#define CORNER_TL_2 ' '
#define CORNER_TR_2 ' '
#define CORNER_BL_2 '|'
#define CORNER_BR_2 '|'
#define CORNER_2 ','

#define LINE_HORZ_1 L'═'
#define LINE_VERT_1 L'║'
#define CORNER_TL_1 L'╔'
#define CORNER_TR_1 L'╗'
#define CORNER_BL_1 L'╚'
#define CORNER_BR_1 L'╝'

#define BOX_H_LINE LINE_HORZ_2
#define BOX_V_LINE LINE_VERT_2
#define BOX_TL_CORNER CORNER_TL_2
#define BOX_TR_CORNER CORNER_TR_2
#define BOX_BL_CORNER CORNER_BL_2
#define BOX_BR_CORNER CORNER_BR_2

#define ARM_H_LINE LINE_HORZ_2
#define ARM_V_LINE LINE_VERT_2
#define ARM_TL_ELBOW CORNER_TL_2
#define ARM_TR_ELBOW CORNER_TR_2
#define ARM_R_JUNCTION LINE_VERT_2
#define ARM_L_JUNCTION LINE_VERT_2
#define ARM_T_JUNCTION LINE_VERT_2

#pragma region Function Declarations
void measure(BSTBox* node);
void draw(char** buffer, int x, int y, BSTBox* parent, BSTBox* node);
void drawArm(char** buffer, int x, int y, BSTBox* parent, BSTBox* child);
void drawBox(char** buffer, int x, int y, BSTBox* parent, BSTBox* node);
inline int getWidth(BSTBox* node);
inline int getHeight(BSTBox* node);
inline int getBoxCenterX(BSTBox* node, int offset = 0);
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
    buffer[boxStartY][boxStartX] = BOX_TL_CORNER;
    buffer[boxStartY][boxEndX] = BOX_TR_CORNER;
    buffer[boxEndY][boxStartX] = BOX_BL_CORNER;
    buffer[boxEndY][boxEndX] = BOX_BR_CORNER;

    // Draw horizontal lines on top and bottom
    memset(buffer[y] + boxStartX + BOX_BORDER, BOX_H_LINE, boxEndX - boxStartX - BOX_BORDER);
    memset(buffer[boxEndY] + boxStartX + BOX_BORDER, BOX_H_LINE, boxEndX - boxStartX - BOX_BORDER);

    // Draw vertical lines on two sides
    for (int i = boxStartY + 1; i < boxEndY; ++i) {
        buffer[i][boxStartX] = BOX_V_LINE;
        buffer[i][boxEndX] = BOX_V_LINE;
    }

    // If the box is a child node, show the connecting point with its parent's arm.
    if (parent) {
        buffer[boxStartY][boxStartX + node->boxWidth / 2] = ARM_T_JUNCTION;
    }

    // Draw the value
    int valueStartX = boxStartX + BOX_BORDER + BOX_PADDING;
    int valueY = (boxStartY + boxEndY + 1) / 2;
    for (int i = 0; i < node->valueString.length(); ++i) {
        buffer[valueY][valueStartX + i] = node->valueString[i];
    }
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

    if (node->left) {
        drawArm(buffer, x, y, node, node->left);
    }
    if (node->right) {
        drawArm(buffer, x, y, node, node->right);
    }

    if (node->left) {
        draw(buffer, x, y + BOX_HEIGHT + BOX_V_MARGIN, node, node->left);
    }
    
    if (node->right) {
        draw(buffer, x + node->rightOffset, y + BOX_HEIGHT + BOX_V_MARGIN, node, node->right);
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
    int armHeight = (BOX_HEIGHT - 1) / 2 + BOX_V_MARGIN + 1;
    int startX, endX;
    int startY = y + BOX_HEIGHT / 2;
    int endY = startY + armHeight - 1;
    char elbow;
    if (parent->left == child) {
        startX = x + parent->boxX - 1;
        endX = getBoxCenterX(child, x);
        elbow = ARM_TL_ELBOW;
        buffer[startY][startX + 1] = ARM_L_JUNCTION;
    } else if (parent->right == child) {
        startX = x + parent->boxX + parent->boxWidth;
        endX = getBoxCenterX(child, x + parent->rightOffset);
        elbow = ARM_TR_ELBOW;
        buffer[startY][startX - 1] = ARM_R_JUNCTION;
    }
    memset(buffer[startY] + min(startX, endX), ARM_H_LINE, max(startX, endX) - min(startX, endX) + 1);
    for (int i = startY + 1; i <= endY; ++i) {
        buffer[i][endX] = ARM_V_LINE;
    }
    buffer[startY][endX] = elbow;
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
        memset(buffer[i], ' ', node->width);
    }

    logger << "Buffer initialized, size: width " << node->width << ", height " << node->height << "\n";

    draw(buffer, 0, 0, nullptr, node);

    logger << "Printing tree:\n";
    for (int i = 0; i < node->height; ++i) {
        buffer[i][node->width] = '\0';
        out << buffer[i] << endl;
    }

    logger << "Freeing buffer memory\n";
    for (int i = 0; i < node->height; ++i) {
        delete[] buffer[i];
    }
}

/**
 * @brief Calculate dimensions and sizes needed for drawing for all nodes in the tree.
 * @param node Tree's root node.
 */
void measure(BSTBox* node) {
    // Postorder traversal
    if (node->left) {
        measure(node->left);
    }
    if (node->right) {
        measure(node->right);
    }

    // The bounding box
    node->valueString = to_string(node->value);
    node->boxWidth = node->valueString.size() + 2 * BOX_PADDING + 2 * BOX_BORDER;

    // Use left child as parent's anchor
    int leftBoxCenterX = node->left ? getBoxCenterX(node->left) : - ARM_MIN_WIDTH;
    node->boxX = leftBoxCenterX + ARM_MIN_WIDTH;
    node->width = node->boxX + node->boxWidth;
    if (node->right) {
        // First assume the two childs are back to back, then check for any overlappings.
        node->rightOffset = node->left ? node->left->width : node->boxWidth / 2;

        // 1. Check spaces for the right arm, shift the right node forwards if needed.
        int rightBoxCenterX = getBoxCenterX(node->right, node->rightOffset);
        int minRightBoxCenterX = node->boxX + node->boxWidth + ARM_MIN_WIDTH - 1;
        int centerXOffset = max(0, minRightBoxCenterX - rightBoxCenterX);
        node->rightOffset += centerXOffset;

        // 2. Check if the two childs leave enough spaces in between, if not increase the offset.
        int childSeparatorOffset = max(0, node->left ? (BOX_H_MARGIN - node->rightOffset + node->left->width) : 0);
        node->rightOffset += childSeparatorOffset;

        // Center-align the parent's box
        node->boxX = (getBoxCenterX(node->right, node->rightOffset) + leftBoxCenterX + 1) / 2 - node->boxWidth / 2;

        node->width = node->rightOffset + node->right->width;
    }

    node->height = BOX_V_MARGIN + BOX_HEIGHT + max(getHeight(node->left), getHeight(node->right));

    logger << "Measured node " << node->value << ":\n";
    logger << "    Width: " << node->width << "\n";
    logger << "    Height: " << node->height << "\n";
    logger << "    Box width: " << node->boxWidth << "\n";
    logger << "    Box X: " << node->boxX << "\n";
    logger << "    Value string: " << node->valueString << "\n";
    logger << "    Left width: " << getWidth(node->left) << "\n";
    logger << "    Right width: " << getWidth(node->right) << "\n";
}

// Return width of the node, or zero if node is null.
int getWidth(BSTBox* node) {
    return node ? node->width : 0;
}

// Return height of the node, or zero if node is null.
int getHeight(BSTBox* node) {
    return node ? node->height : 0;
}

int getBoxCenterX(BSTBox* node, int offset) {
    return node->boxX + node->boxWidth / 2 + offset;
}

int getLeftWidth(BSTBox* node) {
    if (node->left) {
        return node->left->width;
    }
    return node->boxWidth / 2;
}
