#include "avl_node.h"
#include "L.h"

#include <stdlib.h>
#include <math.h>

#pragma region Function Declarations
int get_balance_factor(AVLNode* node);
void update_height(AVLNode* node);
int get_height(AVLNode* node);

void balance(AVLNode* root);
void rotate_left(AVLNode* root);
void rotate_right(AVLNode* root);

int remove_max(AVLNode* parent, AVLNode** node);

#pragma endregion

/**
 * @brief Initialize an AVL tree starting with a number of nodes.
 * @param values Node values array, provided in insertion order.
 * @param len Number of values to insert.
 * @return Pointer to the root node.
 */
AVLNode* avl_create_tree(const int* values, const int len) {
    AVLNode* root = NULL;
    avl_insert_nodes(&root, values, len);
    return root;
}

/**
 * @brief Allocate memory for an AVL Node without children.
 * @param value To assign as the node's value.
 * @return Pointer to the AVL Node.
 */
AVLNode* avl_create_node(int value) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->value = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * @brief Insert multiple nodes into a tree.
 * @param root Root node pointer, will be allocated if passing null.
 * @param values Array of values to insert.
 * @param len Size of value array.
 */
void avl_insert_nodes(AVLNode** root, const int* values, const int len) {
    for (int i = 0; i < len; ++i) {
        avl_insert_node(root, values[i]);
    }
}

/**
 * @brief Delete assigned memory of the entire tree recursively.
 * @param root Pointer to the root node of the tree to be freed, assigned to null after finished.
 */
void avl_delete_tree(AVLNode** root) {
    if (!(*root)) return;
    int rootValue = (*root)->value;
    logger_printf("Deleting tree: %d\n", rootValue);
    avl_delete_tree(&(*root)->left);
    avl_delete_tree(&(*root)->right);
    free(*root);
    *root = NULL;
}

/**
 * @brief Insert a value to the tree.
 * @param root Tree's root.
 * @param value Value to insert.
 */
bool avl_insert_node(AVLNode** root, int value) {
    if (!(*root)) {
        *root = avl_create_node(value);
    }
    if (value == (*root)->value) {
        return false;
    }
    
    bool inserted = false;
    if (value < (*root)->value) {
        if ((*root)->left) {
            // Continue finding on the left sub-tree.
            inserted = avl_insert_node(&((*root)->left), value);
        } else {
            // Left child is empty, insert here.
            (*root)->left = avl_create_node(value);
            inserted = true;
        }
    } else if (value > (*root)->value) {
        if ((*root)->right) {
            // Continue finding on the right sub-tree.
            inserted = avl_insert_node(&((*root)->right), value);
        } else {
            // Right child is empty, insert here.
            (*root)->right = avl_create_node(value);
            inserted = true;
        }
    }

    if (inserted) {
        // Update height and rebalance if needed.
        update_height(*root);
        balance(*root);
    }
    return inserted;
}

/**
 * @brief Check for balance factors at a node and its children to determine if rotations needed.
 */
void balance(AVLNode* root) {
    int balanceFactor = get_balance_factor(root);
    if (balanceFactor >= -1 && balanceFactor <= 1) {
        return;
    }
    logger_printf("Balancing at node %d:\n", root->value);
    logger_printf("    left: %d\n", get_height(root->left));
    logger_printf("    right: %d\n", get_height(root->right));

    if (balanceFactor < -1) {   // Left child is by 2 higher than right child, consider right rotation.

        // Left child is slightly right-heavy, a right rotation at root might make its right child higher.
        // Therefore a left rotation at left child is needed.
        if (get_balance_factor(root->left) > 0) {
            rotate_left(root->left);
        }

        rotate_right(root);
    } else if (balanceFactor > 1) {
        // Similar implementation on the other side.

        if (get_balance_factor(root->right) < 0) {
            rotate_right(root->right);
        }
        rotate_left(root);
    }

    update_height(root->right);
    update_height(root->left);
    update_height(root);
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
void rotate_left(AVLNode* root) {
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
void rotate_right(AVLNode* root) {
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
 * @brief Find node to remove.
 * @param value Value to be removed.
 * @param root Tree's root node.
 */
bool avl_remove_node(AVLNode** root, int value) {
    logger_printf("Removing %d from node %d\n", value, (*root)->value);
    bool removed = false;
    if (value < (*root)->value) {
        // Continue finding on left sub-tree.
        removed = (*root)->left ? avl_remove_node(&(*root)->left, value) : false;
    } else if (value > (*root)->value) {
        // Continue finding on right sub-tree.
        removed = (*root)->right ? avl_remove_node(&(*root)->right, value) : false;
    } else {
        // Node found
        if (!(*root)->left && !(*root)->right) {
            free(*root);
            *root = NULL;
        } else if (!(*root)->left) {  
            // No left child, delete root and replaced by the right child.
            (*root)->value = (*root)->right->value;
            AVLNode* tempRight = (*root)->right;
            (*root)->right = (*root)->right->right;
            free(tempRight);
        } else if ((*root)->left->right) {
            // Left child exists, find the maximum node on the left sub-tree to replace root.
            int removedValue = remove_max((*root)->left, &(*root)->left->right);
            (*root)->value = removedValue;
        } else {
            // No right child, or left child is the maximum in its subtree (left child has no right child)
            // delete root and replaced by the left child
            (*root)->value = (*root)->left->value;
            AVLNode* tempLeft = (*root)->left;
            (*root)->left = (*root)->left->left;
            free(tempLeft);
        }
        removed = true;
        logger_printf("Removed %d\n", value);
    }

    if (removed && *root) {
        update_height(*root);
        balance(*root);
    }
    return removed;
}

/**
 * @brief Find the maximum node of the tree and remove it.
 * @param current Node to start finding.
 * @param parent Parent node of the current node. Needed for re-connecting with grandchild after deletion of current.
 */
int remove_max(AVLNode* parent, AVLNode** current) {
    int removedValue = 0;
    if ((*current)->right) {
        removedValue = remove_max(*current, &((*current)->right));
    } else {
        removedValue = (*current)->value;
        AVLNode* currentLeft = (*current)->left;
        logger_printf("Removing maximum child %d\n", removedValue);
        free(*current);
        *current = currentLeft;
    }

    update_height(parent);
    balance(parent);

    return removedValue;
}

/**
 * @param node A tree's node.
 * @return Height of given node, or zero if node is null.
 */
int get_balance_factor(AVLNode* node) {
    return node ? get_height(node->right) - get_height(node->left) : 0;
}

/**
 * @brief Update node's height by accumulating children's heights.
 * 
 * This step is necessary after insertion or deletion occurs under this node's subtree.
 * @param node Node to update height.
 */
void update_height(AVLNode* node) {
    if (node) {
        node->height = fmax(get_height(node->left), get_height(node->right)) + 1;
    }
}

/**
 * @param node A tree's node.
 * @return Height of given node, or zero if node is null.
 */
int get_height(AVLNode* node) {
    return node ? node->height : 0;
}
