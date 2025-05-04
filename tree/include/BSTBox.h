#ifndef BSTBOX_H
#define BSTBOX_H

#include <iostream>

#include "avl.h"

using std::ostream;
using std::string;

/**
 * @brief Contain calculation results for BST drawing. 
 * BSTBox node replicates the BST structure it draws.
 *
 * Drawing origin
 *  (0,0) ╭┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╮
 *        ┆         ,_____,          ┆ 
 *        ┆     ,___|  B  |___,      ┆ 
 *        ┆     |   |_____|   |      ┆ 
 *        ┆┄┄┄┄┄|┄┄┄┄┄┬┄┬┄┄┄┄┄|┄┄┄┄┄┄┆ total height
 *        ┆  ,__v__,  ┆m┆  ,__v__,   ┆ 
 *        ┆  |  A  |  ┆m┆  |  C  |   ┆ 
 *        ┆  |_____|  ┆m┆  |_____|   ┆ 
 *        ╰┄┄┄┄┄┄┄┄┄┄┄┴┄┴┄┄┄┄┄┄┄┄┄┄┄┄╯
 *         A's width + (m)argin + C's width
 *                total width
 */
struct BSTBox {
    int value = 0;
    BSTBox* left = nullptr;
    BSTBox* right = nullptr;
    // Display value in string.
    string valueString;
    // Total width of the entire tree, covering nodes of all levels underneath.
    int width = 0;
    // Total height of the tree
    int height = 0;
    // Left position of the node value's bounding box, coordinated inside parent (not globally)
    int boxX = 0;
    // Width of node value's bounding box
    int boxWidth = 0;
    // Offset inside parent of the right child
    int rightOffset = 0;
};

#pragma region Function Declarations
BSTBox* createBSTBox(AVLNode* avlNode);
void deleteBSTBox(BSTBox* root);
void presentBSTBox(ostream& out, BSTBox* node);
#pragma endregion

#endif