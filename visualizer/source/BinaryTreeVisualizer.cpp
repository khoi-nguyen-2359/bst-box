#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <iostream>
#include <stdlib.h>

#include "../../treeview/source/BST.h"
#include "../../treeview/source/BSTBox.h"

using std::string;
using std::wcout;
using std::cin;

void initializeLogging();
void randomizeNewTree(BSTNode*& root);
void insertNodes(BSTNode*& root);
void deleteNodes(BSTNode*& root);
void present(BSTNode* root);
vector<int> getInputIntegers();
bool verifyEmptyTree(BSTNode* root);

int main(int argc, char* argv[]) {
    initializeLogging();
    wcout.imbue(std::locale("en_US.UTF-8"));

    BSTNode* tree;
    while (true) {
        wcout << LR"(
╔═══════════════════════════════════════════════════════════════════════╗
║ BINARY SEARCH TREE COMMAND LINE VISUALIZATION                         ║
╠═══════════════════════════════════════════════════════════════════════╣
║ Please choose one task below:                                         ║
║ > Create a new [R]andom binary search tree.                           ║
║ > [I]nsert nodes into the current tree.                               ║
║ > [D]elete nodes from the current tree.                               ║
║ > [E]xit.                                                             ║
║ Please enter your choice: [R|I|D|E][ENTER]                            ║
╚═══════════════════════════════════════════════════════════════════════╝
)";
        char operation;
        cin >> operation;
        switch (operation) {
            case 'R':
            case 'r':
                randomizeNewTree(tree);
            break;

            case 'I':
            case 'i':
                insertNodes(tree);
                break;

            case 'D':
            case 'd':
                deleteNodes(tree);
                break;
            
            default:
                return 0;
        }
    }

    deleteBSTNode(tree);

    return 0;
}

void randomizeNewTree(BSTNode*& root) {
    const int MAX = 20;
    int numResponse;
    wcout << "Please enter number of node (not exceeding " << MAX << "): ";
    cin >> numResponse;

    srand(time(nullptr));
    vector<int> randValues;
    int nodeCount = min(MAX, numResponse);
    for (int i = 0; i < nodeCount; ++i) {
        randValues.push_back(rand() % 1000);
    }
    deleteBSTNode(root);
    root = createBSTNode(randValues.data(), nodeCount);

    present(root);
}

void insertNodes(BSTNode*& root) {
    wcout << "Please enter integers in insertion order: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    if (!root) {
        root = createBSTNode(values.data(), values.size());
    } else {
        for (int value : values) {
            insertBSTNode(root, value);
        }
    }

    present(root);
}

void deleteNodes(BSTNode*& root) {
    if (!verifyEmptyTree(root)) {
        return;
    }
    wcout << "Please enter integers in deletion order: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    for (int value : values) {
        removeBSTNode(root, value);
    }

    present(root);
}

void present(BSTNode* root) {
    if (!verifyEmptyTree(root)) {
        return;
    }

    BSTBox* treeBox = createBSTBox(root);
    wcout << LR"(
╔═══════════════════════════════════════════════════════════════════════╗
║ CURRENT TREE                                                          ║
╚═══════════════════════════════════════════════════════════════════════╝

)";

    presentBSTBox(wcout, treeBox);

    deleteBSTBox(treeBox);
}

void initializeLogging() {
    auto file_logger = spdlog::basic_logger_mt("TreePresenter", "TreePresenterLogs.txt");
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

bool verifyEmptyTree(BSTNode* root) {
    if (!root) {
        wcout << LR"(
╔═══════════════════════════════════════════════════════════════════════╗
║ TREE IS EMPTY!                                                        ║
╚═══════════════════════════════════════════════════════════════════════╝
)";
        return false;
    }

    return true;
}
