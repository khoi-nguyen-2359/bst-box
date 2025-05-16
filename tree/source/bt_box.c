#include "bt_box.h"
#include "bstbox_utils.h"
#include "bstbox_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOX_BORDER 1
#define BOX_PADDING 1
#define BOX_HEIGHT 4
#define BOX_V_MARGIN 1
#define BOX_H_MARGIN 2
#define ARM_MIN_WIDTH 3

// ASCII
#define LINE_HORZ_2 '_'
#define LINE_VERT_2 '|'
#define CORNER_TL_2 ' '
#define CORNER_TR_2 ' '
#define CORNER_BL_2 '|'
#define CORNER_BR_2 '|'
#define CORNER_2 ','

#define BOX_H_LINE LINE_HORZ_2
#define BOX_V_LINE LINE_VERT_2
#define BOX_TL_CORNER CORNER_TL_2
#define BOX_TR_CORNER CORNER_TR_2
#define BOX_BL_CORNER CORNER_BL_2
#define BOX_BR_CORNER CORNER_BR_2

#define ARM_H_LINE LINE_HORZ_2
#define ARM_V_LINE LINE_VERT_2
#define ARM_TL_ELBOW CORNER_TL_2
#define ARM_TR_ELBOW CORNER_TR_2
#define ARM_R_JUNCTION LINE_VERT_2
#define ARM_L_JUNCTION LINE_VERT_2
#define ARM_T_JUNCTION LINE_VERT_2

typedef struct BTBoxRestoreNode {
    BTNode* node;
    int leftChild; // 0 for having no left child, 1 otherwise
    int rightChild; // 0 for having no right child, 1 otherwise
} BTBoxRestoreNode;

#define DEFINE_QUEUE(__TYPE__) \
typedef struct LinkedListEntry_##__TYPE__ { \
    __TYPE__ *data; \
    struct LinkedListEntry_##__TYPE__ *next; \
} LinkedListEntry_##__TYPE__; \
\
typedef struct Queue_##__TYPE__ { \
    LinkedListEntry_##__TYPE__ *head; \
    LinkedListEntry_##__TYPE__ *tail; \
} Queue_##__TYPE__; \
static LinkedListEntry_##__TYPE__* linkedlist_create_entry(__TYPE__ *data) { \
    LinkedListEntry_##__TYPE__ *entry = (LinkedListEntry_##__TYPE__*)malloc(sizeof(LinkedListEntry_##__TYPE__)); \
    if (!entry) { \
        return NULL; \
    } \
    entry->data = data; \
    entry->next = NULL; \
    return entry; \
} \
static Queue_##__TYPE__ * queue_create() {\
    Queue_##__TYPE__ *queue = (Queue_##__TYPE__*)malloc(sizeof(Queue_##__TYPE__)); \
    if (!queue) { \
        return NULL; \
    } \
    queue->head = NULL; \
    queue->tail = NULL; \
    return queue; \
} \
static void queue_push(Queue_##__TYPE__ *queue, __TYPE__ *data) { \
    if (!queue) { \
        return; \
    } \
    LinkedListEntry_##__TYPE__ *entry = linkedlist_create_entry(data); \
    if (!entry) { \
        return; \
    } \
    if (queue->tail == NULL) { \
        queue->head = queue->tail = entry; \
    } else { \
        queue->tail->next = entry; \
        queue->tail = entry; \
    } \
} \
static __TYPE__* queue_pop(Queue_##__TYPE__ *queue) { \
    if (!queue || queue->head == NULL) { \
        return NULL; \
    } \
    LinkedListEntry_##__TYPE__ *popped = queue->head; \
    queue->head = queue->head->next; \
    if (queue->head == NULL) { \
        queue->tail = NULL; \
    } \
    __TYPE__* poppedData = popped->data; \
    free(popped); \
    return poppedData; \
}

DEFINE_QUEUE(BTBoxRestoreNode);

static void linkedlist_free(LinkedListEntry_BTBoxRestoreNode *list) {
    if (list == NULL) {
        return;
    }
    LinkedListEntry_BTBoxRestoreNode *current = list;
    while (current != NULL) {
        LinkedListEntry_BTBoxRestoreNode *next = current->next;
        free(current->data->node);
        free(current->data);
        free(current);
        current = next;
    }
}

#pragma region Function Declarations
static void measure(BTBox* node);
static void print_buffer(char** buffer, int x, int y, BTBox* parent, BTBox* node);
static void print_arm(char** buffer, int x, int y, BTBox* parent, BTBox* child);
static void print_box(char** buffer, int x, int y, BTBox* parent, BTBox* node);
static int get_box_center_x(BTBox* node, int offset);

