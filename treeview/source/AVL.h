#ifndef AVL_H
#define AVL_H

struct AVLNode {
    int value = 0;
    int height = 1;
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;
};

AVLNode* createAVLTree(const int* values, int len);
AVLNode* createAVLNode(int value);
bool insertAVLNode(AVLNode*& root, int value);
void insertAVLNodes(AVLNode*& root, const int* values, const int len);
bool removeAVLNode(AVLNode*& root, int value);
void deleteAVLNode(AVLNode*& root);

#endif