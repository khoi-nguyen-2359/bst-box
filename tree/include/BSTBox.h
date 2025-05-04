#ifndef BSTBOX_H
#define BSTBOX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Contain calculation results for BST drawing. 
 * BSTBox node replicates the BST structure it draws.
 *
 * Drawing origin
 *  (0,0) ╭┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╮
 *        ┆          _____           ┆ 
 *        ┆         |     |          ┆ 
 *        ┆      ___|  B  |____      ┆ 
 *        ┆     |   |_____|    |     ┆ 
 *        ┆┄┄┄┄┄|┄┄┄┬┄┄┄┄┄┄┬┄┄┄|┄┄┄┄┄┆ total height
 *        ┆   __|__ ┆      ┆ __|__   ┆ 
 *        ┆  |     |┆      ┆|     |  ┆ 
 *        ┆  |  A  |┆margin┆|  C  |  ┆ 
 *        ┆  |_____|┆      ┆|_____|  ┆ 
 *        ╰┄┄┄┄┄┄┄┄┄┴┄┄┄┄┄┄┴┄┄┄┄┄┄┄┄┄╯
 *         A's width + (m)argin + C's width
 *                total width
 */
typedef struct BSTBox {
    int value;
    struct BSTBox* left;
    struct BSTBox* right;
    // Display value in string.
    char* valueString;
    // Total width of the entire tree, covering nodes of all levels underneath.
    int width;
    // Total height of the tree
    int height;
    // Left position of the node value's bounding box, coordinated inside parent (not globally)
    int boxX;
    // Width of node value's bounding box
    int boxWidth;
    // Offset inside parent of the right child
    int rightOffset;
} BSTBox;

// Function declarations
BSTBox* createBSTBox(struct AVLNode* avlNode);
void deleteBSTBox(BSTBox* root);
void presentBSTBox(FILE* out, BSTBox* node);

#endif