static int search_arm(char *line, int len, int start, int step);
static BTBoxRestoreNode* create_restore_node();
static LinkedListEntry_BTBoxRestoreNode* restore_nodes(char* line, int len);

// Return width of the node, or zero if node is null.
static inline int get_width(BTBox* node) {
    return node ? node->width : 0;
}

// Return height of the node, or zero if node is null.
static inline int get_height(BTBox* node) {
    return node ? node->height : 0;
}

#pragma endregion

BTNode* btbox_create_node(int value) {
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * @brief Construct the BSTBox node based on the binary tree hierarchy.
 */
BTBox* btbox_create_tree(BTNode* tree) {
    BTBox* box = (BTBox*)malloc(sizeof(BTBox));
    box->value = tree->value;
    box->left = tree->left ? btbox_create_tree(tree->left) : NULL;
    box->right = tree->right ? btbox_create_tree(tree->right) : NULL;
    return box;
}

/**
 * @brief Delete entire BSTBox tree.
 */
void btbox_free_tree(BTBox* root) {
    if (!root) return;
    btbox_free_tree(root->left);
    btbox_free_tree(root->right);
    free(root->valueString);
    free(root);
}

void btbox_free_node(BTNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->left) {
        btbox_free_node(node->left);
    }
    if (node->right) {
        btbox_free_node(node->right);
    }
    free(node);
}

void print_box(char** buffer, int x, int y, BTBox* parent, BTBox* node) {
    int boxStartX = x + node->boxX;
    int boxEndX = boxStartX + node->boxWidth - 1;
    int boxStartY = y;
    int boxEndY = y + BOX_HEIGHT - 1;
    // 4 box's corners
    buffer[boxStartY][boxStartX] = BOX_TL_CORNER;
    buffer[boxStartY][boxEndX] = BOX_TR_CORNER;
    buffer[boxEndY][boxStartX] = BOX_BL_CORNER;
    buffer[boxEndY][boxEndX] = BOX_BR_CORNER;

    // Draw horizontal lines on top and bottom
    memset(buffer[y] + boxStartX + BOX_BORDER, BOX_H_LINE, boxEndX - boxStartX - BOX_BORDER);
    memset(buffer[boxEndY] + boxStartX + BOX_BORDER, BOX_H_LINE, boxEndX - boxStartX - BOX_BORDER);

    // Draw vertical lines on two sides
    for (int i = boxStartY + 1; i < boxEndY; ++i) {
        buffer[i][boxStartX] = BOX_V_LINE;
        buffer[i][boxEndX] = BOX_V_LINE;
    }

    // If the box is a child node, show the connecting point with its parent's arm.
    if (parent) {
        buffer[boxStartY][boxStartX + node->boxWidth / 2] = ARM_T_JUNCTION;
    }

    // Draw the value
    int valueStartX = boxStartX + BOX_BORDER + BOX_PADDING;
    int valueY = (boxStartY + boxEndY + 1) / 2;
    for (int i = 0; i < strlen(node->valueString); ++i) {
        buffer[valueY][valueStartX + i] = node->valueString[i];
    }
}

/**
 * @brief Draw a node to 2-D buffer.
 * @param buffer 2-D buffer holding drawing characters.
 * @param x Offset x from the origin of the buffer.
 * @param y Offset y from the origin of the buffer.
 * @param node Tree's root to be printed.
 * @param parent Parent node, for additional information while printing.
 */
void print_buffer(char** buffer, int x, int y, BTBox* parent, BTBox* node) {
    print_box(buffer, x, y, parent, node);

    if (node->left) {
        print_arm(buffer, x, y, node, node->left);
    }
    if (node->right) {
        print_arm(buffer, x, y, node, node->right);
    }

    if (node->left) {
        print_buffer(buffer, x, y + BOX_HEIGHT + BOX_V_MARGIN, node, node->left);
    }
    
    if (node->right) {
        print_buffer(buffer, x + node->rightOffset, y + BOX_HEIGHT + BOX_V_MARGIN, node, node->right);
    }
}

/**
 * @brief Print the connecting line from parent node to child node.
 * @param buffer 2-D printing buffer
 * @param parent Parent node
 * @param child Child node
 * @param x Starting x position of the child from the printing origin.
 * @param y Starting y position of the child from the printing origin.
 */
