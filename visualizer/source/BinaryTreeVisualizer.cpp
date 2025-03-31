#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <iostream>

#include "../../treeview/source/BST.h"
#include "../../treeview/source/BSTBox.h"

using std::string;
using std::wcout;
using std::cin;

void initializeLogging();
void createNewTree(BSTNode*& root);
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
║ [C]reate a new binary search tree.                                    ║
║ [I]nsert nodes into the current tree.                                 ║
║ [D]elete nodes from the current tree.                                 ║
║ [E]xit.                                                               ║
║ Please enter your choice: [C|I|D|E][ENTER]                            ║
╚═══════════════════════════════════════════════════════════════════════╝
)";
        char operation;
        cin >> operation;
        switch (operation) {
            case 'C':
            case 'c':
                createNewTree(tree);
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

void createNewTree(BSTNode*& root) {
    wcout << "Please enter integers in insertion order: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    debug("Creating new tree with values: ");
    for (int value : values) {
        debug(std::to_string(value));
    }
    wcout << endl;
    root = createBSTNode(values.data(), values.size());

    present(root);
}

void insertNodes(BSTNode*& root) {
    wcout << "Please enter integer in insertion order: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    for (int value : values) {
        if (!root) {
            root = createBSTNode(value);
        }
        insertBSTNode(root, value);
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
║ EXISTING TREE                                                         ║
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
