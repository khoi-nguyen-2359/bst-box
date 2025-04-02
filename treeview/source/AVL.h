#ifndef AVL_H
#define AVL_H

/**
 * @brief AVL Binary Search Tree using node height as balancing factor.
 */
struct AVLNode {
    // Each node has an integer value.
    int value = 0;

    // Number of levels counting from this node down below in the tree hierarchy.
    // Might get updated when insertion or deletion occurs.
    int height = 1;             

    // Node represents the left child.
    AVLNode* left = nullptr;    

    // Node represents the right child. 
    AVLNode* right = nullptr;   
};

#pragma region Functions Declarations

AVLNode* createAVLTree(const int* values, int len);
AVLNode* createAVLNode(int value);
bool insertAVLNode(AVLNode*& root, int value);
void insertAVLNodes(AVLNode*& root, const int* values, const int len);
bool removeAVLNode(AVLNode*& root, int value);
void deleteAVLNode(AVLNode*& root);

#pragma endregion

#endif