#include "BSTBoxy.h"

#define MIN_VALUE_WIDTH 3
#define MIN_ARM_LEN 2
#define BOX_BORDER 1
#define BOX_HEIGHT 3
#define BOX_V_SPACING 1
#define MIN_TREE_WIDTH 9
#define ARM_HEIGHT 2

#define LINE_HORZ_3 L'─'
#define LINE_VERT_3 L'│'
#define CORNER_TL_3 L'┌'
#define CORNER_TR_3 L'┐'
#define CORNER_BL_3 L'└'
#define CORNER_BR_3 L'┘'

#define LINE_HORZ_1 L'═'
#define LINE_VERT_1 L'║'
#define CORNER_TL_1 L'╔'
#define CORNER_TR_1 L'╗'
#define CORNER_BL_1 L'╚'
#define CORNER_BR_1 L'╝'

#define LINE_HORZ_2 '-'
#define LINE_VERT_2 '|'
#define CORNER_TL_2 '+'
#define CORNER_TR_2 '+'
#define CORNER_BL_2 '+'
#define CORNER_BR_2 '+'

#define BOX_H_LINE LINE_HORZ_1
#define BOX_V_LINE LINE_VERT_1
#define BOX_TL_CORNER CORNER_TL_1
#define BOX_TR_CORNER CORNER_TR_1
#define BOX_BL_CORNER CORNER_BL_1
#define BOX_BR_CORNER CORNER_BR_1

#define ARM_H_LINE LINE_HORZ_3
#define ARM_V_LINE LINE_VERT_3
#define ARM_TL_CORNER CORNER_TL_3
#define ARM_TR_CORNER CORNER_TR_3

int getWidth(BSTBoxyNode* node);
int getHeight(BSTBoxyNode* node);

void render(wchar_t** buffer, int x, int y, BSTBoxyNode* node);
void renderSquareArm(wchar_t** buffer, int x, int y, BSTBoxyNode* parent, BSTBoxyNode* child);
void renderSlashArm(wchar_t** buffer, int x, int y, BSTBoxyNode* parent, BSTBoxyNode* child);
void repeatRow(wchar_t* row, const wchar_t c, int start, int end);
void repeatColumn(wchar_t** buffer, int col, const wchar_t c, int start, int end);

void measureZigZagTree(BSTBoxyNode* node);

BSTBoxyNode* createBoxyNode(BSTNode* bstNode) {
    BSTBoxyNode* node = new BSTBoxyNode;
    node->value = bstNode->value;
    node->left = bstNode->left ? createBoxyNode(bstNode->left) : nullptr;
    node->right = bstNode->right ? createBoxyNode(bstNode->right) : nullptr;
    return node;
}

void deleteBoxyTree(BSTBoxyNode* root) {
    if (!root) return;
    deleteBoxyTree(root->left);
    deleteBoxyTree(root->right);
    delete root;
}

void repeatRow(wchar_t* row, const wchar_t c, int start, int end) {
    for (int i = start; i < end; i++) {
        row[i] = c;
    }
}

void repeatColumn(wchar_t** buffer, int col, const wchar_t c, int start, int end) {
    for (int i = start; i < end; i++) {
        buffer[i][col] = c;
    }
}

void render(wchar_t** buffer, int x, int y, BSTBoxyNode* node) {
    // Draw the box
    int boxStartX = x + node->boxX;
    int boxEndX = boxStartX + node->boxW - 1;
    int boxStartY = y;
    int boxEndY = y + BOX_HEIGHT - 1;
    buffer[y][boxStartX] = BOX_TL_CORNER;
    buffer[y][boxEndX] = BOX_TR_CORNER;
    buffer[boxEndY][boxStartX] = BOX_BL_CORNER;
    buffer[boxEndY][boxEndX] = BOX_BR_CORNER;
    repeatRow(buffer[y], BOX_H_LINE, boxStartX + 1, boxEndX);
    repeatRow(buffer[boxEndY], BOX_H_LINE, boxStartX + 1, boxEndX);
    repeatColumn(buffer, boxStartX, BOX_V_LINE, y + 1, boxEndY);
    repeatColumn(buffer, boxEndX, BOX_V_LINE, y + 1, boxEndY);

    debug("Drawing box for node " + to_string(node->value) + ":");
    debug("    Box start: (" + to_string(boxStartX) + ", " + to_string(boxStartY) + ")");
    debug("    Box end: (" + to_string(boxEndX) + ", " + to_string(boxEndY) + ")");
    debug("    Box width: " + to_string(node->boxW));

    // Draw the value
    int valueStartX = boxStartX + BOX_BORDER;
    for (int i = 0; i < node->valueString.length(); ++i) {
        buffer[(boxEndY + boxStartY) / 2][valueStartX + i] = node->valueString[i];
    }
    debug("    Value start: " + to_string(valueStartX));

    // Draw the arms
    if (node->left) {
        renderSquareArm(buffer, x, y, node, node->left);
    }
    if (node->right) {
        renderSquareArm(buffer, x, y, node, node->right);
    }

    if (node->left) {
        render(buffer, x, y + BOX_HEIGHT, node->left);
    }
    
    if (node->right) {
        render(buffer, x + node->boxX + node->boxW / 2 + 1, y + BOX_HEIGHT, node->right);
    }
}