void print_arm(char** buffer, int x, int y, BTBox* parent, BTBox* child) {
    int armHeight = (BOX_HEIGHT - 1) / 2 + BOX_V_MARGIN + 1;
    int startX, endX;
    int startY = y + BOX_HEIGHT / 2;
    int endY = startY + armHeight - 1;
    char elbow;
    if (parent->left == child) {
        startX = x + parent->boxX - 1;
        endX = get_box_center_x(child, x);
        elbow = ARM_TL_ELBOW;
        buffer[startY][startX + 1] = ARM_L_JUNCTION;
    } else if (parent->right == child) {
        startX = x + parent->boxX + parent->boxWidth;
        endX = get_box_center_x(child, x + parent->rightOffset);
        elbow = ARM_TR_ELBOW;
        buffer[startY][startX - 1] = ARM_R_JUNCTION;
    }
    memset(buffer[startY] + bstbox_min(startX, endX), ARM_H_LINE, abs(endX - startX) + 1);
    for (int i = startY + 1; i <= endY; ++i) {
        buffer[i][endX] = ARM_V_LINE;
    }
    buffer[startY][endX] = elbow;
}

/**
 * @brief Print the tree content into an output stream.
 * @param out The output stream to print the result
 * @param node Tree's root.
 */
void btbox_print(FILE* file, BTBox* node) {
    if (!file || !node) {
        return;
    }

    // Do measurement before printing
    measure(node);
    
    // Allocate memory for printing buffer based on tree's measured width and height
    int actualRows = 0;
    char** buffer = (char**)malloc(node->height * sizeof(char*));
    if (!buffer) {
        goto cleanup;
    }
    for (int i = 0; i < node->height; ++i) {
        buffer[i] = (char*)malloc(node->width + 1); // plus 1 for end of line character
        if (!buffer[i]) {
            goto cleanup;
        }

        ++actualRows;
        memset(buffer[i], ' ', node->width);
        buffer[i][node->width] = '\0';
    }

    print_buffer(buffer, 0, 0, NULL, node);

    for (int i = 0; i < node->height; ++i) {
        fprintf(file, "%s\n", buffer[i]);
    }

    fflush(file);

cleanup:
    for (int i = 0; i < actualRows; ++i) {
        free(buffer[i]);
    }
    free(buffer);
}

/**
 * @brief Calculate dimensions and sizes needed for printing for all nodes in the tree.
 * @param node Tree's root node.
 */
void measure(BTBox* node) {
    if (node->left) {
        measure(node->left);
    }
    if (node->right) {
        measure(node->right);
    }

    // The bounding box
    node->valueString = bstbox_to_string(node->value);
    node->boxWidth = strlen(node->valueString) + 2 * BOX_PADDING + 2 * BOX_BORDER;

    // Use left child as parent's anchor
    int leftBoxCenterX = node->left ? get_box_center_x(node->left, 0) : - ARM_MIN_WIDTH;
    node->boxX = leftBoxCenterX + ARM_MIN_WIDTH;
    node->width = node->boxX + node->boxWidth;
    if (node->right) {
        // First assume the two childs are back to back, then check for any overlappings.
        node->rightOffset = node->left ? node->left->width : node->boxWidth / 2;

        // 1. Check spaces for the right arm, shift the right node forwards if needed.
        int rightBoxCenterX = get_box_center_x(node->right, node->rightOffset);
        int minRightBoxCenterX = node->boxX + node->boxWidth + ARM_MIN_WIDTH - 1;
        int centerXOffset = (minRightBoxCenterX > rightBoxCenterX) ? (minRightBoxCenterX - rightBoxCenterX) : 0;
        node->rightOffset += centerXOffset;

        // 2. Check if the two childs leave enough spaces in between, if not increase the offset.
        int childSeparatorOffset = (node->left && (BOX_H_MARGIN > node->rightOffset - node->left->width)) ? (BOX_H_MARGIN - node->rightOffset + node->left->width) : 0;
        node->rightOffset += childSeparatorOffset;

        // Center-align the parent's box
        node->boxX = (get_box_center_x(node->right, node->rightOffset) + leftBoxCenterX + 1) / 2 - node->boxWidth / 2;

        node->width = node->rightOffset + node->right->width;
    }

    node->height = BOX_V_MARGIN + BOX_HEIGHT + bstbox_max(get_height(node->left), get_height(node->right));
}

int get_box_center_x(BTBox* node, int offset) {
    return node->boxX + node->boxWidth / 2 + offset;
}

/**
 * @brief Read the text content from [file] and recreate the binary tree.
 * @param file Text file in the export format.
 * @return The binary tree, or NULL if malformed input is given.
 */
