#ifndef BINARYTREENODE_H
#define BINARYTREENODE_H

#include <iostream>

using std::cout;
using std::endl;

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

        friend class BinaryTreeDisplay;
};

#endif