void renderSlashArm(wchar_t** buffer, int x, int y, BSTBoxyNode* parent, BSTBoxyNode* child) {
    int startX, endX;
    int startY = y + BOX_HEIGHT - 1;
    int endY = startY + ARM_HEIGHT - 1;
    wchar_t corner;
    if (parent->left == child) {
        startX = x + parent->boxX - 1;
        endX = x + child->boxX + child->boxW / 2;
        corner = ARM_TL_CORNER;
    } else if (parent->right == child) {
        startX = x + parent->boxX + parent->boxW;
        endX = x + parent->boxX + parent->boxW / 2 + 1 + child->boxX + child->boxW / 2;
        corner = ARM_TR_CORNER;
    }
    repeatRow(buffer[startY], ARM_H_LINE, min(startX, endX), max(startX, endX) + 1);
    repeatColumn(buffer, endX, ARM_V_LINE, startY + 1, endY + 1);
    buffer[startY][endX] = corner;
}    

void renderSquareArm(wchar_t** buffer, int x, int y, BSTBoxyNode* parent, BSTBoxyNode* child) {
    int startX, endX;
    int startY = y + BOX_HEIGHT / 2;
    int endY = startY + ARM_HEIGHT - 1;
    wchar_t corner;
    if (parent->left == child) {
        startX = x + parent->boxX - 1;
        endX = x + child->boxX + child->boxW / 2;
        corner = ARM_TL_CORNER;
    } else if (parent->right == child) {
        startX = x + parent->boxX + parent->boxW;
        endX = x + parent->boxX + parent->boxW / 2 + 1 + child->boxX + child->boxW / 2;
        corner = ARM_TR_CORNER;
    }
    repeatRow(buffer[startY], ARM_H_LINE, min(startX, endX), max(startX, endX) + 1);
    repeatColumn(buffer, endX, ARM_V_LINE, startY + 1, endY + 1);
    buffer[startY][endX] = corner;

    debug("Drawing arm for node " + to_string(parent->value) + ":");
    debug("    Arm start: (" + to_string(startX) + ", " + to_string(startY) + ")");
    debug("    Arm end: (" + to_string(endX) + ", " + to_string(endY) + ")");
    
}

void present(wostream& out, BSTBoxyNode* node) {
    wchar_t* buffer[node->height];
    for (int i = 0; i < node->height; ++i) {
        buffer[i] = new wchar_t[node->width + 1];
        wmemset(buffer[i], L' ', node->width * sizeof(wchar_t));
    }

    debug("Initialized buffer, size: width " + to_string(node->width) + ", height " + to_string(node->height));

    render(buffer, 0, 0, node);

    debug("Printing tree " + to_string(node->value) + ":");
    for (int i = 0; i < node->height; ++i) {
        buffer[i][node->width] = L'\0';
        out << buffer[i] << endl;
    }

    debug("Freeing buffer memory");
    for (int i = 0; i < node->height; ++i) {
        delete[] buffer[i];
    }
}

void measure(BSTBoxyNode* node) {
    if (node->left) {
        measure(node->left);
    }
    if (node->right) {
        measure(node->right);
    }

    // calculate width of display value
    node->valueString = to_string(node->value);
    node->valueWidth = max((int)node->valueString.length(), MIN_VALUE_WIDTH);
    if (node->valueWidth % 2 == 0) {
        ++node->valueWidth;
    }

    // calculate width of the tree
    node->boxW = node->valueWidth + 2 * BOX_BORDER;
    int leftWidth = (node->left ? node->left->width : max(node->boxW, MIN_TREE_WIDTH) / 2);
    int rightWidth = (node->right ? node->right->width : max(node->boxW, MIN_TREE_WIDTH) / 2);
    node->width = leftWidth + rightWidth + 1;
    node->boxX = leftWidth - node->boxW / 2;

    node->height = BOX_HEIGHT + BOX_V_SPACING + max(getHeight(node->left), getHeight(node->right));

    debug("Measured node " + to_string(node->value) + ":");
    debug("    Width: " + to_string(node->width));
    debug("    Height: " + to_string(node->height));
    debug("    Box width: " + to_string(node->boxW));
    debug("    Box X: " + to_string(node->boxX));
    debug("    Value width: " + to_string(node->valueWidth));
    debug("    Value string: " + node->valueString);
    debug("    Left width: " + to_string(getWidth(node->left)));
    debug("    Right width: " + to_string(getWidth(node->right)));
}

void measureZigZagTree(BSTBoxyNode* node) {

}

void presentZigzagTree(wostream& out, BSTBoxyNode* node) {
    measureZigZagTree(node);
}

int getWidth(BSTBoxyNode* node) {
    return node ? node->width : 0;
}

int getHeight(BSTBoxyNode* node) {
    return node ? node->height : 0;
}