BTNode* btbox_restore_tree(FILE* file) {
    char *buffer = NULL;
    size_t bufferSize = 0;
    ssize_t len = 0;

    LinkedListEntry_BTBoxRestoreNode *list = NULL;
    BTBoxRestoreNode *rootInfo = NULL;
    // First loop: find the root node.
    do {
        buffer = bstbox_read_line(file, &bufferSize); // Replace getline
        len = buffer ? bufferSize : -1;
        list = restore_nodes(buffer, len);
        free(buffer);
        buffer = NULL;
        if (list != NULL) {
            if (list->next == NULL) {
                // the root is found
                rootInfo = list->data;
                free(list);
            } else {
                // malformed input, multiple nodes found
                linkedlist_free(list);
            }
            break;
        }
    } while (!feof(file));

    if (rootInfo == NULL) {
        return NULL;
    }
    
    BTNode *root = rootInfo->node; // keep the root for something to return

    // Second loop: parse nodes in each levels and connect to their parent above.
    Queue_BTBoxRestoreNode *queue = queue_create();
    queue_push(queue, rootInfo);
    do {
        // 1. Parse information of nodes on a line.
        buffer = bstbox_read_line(file, &bufferSize);
        len = buffer ? bufferSize : -1;
        list = restore_nodes(buffer, len);
        free(buffer);
        buffer = NULL;
        if (list == NULL) {
            // this line doesn't contain any nodes
            continue;
        }

        // 2. Connect each node on this level with its parent node in the queue.
        LinkedListEntry_BTBoxRestoreNode *current = list;
        while (queue->head != NULL) {
            BTBoxRestoreNode *parent = queue_pop(queue);
            if (parent->leftChild && current != NULL) {
                BTBoxRestoreNode *leftChild = current->data;
                parent->node->left = leftChild->node;
                current = current->next;
            }
            if (parent->rightChild && current != NULL) {
                BTBoxRestoreNode *rightChild = current->data;
                parent->node->right = rightChild->node;
                current = current->next;
            }
            free(parent);
        }

        // 3. Enqueue all nodes on this level for the next iteration.
        current = list;
        while (current != NULL) {
            if (current->data->leftChild || current->data->rightChild) {
                queue_push(queue, current->data);
            } else {
                // don't need to process nodes without children
                free(current->data);
            }
            LinkedListEntry_BTBoxRestoreNode *next = current->next;
            free(current);
            current = next;
        }
    } while (queue->head != NULL);

clean_up:
    free(queue); // queue is already empty here

    return root;
}

BTBoxRestoreNode* create_restore_node() {
    BTBoxRestoreNode *node = (BTBoxRestoreNode*)malloc(sizeof(BTBoxRestoreNode));
    node->leftChild = 0;
    node->rightChild = 0;
    node->node = NULL;
    return node;
}

LinkedListEntry_BTBoxRestoreNode* restore_nodes(char* buffer, int len) {
    LinkedListEntry_BTBoxRestoreNode* list = NULL;
    int detectNum = 0;
    int c = 1;
    int numStart = -1, numEnd = -1;
    for (int i = len - 1; i >= 0; --i) {
        if (bstbox_is_numeric(buffer[i])) {
            if (buffer[i] == '-') {
                detectNum *= -1;
            } else {
                detectNum += (buffer[i] - '0') * c;
                c *= 10;
            }
            if (numEnd == -1) {
                numEnd = i;
            }
            if (i == 0 || !bstbox_is_numeric(buffer[i-1])) {
                numStart = i;
            }

            // a number detected, check if there're arms at two sides of it
            if (numStart != -1 && numEnd != -1) {
                BTBoxRestoreNode *node = create_restore_node();
                node->leftChild = search_arm(buffer, len, numStart - 1, -1);
                node->rightChild = search_arm(buffer, len, numEnd + 1, 1);
                node->node = btbox_create_node(detectNum);

                LinkedListEntry_BTBoxRestoreNode* entry = linkedlist_create_entry(node);
                entry->next = list;
                list = entry;

                detectNum = 0;
                c = 1;
                numStart = -1;
                numEnd = -1;
            }
        }
    }

    return list;
}

/**
 * Go forward or backward to search for signs of an arm.
 * @param buffer One line in the text file.
 * @param len Length of the buffer.
 * @param start Start position of searching.
 * @param step 1 - search for the right arm, -1 - left arm.
 */
int search_arm(char *buffer, int len, int start, int step) {
    for (int k = start; k >= 0 && k <= len - 2; k += step) {
        if (buffer[k] == ' ' || bstbox_is_numeric(buffer[k])) {
            continue;
        }
        // [k] is an edge, check [k+step] for the arm
        if (k == 0 || k == len - 2 || buffer[k+step] == ' ') {
            return 0;
        }
        return 1;
    }
    return 0;
}