#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <filesystem>

#include "AVL.h"
#include "BSTBox.h"

using std::string;
using std::wcout;
using std::cin;
using std::endl;
using std::wofstream;
using std::vector;

// Locale for the output stream to display wide characters (node bounding box, connecting arms)
#define LOCALE "en_US.UTF-8"

// Width of the decoration frame for action menu and texts
#define FRAME_WIDTH 60

// Flags indicate which sides a text should be bound in a frame
#define FLAG_TOP 0b00001
#define FLAG_LEFT 0b00010
#define FLAG_RIGHT 0b00100
#define FLAG_BOTTOM 0b01000
#define FLAG_BOTTOM_T 0b10000
#define FLAG_SIDE (FLAG_LEFT | FLAG_RIGHT)
#define FLAG_CLOSED (FLAG_TOP | FLAG_LEFT | FLAG_RIGHT | FLAG_BOTTOM)

#pragma region Function Declarations
void createRandomTree(AVLNode*& root);
void insertNodes(AVLNode*& root);
void deleteNodes(AVLNode*& root);
void present(AVLNode* root);
void resetCurrentTree(AVLNode*& root);
void exportToFile(AVLNode* root);
void initializeLogging();
vector<int> getInputIntegers();
bool verifyTreeContent(AVLNode* root);
char printActionMenu();
void printFrame(const wchar_t* text, int mask);
#pragma endregion

/**
 Binary Tree Visualization
        ┏━━━┓          
     ┏━━┫ 4 ┣━━━━┓     
     ┃  ┗━━━┛    ┃     
   ┏━┻━┓      ┏━━┻━━┓  
   ┃ 1 ┃      ┃2025 ┃  
   ┗━━━┛      ┗━━━━━┛
 */
int main(int argc, char* argv[]) {
    initializeLogging();

    // This is important because text visualization is based on Unicode characters.
    wcout.imbue(std::locale(LOCALE));

    AVLNode* tree;  // Pointer to the main tree object of the program.
    char action;    // Action user chooses from the menu below.
    while (true) {
        action = printActionMenu();
        switch (action) {
            case 'C':
            case 'c':
                createRandomTree(tree);
            break;

            case 'I':
            case 'i':
                insertNodes(tree);
                break;

            case 'D':
            case 'd':
                deleteNodes(tree);
                break;

            case 'V':
            case 'v':
                present(tree);
                break;

            case 'R':
            case 'r':
                resetCurrentTree(tree);
                break;

            case 'E':
            case 'e':
                exportToFile(tree);
                break;
            
            default:
                return 0;
        }
    }

    deleteAVLNode(tree);

    return 0;
}

/**
 * @brief First receive from user a number for node count, then generate random integers for node values.
 * 
 * Randomized values range around -500 -> 500.
 * @param root Tree's root node, will be deleted and re-allocated before insertion.
 */
void createRandomTree(AVLNode*& root) {
    const int MAX_RAND_NODE = 20;
    int numResponse;
    wcout << "Please enter number of nodes (not exceeding " << MAX_RAND_NODE << "): ";
    cin >> numResponse;

    srand(time(nullptr));
    vector<int> randValues;
    int nodeCount = std::min(MAX_RAND_NODE, numResponse);
    for (int i = 0; i < nodeCount; ++i) {
        randValues.push_back(rand() % 1000 - 500);
    }
    deleteAVLNode(root);
    insertAVLNodes(root, randValues.data(), nodeCount);

    present(root);
}

/**
 * @brief Prompt user to enter a sequence of nodes to insert to the current tree.
 * 
 * @param root Tree's root node, will be allocated before insertion if null.
 */
void insertNodes(AVLNode*& root) {
    wcout << "Please enter integers in insertion order: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    insertAVLNodes(root, values.data(), values.size());
    present(root);
}

/**
 * @brief Prompt user to key in a number of integers to delete from the current tree.
 * 
 * The last node can also be deleted.
 * @param root Tree's root node, will be null if all nodes are deleted.
 */
void deleteNodes(AVLNode*& root) {
    if (!verifyTreeContent(root)) {
        return;
    }
    wcout << "Please enter integers in deletion order: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    for (int value : values) {
        removeAVLNode(root, value);
    }

    present(root);
}

/**
 * @brief Use BSTBox implementation to print the tree content to console output.
 * 
 * @param root Tree's root node.
 */
void present(AVLNode* root) {
    if (!verifyTreeContent(root)) {
        return;
    }

    BSTBox* treeBox = createBSTBox(root);

    wcout << endl;
    printFrame(L"CURRENT TREE", FLAG_CLOSED);

    wcout << endl;
    presentBSTBox(wcout, treeBox);

    deleteBSTBox(treeBox);
}

/**
 * @brief Prompt user to put in the text file's name to store tree content as presented on console UI.
 * 
 * @param root Tree's root node.
 */
