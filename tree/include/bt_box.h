#ifndef BT_BOX_H
#define BT_BOX_H

#include "avl_tree.h"

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
typedef struct BTBox {
    int value;
    struct BTBox* left;
    struct BTBox* right;
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
} BTBox;

// Function declarations
BTBox* btbox_create_tree(struct AVLNode* avlNode);
void btbox_free_tree(BTBox* root);
void btbox_print(FILE* file, BTBox* node);
AVLNode* btbox_restore_tree(FILE* file);

#endif