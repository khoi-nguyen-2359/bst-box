#include "avl_tree.h"
#include "bt_box.h"
#include "bstbox_input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Width of the decoration frame for action menu and texts
#define FRAME_WIDTH 70

// Flags indicate which sides a text should be bound in a frame
#define FLAG_TOP        0x01
#define FLAG_LEFT       0x02
#define FLAG_RIGHT      0x04
#define FLAG_BOTTOM     0x08
#define FLAG_SIDES      (FLAG_LEFT | FLAG_RIGHT)
#define FLAG_CLOSED     (FLAG_TOP | FLAG_LEFT | FLAG_RIGHT | FLAG_BOTTOM)

#pragma region Function Declarations

void create_random_tree(AVLNode** root, char* input);
void insert_nodes(AVLNode** root, char* input);
void delete_nodes(AVLNode** root, char* input);
void print_tree(AVLNode* root);
void reset_current_tree(AVLNode** root);
void export_to_file(AVLNode* root, char* input);
void import_from_file(AVLNode** root, char* input);
int verify_tree_content(AVLNode* root);
char* print_action_menu();
void print_frame(const char* text, int mask);
int rand_range(int min, int max);

#pragma endregion

#define DEFINE_TREE_CONVERTER(__TYPE_S__, __TYPE_D__) \
__TYPE_D__* convert_##__TYPE_S__##_to_##__TYPE_D__(__TYPE_S__ *tree) { \
    if (tree == NULL) { \
        return NULL; \
    } \
    __TYPE_D__ *btNode = NULL, *left = NULL, *right = NULL; \
    if (tree->left) { \
        left = convert_##__TYPE_S__##_to_##__TYPE_D__(tree->left); \
    } \
    if (tree->right) { \
        right = convert_##__TYPE_S__##_to_##__TYPE_D__(tree->right); \
    } \
    btNode = (__TYPE_D__*)malloc(sizeof(__TYPE_D__)); \
    btNode->value = tree->value; \
    btNode->left = left; \
    btNode->right = right; \
    return btNode; \
}

DEFINE_TREE_CONVERTER(AVLNode, BTNode);
DEFINE_TREE_CONVERTER(BTNode, AVLNode);

/**
 Binary Tree Visualization
         ┏━━━┓         
     ┏━━━┫22 ┣━━━┓     
     ┃   ┗━━━┛   ┃     
   ┏━┻━┓       ┏━┻━┓   
   ┃ 1 ┃       ┃333┃   
   ┗━━━┛       ┗━━━┛
 */
int main(int argc, char* argv[]) {
    AVLNode* tree = NULL;  // Pointer to the main tree object of the program.
    char* input = NULL;    // Pointer to hold user input.
    while (1) {
        free(input); // Free input after each iteration.
        input = print_action_menu();
        if (!input) {
            continue;
        }
        switch (input[0]) { // Action user chooses from the menu.
            case 'C': case 'c':
                create_random_tree(&tree, input);
            break;

            case 'I': case 'i':
                insert_nodes(&tree, input);
            break;

            case 'D': case 'd':
                delete_nodes(&tree, input);
            break;

            case 'V': case 'v':
                print_tree(tree);
            break;

            case 'R': case 'r':
                reset_current_tree(&tree);
            break;

            case 'E': case 'e':
                export_to_file(tree, input);
            break;

            case 'M': case 'm':
                import_from_file(&tree, input);
            break;

            default: goto clean_up;
        }
    }

clean_up:
    free(input);
    avl_free_tree(&tree);

    return 0;
}

/**
 * @brief First receive from user a number for node count, then generate random integers for node values.
 * 
 * Randomized values range around -500 -> 500.
 * @param root Tree's root node, will be deleted and re-allocated before insertion.
 */
