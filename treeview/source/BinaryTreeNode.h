#ifndef BINARYTREENODE_H
#define BINARYTREENODE_H

#include <iostream>

using std::cout;
using std::endl;

/**
 * @author Anh Khoi Nguyen
 * @date 2025-Mar-12
 */
class BinaryTreeNode {
    private:
        int value;
        BinaryTreeNode* left;
        BinaryTreeNode* right;

        void insert(BinaryTreeNode* node);

    public:
        BinaryTreeNode();
        BinaryTreeNode(int value);
        ~BinaryTreeNode();
        
        void insert(int value);
        
        int getValue() { return value; }
        BinaryTreeNode* getLeft() { return left; }
        BinaryTreeNode* getRight() { return right; }

        friend class BinaryTreeDisplay;
};

#endif