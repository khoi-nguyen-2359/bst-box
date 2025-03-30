#ifndef BINARYTREENODE_H
#define BINARYTREENODE_H

#include <iostream>
#include <spdlog/spdlog.h>

using std::cout;
using std::endl;
using spdlog::debug;
using std::vector;

/**
 * @author Anh Khoi Nguyen
 * @date 2025-Mar-12
 */
class BinaryTreeNode {
    private:
        int value = 0;
        int height = 1;
        BinaryTreeNode* left = nullptr;
        BinaryTreeNode* right = nullptr;

        bool insert(BinaryTreeNode* node);
        int removeMin(BinaryTreeNode* parent, BinaryTreeNode* current);
        int removeMax(BinaryTreeNode* parent, BinaryTreeNode* current);
        bool balance();
        void rotateRight();
        void rotateLeft();
    
        int getLeftHeight() { return left ? left->height : 0; }
        int getRightHeight() { return right ? right->height : 0; }

        int updateHeight();
        void calculateHeights();

        BinaryTreeNode& copy(const BinaryTreeNode& node);

    public:
        BinaryTreeNode();
        BinaryTreeNode(const BinaryTreeNode& node);
        BinaryTreeNode(const int value);
        BinaryTreeNode(const int value, const int height);
        BinaryTreeNode(const int values[], const int size);
        BinaryTreeNode(const vector<int> &values);
        ~BinaryTreeNode();
        
        void insert(int value);
        bool remove(int value);
        
        int getValue() { return value; }
        int getHeight() { return height; }
        BinaryTreeNode* getLeft() { return left; }
        BinaryTreeNode* getRight() { return right; }

        BinaryTreeNode& operator=(const BinaryTreeNode& node);

        friend class BinaryTreeDisplay;
};

#endif