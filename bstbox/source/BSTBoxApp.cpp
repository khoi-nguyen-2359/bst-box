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

#define LOCALE "en_US.UTF-8"
#define FRAME_WIDTH 60

#define FLAG_TOP 0b00001
#define FLAG_LEFT 0b00010
#define FLAG_RIGHT 0b00100
#define FLAG_BOTTOM 0b01000
#define FLAG_SIDE FLAG_LEFT | FLAG_RIGHT
#define FLAG_BOTTOM_T 0b10000
#define FLAG_CLOSED 0b1111

void initializeLogging();
void createRandomTree(AVLNode*& root);
void insertNodes(AVLNode*& root);
void deleteNodes(AVLNode*& root);
void present(AVLNode* root);
void exportToFile(AVLNode* root);
void viewCurrentTree(AVLNode* root);
void resetCurrentTree(AVLNode*& root);
vector<int> getInputIntegers();
bool verifyTreeContent(AVLNode* root);
char printActionMenu();
void printFrame(const wchar_t* text, int mask);

int main(int argc, char* argv[]) {
    initializeLogging();
    wcout.imbue(std::locale(LOCALE));

    AVLNode* tree;
    char action;
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
                viewCurrentTree(tree);
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

void createRandomTree(AVLNode*& root) {
    const int MAX = 20;
    int numResponse;
    wcout << "Please enter number of nodes (not exceeding " << MAX << "): ";
    cin >> numResponse;

    srand(time(nullptr));
    vector<int> randValues;
    int nodeCount = std::min(MAX, numResponse);
    for (int i = 0; i < nodeCount; ++i) {
        randValues.push_back(rand() % 1000 - 500);
    }
    deleteAVLNode(root);
    insertAVLNodes(root, randValues.data(), nodeCount);

    present(root);
}

void insertNodes(AVLNode*& root) {
    wcout << "Please enter integers in insertion order: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    insertAVLNodes(root, values.data(), values.size());
    present(root);
}

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

void exportToFile(AVLNode* root) {
    if (!verifyTreeContent(root)) {
        return;
    }
    wcout << "Please enter file name: ";
    string fileName;
    cin >> fileName;
    wofstream wofs;
    wofs.open(fileName, std::ofstream::out);
    wofs.imbue(std::locale(LOCALE));
    if (!wofs.is_open()) {
        wcout << "Error opening file " << fileName.data() << endl;
        return;
    }

    BSTBox* box = createBSTBox(root);
    presentBSTBox(wofs, box);

    wcout << "File exported successfully at " << fileName.data() << endl;

    wofs.close();
    deleteBSTBox(box);
}

void viewCurrentTree(AVLNode* root) {
    present(root);
}

void resetCurrentTree(AVLNode*& root) {
    deleteAVLNode(root);
    root = nullptr;
    verifyTreeContent(root);
}

void present(AVLNode* root) {
    wcout << endl;
    if (!verifyTreeContent(root)) {
        return;
    }

    BSTBox* treeBox = createBSTBox(root);
    printFrame(L"CURRENT TREE", FLAG_CLOSED);

    presentBSTBox(wcout, treeBox);

    deleteBSTBox(treeBox);
}

void initializeLogging() {
    auto file_logger = spdlog::basic_logger_mt("BSTBox", "BSTBoxLogs.log");
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);
    file_logger->flush_on(spdlog::level::debug);
}

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

bool verifyTreeContent(AVLNode* root) {
    if (!root) {
        printFrame(L"TREE IS EMPTY.", FLAG_CLOSED);
        return false;
    }

    return true;
}

char printActionMenu() {
    printFrame(L"BINARY SEARCH TREE COMMAND LINE VISUALIZATION", FLAG_TOP | FLAG_SIDE | FLAG_BOTTOM_T);
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

void printFrame(const wchar_t* text, int mask) {
    wchar_t* buffer = new wchar_t[FRAME_WIDTH + 1];
    if (mask & FLAG_TOP) {
        wmemset(buffer, L'═', FRAME_WIDTH);
        buffer[0] = L'╔';
        buffer[FRAME_WIDTH - 1] = L'╗';
        buffer[FRAME_WIDTH] = '\0';
        wcout << buffer << endl;
    }
    
    const wchar_t* found = std::wcschr(text, L'\n');
    const wchar_t* line = text;
    do {
        wmemset(buffer, L' ', FRAME_WIDTH);
        if (mask & FLAG_LEFT) {
            buffer[0] = L'║';
            buffer[1] = ' ';
        }

        int lineLen = found ? (found - line) : wcslen(text);
        int copyLen = std::min(lineLen, FRAME_WIDTH - 4);
        std::wmemcpy(buffer + 2, line, copyLen);

        if (mask & FLAG_RIGHT) {
            buffer[FRAME_WIDTH - 2] = ' ';
            buffer[FRAME_WIDTH - 1] = L'║';
        }

        buffer[FRAME_WIDTH] = '\0';
        wcout << buffer << endl;

        if (!found) {
            break;
        }
        line = found + 1;
        found = std::wcschr(line, L'\n');
    } while (found);

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