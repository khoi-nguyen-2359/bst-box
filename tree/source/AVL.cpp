#include <iostream>

#include "AVL.h"
#include "L.h"

using std::max;
using std::endl;
using std::vector;
using std::to_string;

#pragma region Function Declarations
AVLNode* createAVLNode(int value);
void deleteAVLNode(AVLNode*& root);

int getBalanceFactor(AVLNode* node);
void updateHeight(AVLNode* node);
int getHeight(AVLNode* node);

void balance(AVLNode* root);
inline void rotateLeft(AVLNode* root);
inline void rotateRight(AVLNode* root);

int removeMax(AVLNode* parent, AVLNode*& node);

#pragma endregion

/**
 * @brief Allocate memory for an AVL Node without children.
 * @param value To assign as the node's value.
 * @return Pointer to the AVL Node.
 */
AVLNode* createAVLNode(int value) {
    AVLNode* node = new AVLNode();
    node->value = value;
    // As having no child, node height is 1 by default.
    node->height = 1;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

/**
 * @brief Delete assigned memory of the entire tree recursively.
 * @param root Pointer to the root node of the tree to be freed, assigned to null after finished.
 */
void deleteAVLNode(AVLNode*& root) {
    if (!root) return;
    int rootValue = root->value;
    logger_printf("Deleting tree: %d\n", rootValue);
    deleteAVLNode(root->left);
    deleteAVLNode(root->right);
    delete root;
    root = nullptr;
}

/**
 * @brief Initialize an AVL tree starting with a number of nodes.
 * @param values Node values array, provided in insertion order.
 * @param len Number of values to insert.
 * @return Pointer to the root node.
 */
AVLNode* createAVLTree(const int* values, int len) {
    AVLNode* root = nullptr;
    insertAVLNodes(root, values, len);
    return root;
}

/**
 * @brief Insert multiple nodes into a tree.
 * @param root Root node pointer, will be allocated if passing null.
 * @param values Array of values to insert.
 * @param len Size of value array.
 */
void insertAVLNodes(AVLNode*& root, const int* values, int len) {
    for (int i = 0; i < len; ++i) {
        insertAVLNode(root, values[i]);
    }
}

int getBalanceFactor(AVLNode* node) {
    return node ? getHeight(node->right) - getHeight(node->left) : 0;
}

/**
 * @param node A tree's node.
 * @return Height of given node, or zero if node is null.
 */
int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

/**
 * @brief Update node's height by accumulating children's heights.
 * 
 * This step is necessary after insertion or deletion occurs under this node's subtree.
 * @param node Node to update height.
 */
void updateHeight(AVLNode* node) {
    if (node) {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }
}

/**
 * @brief Do a left rotation at the given node. 
 * 
 * This operation belongs to self-balancing mechanism of AVL tree.
 * @param root Node where the rotation happens.
 * @example Assume a left rotation at node X needed to be done.
 * 
 *        ┏━━━┓                  
 *     ┏━━┫ A ┣━━┓           
 *     ┃  ┗━━━┛  ┃           
 *   ┏━┻━┓     ┏━┻━┓         
 *   ┃ B ┃  ┏━━┫ X ┣━━━┓      
 *   ┗━━━┛  ┃  ┗━━━┛   ┃     
 *        ┏━┻━┓      ┏━┻━┓                  
 *        ┃ X1┃   ┏━━┫ Z ┣━━┓           
 *        ┗━━━┛   ┃  ┗━━━┛  ┃           
 *              ┏━┻━┓     ┏━┻━┓         
 *              ┃ Z1┃     ┃ Z2┃
 *              ┗━━━┛     ┗━━━┛
 * 
 * become:
 *        ┏━━━┓                  
 *     ┏━━┫ A ┣━━┓           
 *     ┃  ┗━━━┛  ┃           
 *   ┏━┻━┓     ┏━┻━┓         
 *   ┃ B ┃  ┏━━┫ Z ┣━━━┓      
 *   ┗━━━┛  ┃  ┗━━━┛   ┃     
 *        ┏━┻━┓      ┏━┻━┓            
 *     ┏━━┫ X ┣━━┓   ┃ Z2┃        
 *     ┃  ┗━━━┛  ┃   ┗━━━┛        
 *   ┏━┻━┓     ┏━┻━┓         
 *   ┃ X1┃     ┃ Z1┃
 *   ┗━━━┛     ┗━━━┛
 */
inline void rotateLeft(AVLNode* root) {
    logger_printf("Rotating left at node %d\n", root->value);
    // Right child node is taken out and reserved as its value will shift to the root node's position
    AVLNode* reservedNode = root->right;
    
    int tempRootValue = root->value;

    // Relocate value and children of the right child upto root position.
    root->value = reservedNode->value;
    root->right = reservedNode->right;

    // Reuse the reserved node as the left-shifted old root.
    reservedNode->right = reservedNode->left;
    reservedNode->left = root->left;
    reservedNode->value = tempRootValue;

    // Connect the new root and the new left nodes.
    root->left = reservedNode;
}

/**
 * @brief Similar to left rotation.
 * @ref rotateLeft
 */
inline void rotateRight(AVLNode* root) {
    logger_printf("Rotating right at node %d\n", root->value);
    AVLNode* toReuseNode = root->left;
    int tempRootValue = root->value;
    root->value = root->left->value;
    root->left = root->left->left;
    toReuseNode->value = tempRootValue;
    toReuseNode->left = toReuseNode->right;
    toReuseNode->right = root->right;
    root->right = toReuseNode;
}

/**
 * @brief Check for balance factors at a node and its children to determine if rotations needed.
 */
void balance(AVLNode* root) {
    int balanceFactor = getBalanceFactor(root);
    if (balanceFactor >= -1 && balanceFactor <= 1) {
        return;
    }
    logger_printf("Balancing at node %d:\n", root->value);
    logger_printf("    left: %d\n", getHeight(root->left));
    logger_printf("    right: %d\n", getHeight(root->right));

    if (balanceFactor < -1) {   // Left child is by 2 higher than right child, consider right rotation.

        // Left child is slightly right-heavy, a right rotation at root might make its right child higher.
        // Therefore a left rotation at left child is needed.
        if (getBalanceFactor(root->left) > 0) {
            rotateLeft(root->left);
        }

        rotateRight(root);
    } else if (balanceFactor > 1) {
        // Similar implementation on the other side.

        if (getBalanceFactor(root->right) < 0) {
            rotateRight(root->right);
        }
        rotateLeft(root);
    }

    // Affected nodes must re-evaluate height.
    updateHeight(root->right);
    updateHeight(root->left);
    updateHeight(root);
}

/**
 * @brief Insert a value to the tree.
 * @param root Tree's root.
 * @param value Value to insert.
 */
bool insertAVLNode(AVLNode*& root, int value) {
    if (!root) {
        root = createAVLNode(value);
    }
    if (value == root->value) {
        return false; // Duplicate value, stop.
    }
    
    bool inserted = false;
    if (value < root->value) {
        if (root->left) {
            // Continue finding on the left sub-tree.
            inserted = insertAVLNode(root->left, value);
        } else {
            // Left child is empty, insert here.
            root->left = createAVLNode(value);
            inserted = true;
        }
    } else if (value > root->value) {
        if (root->right) {
            // Continue finding on the right sub-tree.
            inserted = insertAVLNode(root->right, value);
        } else {
            // Right child is empty, insert here.
            root->right = createAVLNode(value);
            inserted = true;
        }
    }

    if (inserted) {
        // Update height and rebalance if needed.
        updateHeight(root);
        balance(root);
    }
    return inserted;
}

/**
 * @brief Find node to remove.
 * @param value Value to be removed.
 * @param root Tree's root node.
 */
bool removeAVLNode(AVLNode*& root, int value) {
    logger_printf("Removing %d from node %d\n", value, root->value);
    bool removed = false;
    if (value < root->value) {
        // Continue finding on left sub-tree.
        removed = root->left ? removeAVLNode(root->left, value) : false;
    } else if (value > root->value) {
        // Continue finding on right sub-tree.
        removed = root->right ? removeAVLNode(root->right, value) : false;
    } else {
        // Node found
        if (!root->left && !root->right) {
            delete root;
            root = nullptr;
        } else if (!root->left) {  
            // No left child, delete root and replaced by the right child.
            root->value = root->right->value;
            AVLNode* tempRight = root->right;
            root->right = root->right->right;
            delete tempRight;
        } else if (root->left->right) {
            // Left child exists, find the maximum node on the left sub-tree to replace root.
            int removedValue = removeMax(root->left, root->left->right);
            root->value = removedValue;
        } else {    
            // No right child, or left child is the maximum in its subtree (left child has no right child)
            // delete root and replaced by the left child
            root->value = root->left->value;
            AVLNode* tempLeft = root->left;
            root->left = root->left->left;
            delete tempLeft;
        }
        removed = true;
        logger_printf("Removed %d\n", value);
    }

    if (removed && root) {
        updateHeight(root);
        balance(root);
    }
    return removed;
}

/**
 * @brief Find the maximum node of the tree and remove it.
 * @param current Node to start finding.
 * @param parent Parent node of the current node. Needed for re-connecting with grandchild after deletion of current.
 */
int removeMax(AVLNode* parent, AVLNode*& current) {
    int removedValue = 0;
    if (current->right) {
        removedValue = removeMax(current, current->right);
    } else {
        removedValue = current->value;
        AVLNode* currentLeft = current->left;
        logger_printf("Removing maximum child %d\n", removedValue);
        delete current;
        current = currentLeft;
    }

    updateHeight(parent);
    balance(parent);

    return removedValue;
}