void create_random_tree(AVLNode** root, char* input) {
    static const int MAX_RAND_VALUE = 1000;
    int nodeCount = atoi(input + 2);    // Skip the first two characters, which are 'C' and a space.
    printf("Creating tree with %d random nodes:", nodeCount);

    srand(time(NULL));
    int* randValues = (int*)malloc(nodeCount * sizeof(int));
    for (int i = 0; i < nodeCount; ++i) {
        int digits = rand_range(1, 9);
        int min = pow(10, digits - 1);
        int max = pow(10, digits) - 1;
        randValues[i] = rand_range(min, max) - max / 2;
        printf(" %d", randValues[i]);
    }
    printf("\n");
    
    avl_free_tree(root); // Free the existing tree before creating a new one.
    avl_insert_nodes(root, randValues, nodeCount);

    free(randValues);
    print_tree(*root);
}

/**
 * @brief Prompt user to enter a sequence of nodes to insert to the current tree.
 * 
 * @param root Tree's root node, will be allocated before insertion if null.
 */
void insert_nodes(AVLNode** root, char* input) {
    size_t size = 0;
    int* ints = bstbox_read_ints(input + 2, &size); // Skip the first two characters, which are 'I' and a space.
    printf("Inserting %lu integers.\n", size);
    avl_insert_nodes(root, ints, size);
    free(ints);
    print_tree(*root);
}

/**
 * @brief Prompt user to key in a number of integers to delete from the current tree.
 * 
 * The last node can also be deleted.
 * @param root Tree's root node, will be null if all nodes are deleted.
 */
void delete_nodes(AVLNode** root, char* input) {
    if (!verify_tree_content(*root)) {
        return;
    }
    size_t size = 0;
    int* ints = bstbox_read_ints(input + 2, &size);
    printf("Removing %lu integers.\n", size);
    for (int i = 0; i < size; ++i) {
        avl_remove_node(root, ints[i]);
    }
    free(ints);
    print_tree(*root);
}

/**
 * @brief Use BSTBox implementation to print the tree content to console output.
 * 
 * @param root Tree's root node.
 */
void print_tree(AVLNode* root) {
    if (!verify_tree_content(root)) {
        return;
    }

    BTNode* btRoot = convert_AVLNode_to_BTNode(root);
    BTBox* treeBox = btbox_create_tree(btRoot);

    printf("\n");
    print_frame("CURRENT TREE", FLAG_CLOSED);

    printf("\n");
    btbox_print(stdout, treeBox);

    btbox_free_tree(treeBox);
    btbox_free_node(btRoot);
}

/**
 * @brief Prompt user to put in the text file's name to store tree content as presented on console UI.
 * 
 * @param root Tree's root node.
 */
void export_to_file(AVLNode* root, char* input) {
    if (!verify_tree_content(root)) {
        return;
    }

    char c, fileName[strlen(input)];
    sscanf(input, "%c %s", &c, fileName);
    printf("Writing current tree content to file \"%s\"\n", fileName);

    FILE* file = fopen(fileName, "w");

    if (!file) {
        printf("Error opening file \"%s\"\n", fileName);
        return;
    }

    BTNode *btRoot = convert_AVLNode_to_BTNode(root);
    BTBox* box = btbox_create_tree(btRoot);

    btbox_print(file, box); // Print the tree into output file stream instead of console output stream.

    printf("File exported successfully at %s\n", fileName);

    fclose(file);
    btbox_free_tree(box);
    btbox_free_node(btRoot);
}

/**
 * @brief Delete all nodes from the tree and set to null.
 * 
 * @param root Tree's root node.
 */
void reset_current_tree(AVLNode** root) {
    avl_free_tree(root);
    verify_tree_content(*root);
}

/**
 * @brief Show a notice telling whether the tree is empty. 
 * 
 * This is to check before some tree's operations.
 * @return True If tree contains nodes, otherwise False.
 */
int verify_tree_content(AVLNode* root) {
    if (!root) {
        printf("\n");
        print_frame("TREE IS EMPTY.", FLAG_CLOSED);
        return 0;
    }

    return 1;
}

