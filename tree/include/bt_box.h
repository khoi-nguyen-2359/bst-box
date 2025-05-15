#ifndef BT_BOX_H
#define BT_BOX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Contain calculation results for printing. 
 * BSTBox node replicates the binary tree structure it prints.
 *
 * Origin
 *  (0,0) ╭┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╮
 *        ┆         _____         ┆ 
 *        ┆        |     |        ┆ 
 *        ┆    ____|  B  |____    ┆ 
 *        ┆   |    |_____|    |   ┆ 
 *        ┆┄┄┄|┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄|┄┄┄┆
 *        ┆   |    spacing    ┆   ┆ total height
 *        ┆┄┄┄|┄┄┄┬┄┄┄┄┄┄┄┬┄┄┄|┄┄┄┆
 *        ┆ __|__ ┆       ┆ __|__ ┆ 
 *        ┆|     |┆       ┆|     |┆ 
 *        ┆|  A  |┆spacing┆|  C  |┆ 
 *        ┆|_____|┆       ┆|_____|┆ 
 *        ╰┄┄┄┄┄┄┄┴┄┄┄┄┄┄┄┴┄┄┄┄┄┄┄╯
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

/**
 * Data of the binary tree to print out.
 */
typedef struct BTNode {
    int value;
    struct BTNode* left;
    struct BTNode* right;
} BTNode;

// Function declarations
BTNode* btbox_create_node(int value);
BTBox* btbox_create_tree(BTNode* tree);
void btbox_free_tree(BTBox* root);
void btbox_free_node(BTNode *node);
void btbox_print(FILE* file, BTBox* node);
BTNode* btbox_restore_tree(FILE* file);

#endif