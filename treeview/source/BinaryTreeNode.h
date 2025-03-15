#ifndef BINARYTREENODE_H
#define BINARYTREENODE_H

#include <iostream>
#include "TestSupports.h"
#include "../../logger/source/Logger.h"

using std::cout;
using std::endl;

/**
 * @author Anh Khoi Nguyen
 * @date 2025-Mar-12
 */
class BinaryTreeNode {
    private:
        int value;
        int height;
        BinaryTreeNode* left;
        BinaryTreeNode* right;

        bool insert(BinaryTreeNode* node);
        bool balance();
        void rotateRight();
        void rotateLeft();

        int getLeftHeight() { return left ? left->height : 0; }
        int getRightHeight() { return right ? right->height : 0; }

        void updateHeight();

    public:
        BinaryTreeNode();
        BinaryTreeNode(int value);
        BinaryTreeNode(int value, int height);
        ~BinaryTreeNode();
        
        void insert(int value);
        
        int getValue() { return value; }
        BinaryTreeNode* getLeft() { return left; }
        BinaryTreeNode* getRight() { return right; }

        friend class BinaryTreeDisplay;

    VISIBLE_FOR_TESTING:
        void setLeftTest(BinaryTreeNode* left) { this->left = left; }
        void setRightTest(BinaryTreeNode* right) { this->right = right; }
        void rotateRightTest() { rotateRight();}
        void rotateLeftTest() { rotateLeft(); }
};

#endif