/**
 * @brief Print the program name and actions that the program offers in a menu.
 * 
 * @return Letter represents the action to proceed.
 */
char* print_action_menu() {
    printf("\n");
    print_frame("BINARY SEARCH TREE CONSOLE VISUALIZATION", FLAG_TOP | FLAG_SIDES | FLAG_BOTTOM);
    print_frame(
        "Please choose one action below:\n"
        "    > [C]reate a binary search tree from random nodes.\n"
        "    > [I]nsert nodes to current tree.\n"
        "    > [D]elete nodes from current tree.\n"
        "    > [V]iew current tree.\n"
        "    > [R]eset current tree.\n"
        "    > [E]xport to text file.\n"
        "    > I[M]port from text file.\n"
        "    > [Q]uit.\n"
        "Please enter your choice: [LETTER] [SPACE] [ARGUMENTS] [ENTER]\n"
        "Example: \"I 1 2 3\" to insert 3 nodes, \"E tree.txt\" to export to file.\n",
        FLAG_SIDES | FLAG_BOTTOM);

    char* input = NULL;
    size_t inputLen = 0;
    input = bstbox_read_line(stdin, &inputLen);
    return input;
}

/**
 * @brief Utility function to print a text content embedded inside a bounding box for decoration purpose.
 * 
 * @example 
 * ╔═══════════╗ -> top line
 * ║ BST     1 ║ -> text content lines
 * ║ BST     2 ║ -> (can be multi-line)
 * ╚═══════════╝ -> bottom line
 */
void print_frame(const char* text, int mask) {
    char buffer[FRAME_WIDTH + 1];

    if (mask & FLAG_TOP) {
        memset(buffer, '-', FRAME_WIDTH);
        buffer[0] = '+';
        buffer[FRAME_WIDTH - 1] = '+';
        buffer[FRAME_WIDTH] = '\0';
        printf("%s\n", buffer);
    }

    const char* found = strchr(text, '\n');
    const char* line = text;
    do {
        memset(buffer, ' ', FRAME_WIDTH);

        if (mask & FLAG_LEFT) {
            buffer[0] = '|';
            buffer[1] = ' ';
        }

        int lineLen = found ? (found - line) : strlen(line);
        int copyLen = lineLen < FRAME_WIDTH - 4 ? lineLen : FRAME_WIDTH - 4;
        memcpy(buffer + 2, line, copyLen);

        if (mask & FLAG_RIGHT) {
            buffer[FRAME_WIDTH - 2] = ' ';
            buffer[FRAME_WIDTH - 1] = '|';
        }

        buffer[FRAME_WIDTH] = '\0';
        printf("%s\n", buffer);

        if (!found) {
            break;
        }
        line = found + 1;
        found = strchr(line, '\n');
    } while (found);

    if (mask & FLAG_BOTTOM) {
        memset(buffer, '-', FRAME_WIDTH);
        buffer[0] = '+';
        buffer[FRAME_WIDTH - 1] = '+';
        buffer[FRAME_WIDTH] = '\0';
        printf("%s\n", buffer);
    }
}

int rand_range(int min, int max) {
    return min + (rand() % (max - min + 1));
}

void import_from_file(AVLNode** root, char* input) {
    char c, fileName[strlen(input)];
    sscanf(input, "%c %s", &c, fileName);
    printf("Reading tree content from file \"%s\"\n", fileName);

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file %s\n. Stop.", fileName);
        return;
    }

    BTNode *btRoot = btbox_restore_tree(file);
    AVLNode *avlRoot = convert_BTNode_to_AVLNode(btRoot);
    AVLNode *temp = *root;
    *root = avlRoot;

    print_tree(*root);

    fclose(file);
    btbox_free_node(btRoot);
    avl_free_tree(&temp); // Free the old tree after replacing it with the new one.
}