void exportToFile(AVLNode* root) {
    if (!verifyTreeContent(root)) {
        return;
    }
    wcout << "Please enter file name: ";
    string fileName;
    cin >> fileName;

    wofstream wofs;
    wofs.open(fileName, std::ofstream::out);
    wofs.imbue(std::locale(LOCALE)); // This locale must be similar to the console output's.

    if (!wofs.is_open()) {
        wcout << "Error opening file " << fileName.data() << endl;
        return;
    }

    BSTBox* box = createBSTBox(root);

    presentBSTBox(wofs, box); // Print the tree into output file stream instead of console output stream.

    wcout << "File exported successfully at " << fileName.data() << endl;

    wofs.close();
    deleteBSTBox(box);
}

/**
 * @brief Delete all nodes from the tree and set to null.
 * 
 * @param root Tree's root node.
 */
void resetCurrentTree(AVLNode*& root) {
    deleteAVLNode(root);
    verifyTreeContent(root);
}

/**
 * @brief Show a notice telling whether the tree is empty. 
 * 
 * This is to check before some tree's operations.
 * @return True If tree contains nodes, otherwise False.
 */
bool verifyTreeContent(AVLNode* root) {
    if (!root) {
        wcout << endl;
        printFrame(L"TREE IS EMPTY.", FLAG_CLOSED);
        return false;
    }

    return true;
}

/**
 * @brief Print the program name and actions that the program offers in a menu.
 * 
 * @return Letter represents the action to proceed.
 */
char printActionMenu() {
    wcout << endl;
    printFrame(L"BINARY SEARCH TREE CONSOLE VISUALIZATION", FLAG_TOP | FLAG_SIDE | FLAG_BOTTOM_T);
    printFrame(
LR"(Please choose one action below:
    > [C]reate a binary search tree from random nodes.
    > [I]nsert nodes to current tree.
    > [D]elete nodes from current tree.
    > [V]iew current tree.
    > [R]eset current tree.
    > [E]xport to text file.
    > [Q]uit.
Please enter your choice: [C|I|D|V|R|E|Q][ENTER]
)", FLAG_SIDE | FLAG_BOTTOM);

    char action;
    cin >> action;
    return action;
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
void printFrame(const wchar_t* text, int mask) {
    // Allocate output buffer memory for one line.
    wchar_t* buffer = new wchar_t[FRAME_WIDTH + 1]; 

    // Print the top line
    if (mask & FLAG_TOP) {
        wmemset(buffer, L'═', FRAME_WIDTH);
        buffer[0] = L'╔';
        buffer[FRAME_WIDTH - 1] = L'╗';
        buffer[FRAME_WIDTH] = '\0';
        wcout << buffer << endl;
    }
    
    // Split the text content into lines and print each line in a loop
    const wchar_t* found = std::wcschr(text, L'\n');
    const wchar_t* line = text;
    do {
        // Pre-fill the entire line with spaces
        wmemset(buffer, L' ', FRAME_WIDTH);

        // Left edge
        if (mask & FLAG_LEFT) {
            buffer[0] = L'║';
            buffer[1] = ' ';
        }

        // Copy line's text content into buffer, cut off at frame's end
        int lineLen = found ? (found - line) : wcslen(text);
        int copyLen = std::min(lineLen, FRAME_WIDTH - 4);
        std::wmemcpy(buffer + 2, line, copyLen);

        // Right edge
        if (mask & FLAG_RIGHT) {
            buffer[FRAME_WIDTH - 2] = ' ';
            buffer[FRAME_WIDTH - 1] = L'║';
        }

        // Print line's buffer into output stream
        buffer[FRAME_WIDTH] = '\0';
        wcout << buffer << endl;

        if (!found) {
            break;
        }
        line = found + 1;
        found = std::wcschr(line, L'\n');
    } while (found);

    // Print bottom line
    if (mask & FLAG_BOTTOM || mask & FLAG_BOTTOM_T) {
        wmemset(buffer, L'═', FRAME_WIDTH);
        if (mask & FLAG_BOTTOM_T) {
            buffer[0] = L'╠';
            buffer[FRAME_WIDTH - 1] = L'╣';
        } else {
            buffer[0] = L'╚';
            buffer[FRAME_WIDTH - 1] = L'╝';
        }
        buffer[FRAME_WIDTH] = '\0';
        wcout << buffer << endl;
    }
}

/**
 * @brief Read user input as an array of integers before returning when hitting a line break.
 * 
 * @return Vector of integers.
 */
vector<int> getInputIntegers() {
    vector<int> values;
    int response;
    if (cin.peek() == '\n') {
        cin.ignore();
    }
    while (cin.peek() != '\n') {
        cin >> response;
        values.push_back(response);
    }
    cin.ignore();
    return values;
}

// Initialize file logger for debugging.
void initializeLogging() {
    auto file_logger = spdlog::basic_logger_mt("BSTBox", "BSTBoxLogs.log");
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);
    file_logger->flush_on(spdlog::level::debug);
}