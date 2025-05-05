#ifndef AVL_TREE_H
#define AVL_TREE_H

/**
 * @brief AVL Binary Search Tree using node height for balancing factor.
 */
typedef struct AVLNode {
    // Each node has an integer value.
    int value;

    // Number of levels counting from this node down below in the tree hierarchy.
    // Might get updated when insertion or deletion occurs.
    int height;

    // Node represents the left child.
    AVLNode* left;

    // Node represents the right child. 
    AVLNode* right;
} AVLNode;

#pragma region Functions Declarations

AVLNode* avl_create_tree(const int* values, const int len);
AVLNode* avl_create_node(int value);
bool avl_insert_node(AVLNode** root, int value);
void avl_insert_nodes(AVLNode** root, const int* values, const int len);
bool avl_remove_node(AVLNode** root, int value);
void avl_delete_tree(AVLNode** root);

#pragma endregion

#endif