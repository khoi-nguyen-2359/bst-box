#ifndef BSTBOX_H
#define BSTBOX_H

#include <iostream>

#include "AVL.h"

using std::wostream;
using std::string;

/**
 * @brief Contain calculation results for BST drawing. 
 * BSTBox node replicates the BST structure it draws.
 *
 * Drawing origin
 *  (0,0) ╭┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄╮
 *        ┆          ┏━━━┓           ┆ 
 *        ┆     ┏━━━━┫ B ┣━━━━┓      ┆ 
 *        ┆     ┃    ┗━━━┛    ┃      ┆ 
 *        ┆┄┄┄┄┄┃┄┄┄┄┄┬┄┬┄┄┄┄┄┃┄┄┄┄┄┄┆ total height
 *        ┆   ┏━┻━┓   ┆ ┆   ┏━┻━┓    ┆ 
 *        ┆   ┃ A ┃   ┆1┆   ┃ C ┃    ┆ 
 *        ┆   ┗━━━┛   ┆ ┆   ┗━━━┛    ┆ 
 *        ╰┄┄┄┄┄┄┄┄┄┄┄┴┄┴┄┄┄┄┄┄┄┄┄┄┄┄╯
 *         A's width + 1 + C's width
 *                total width
 */
struct BSTBox {
    int value = 0;
    BSTBox* left = nullptr;
    BSTBox* right = nullptr;
    // Display value in string.
    string valueString;
    // Maximum width for displaying node's value, can be larger than value's width
    int valueWidth = 0;
    // Total width of the entire tree, covering nodes of all levels underneath.
    int width = 0;
    // Total height of the tree
    int height = 0;
    // Width of the left half of the tree
    int leftWidth = 0;
    // Width of the right half of the tree
    int rightWidth = 0;
    // Left position of the node value's bounding box, 
    // relatively from the start of node's drawing region (not globally)
    int boxX = 0;
    // Width of node value's bounding box
    int boxWidth = 0;
};

#pragma region Function Declarations
BSTBox* createBSTBox(AVLNode* avlNode);
void deleteBSTBox(BSTBox* root);
void presentBSTBox(wostream& out, BSTBox* node);
#pragma endregion

#endif