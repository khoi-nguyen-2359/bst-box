#include "../../treeview/source/BinaryTreeNode.h"
#include "../../treeview/source/BinaryTreeDisplay.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::cin;

void initializeLogging();
void createNewTree(BinaryTreeNode &root);
void insertNodes(BinaryTreeNode &root);
void deleteNodes(BinaryTreeNode &root);
vector<int> getInputIntegers();

int main(int argc, char* argv[]) {
    initializeLogging();
    
    BinaryTreeNode root;
    while (true) {
        cout << "Please choose one operation below:" << endl;
        cout << "A. Create a new binary search tree from a list of integers" << endl;
        cout << "B. Add more nodes into the current tree" << endl;
        cout << "C. Remove some nodes from the current tree" << endl;
        cout << "D. Switch tree presentation" << endl;
        cout << "E. Exit" << endl;
        cout << "Your choice: [A|B|C...][ENTER]" << endl;
        char operation;
        cin >> operation;
        switch (operation) {
            case 'A':
            case 'a':
                createNewTree(root);
                break;

            case 'B':
            case 'b':
                insertNodes(root);
                break;

            case 'C':
            case 'c':
                deleteNodes(root);
                break;
            
            default:
                break;
        }
    }

    return 0;
}

void createNewTree(BinaryTreeNode &root) {
    cout << "Please enter a sequence of node values: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    debug("Creating new tree with values: ");
    for (int value : values) {
        debug(std::to_string(value));
    }
    cout << endl;
    root = BinaryTreeNode(values);

    BinaryTreeDisplay display(&root);
    cout << endl;
    display.present2();
    cout << endl;
}

void insertNodes(BinaryTreeNode &root) {
    cout << "Please enter values to insert: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    for (int value : values) {
        root.insert(value);
    }

    BinaryTreeDisplay display(&root);
    cout << endl;
    display.present2();
    cout << endl;
}

void deleteNodes(BinaryTreeNode &root) {
    cout << "Please enter values to delete: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    for (int value : values) {
        root.remove(value);
    }

    BinaryTreeDisplay display(&root);
    cout << endl;
    display.present2();
    cout << endl;
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