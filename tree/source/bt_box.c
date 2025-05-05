#include "bt_box.h"
#include "L.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BOX_BORDER 1
#define BOX_PADDING 1
#define BOX_HEIGHT 4
#define BOX_V_MARGIN 1
#define BOX_H_MARGIN 2
#define ARM_MIN_WIDTH 3

// ASCII
#define LINE_HORZ_2 '_'
#define LINE_VERT_2 '|'
#define CORNER_TL_2 ' '
#define CORNER_TR_2 ' '
#define CORNER_BL_2 '|'
#define CORNER_BR_2 '|'
#define CORNER_2 ','

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
void measure(BTBox* node);
void draw(char** buffer, int x, int y, BTBox* parent, BTBox* node);
void draw_arm(char** buffer, int x, int y, BTBox* parent, BTBox* child);
void draw_box(char** buffer, int x, int y, BTBox* parent, BTBox* node);
inline int get_width(BTBox* node);
inline int get_height(BTBox* node);
inline int get_box_centerX(BTBox* node, int offset);
#pragma endregion

/**
 * @brief Construct the BSTBox node based on the AVL tree hierarchy.
 */
BTBox* btbox_create_tree(AVLNode* avlNode) {
    BTBox* node = (BTBox*)malloc(sizeof(BTBox));
    node->value = avlNode->value;
    node->left = avlNode->left ? btbox_create_tree(avlNode->left) : NULL;
    node->right = avlNode->right ? btbox_create_tree(avlNode->right) : NULL;
    return node;
}

/**
 * @brief Delete entire BSTBox tree.
 */
void btbox_delete_tree(BTBox* root) {
    if (!root) return;
    btbox_delete_tree(root->left);
    btbox_delete_tree(root->right);
    free(root->valueString);
    free(root);
}

void draw_box(char** buffer, int x, int y, BTBox* parent, BTBox* node) {
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
    for (int i = 0; i < strlen(node->valueString); ++i) {
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
void draw(char** buffer, int x, int y, BTBox* parent, BTBox* node) {
    draw_box(buffer, x, y, parent, node);

    if (node->left) {
        draw_arm(buffer, x, y, node, node->left);
    }
    if (node->right) {
        draw_arm(buffer, x, y, node, node->right);
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
void draw_arm(char** buffer, int x, int y, BTBox* parent, BTBox* child) {
    int armHeight = (BOX_HEIGHT - 1) / 2 + BOX_V_MARGIN + 1;
    int startX, endX;
    int startY = y + BOX_HEIGHT / 2;
    int endY = startY + armHeight - 1;
    char elbow;
    if (parent->left == child) {
        startX = x + parent->boxX - 1;
        endX = get_box_centerX(child, x);
        elbow = ARM_TL_ELBOW;
        buffer[startY][startX + 1] = ARM_L_JUNCTION;
    } else if (parent->right == child) {
        startX = x + parent->boxX + parent->boxWidth;
        endX = get_box_centerX(child, x + parent->rightOffset);
        elbow = ARM_TR_ELBOW;
        buffer[startY][startX - 1] = ARM_R_JUNCTION;
    }
    memset(buffer[startY] + (startX < endX ? startX : endX), ARM_H_LINE, abs(endX - startX) + 1);
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
void btbox_draw(FILE* file, BTBox* node) {
    if (!file || !node) {
        return;
    }

    // Do measurement before drawing
    measure(node);
    
    // Allocate memory for drawing buffer based on tree's measured width and height
    char** buffer = (char**)malloc(node->height * sizeof(char*));
    for (int i = 0; i < node->height; ++i) {
        buffer[i] = (char*)malloc((node->width + 1) * sizeof(char)); // plus 1 for end of line character
        memset(buffer[i], ' ', node->width);
        buffer[i][node->width] = '\0';
    }

    draw(buffer, 0, 0, NULL, node);

    for (int i = 0; i < node->height; ++i) {
        fprintf(file, "%s\n", buffer[i]);
    }

    fflush(file);

    for (int i = 0; i < node->height; ++i) {
        free(buffer[i]);
    }
    free(buffer);
}

/**
 * @brief Calculate dimensions and sizes needed for drawing for all nodes in the tree.
 * @param node Tree's root node.
 */
void measure(BTBox* node) {
    // Postorder traversal
    if (node->left) {
        measure(node->left);
    }
    if (node->right) {
        measure(node->right);
    }

    // The bounding box
    node->valueString = (char*) malloc(12 * sizeof(char));
    bstbox_itoa(node->value, node->valueString, 10);
    node->boxWidth = strlen(node->valueString) + 2 * BOX_PADDING + 2 * BOX_BORDER;

    // Use left child as parent's anchor
    int leftBoxCenterX = node->left ? get_box_centerX(node->left, 0) : - ARM_MIN_WIDTH;
    node->boxX = leftBoxCenterX + ARM_MIN_WIDTH;
    node->width = node->boxX + node->boxWidth;
    if (node->right) {
        // First assume the two childs are back to back, then check for any overlappings.
        node->rightOffset = node->left ? node->left->width : node->boxWidth / 2;

        // 1. Check spaces for the right arm, shift the right node forwards if needed.
        int rightBoxCenterX = get_box_centerX(node->right, node->rightOffset);
        int minRightBoxCenterX = node->boxX + node->boxWidth + ARM_MIN_WIDTH - 1;
        int centerXOffset = (minRightBoxCenterX > rightBoxCenterX) ? (minRightBoxCenterX - rightBoxCenterX) : 0;
        node->rightOffset += centerXOffset;

        // 2. Check if the two childs leave enough spaces in between, if not increase the offset.
        int childSeparatorOffset = (node->left && (BOX_H_MARGIN > node->rightOffset - node->left->width)) ? (BOX_H_MARGIN - node->rightOffset + node->left->width) : 0;
        node->rightOffset += childSeparatorOffset;

        // Center-align the parent's box
        node->boxX = (get_box_centerX(node->right, node->rightOffset) + leftBoxCenterX + 1) / 2 - node->boxWidth / 2;

        node->width = node->rightOffset + node->right->width;
    }

    node->height = BOX_V_MARGIN + BOX_HEIGHT + (get_height(node->left) > get_height(node->right) ? get_height(node->left) : get_height(node->right));
}

// Return width of the node, or zero if node is null.
int get_width(BTBox* node) {
    return node ? node->width : 0;
}

// Return height of the node, or zero if node is null.
int get_height(BTBox* node) {
    return node ? node->height : 0;
}

int get_box_centerX(BTBox* node, int offset) {
    return node->boxX + node->boxWidth / 2 + offset;
}

int getLeftWidth(BTBox* node) {
    if (node->left) {
        return node->left->width;
    }
    return node->boxWidth / 2;
}
