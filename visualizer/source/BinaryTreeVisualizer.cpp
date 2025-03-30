#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <iostream>

#include "../../treeview/source/BST.h"
// #include "../../treeview/source/BinaryTreeDisplay.h"
#include "../../treeview/source/BSTBoxy.h"

using std::string;
using std::wcout;
// using std::cout;
using std::cin;

void initializeLogging();
void createNewTree(BSTNode*& root);
void insertNodes(BSTNode* root);
void deleteNodes(BSTNode*& root);
void present(BSTNode* root);
vector<int> getInputIntegers();

int main(int argc, char* argv[]) {
    initializeLogging();
    // std::locale::global(std::locale("en_US.UTF-8")); // Set global locale
    std::wcout.imbue(std::locale("en_US.UTF-8")); // Apply locale to wcout

    // wchar_t* wideStr = new wchar_t[20];
    // // wideStr[0] = L'║';
    // // wideStr[1] = L'\0';
    // for (int i = 0; i < 10; ++i) {
    //     wideStr[i] = L'A' + i; // Assign characters 'A' to 'J'
    // }
    // wideStr[10] = L'║';
    // wideStr[11] = L'A'; // Null-terminate the wide string
    // wideStr[12] = L'\0'; // Null-terminate the wide string

    // // wprintf(L"Wide string content: %lc\n\n", wideStr[10]);
    // // std::wcout << L"Wide string content: " << wideStr << std::endl;
    // // wcout << wideStr << endl;
    // cout << (int)(wideStr[10]) << endl;
    // cout << (wchar_t)(L'║') << endl;
    // string str = "Hello, ║ 世界!";
    // const char* data = str.data();
    // cout << data << endl;
    // while (*data != '\0') {
    //     cout << *data << " ";
    //     data++;
    // }
    // cout << data[7] << endl;

    // std::wstring wstr = L"Hello, ║ 世界!";
    // wchar_t* wdata = new wchar_t[20]{L'║', L'\0'};
    // wcout << wdata << endl;
    
    // while (*wdata != L'\0') {
    //     wcout << *wdata << " ";
    //     wdata++;
    // }
    // wcout << wdata[7] << endl;
    
    BSTNode* tree;
    while (true) {

        // Assign a wide string
        // wcscpy(wideStr, L"Hello, 世界!");  
    
        // Print using std::wcout
        // std::wprintf(wideStr);
        // wchar_t* wstr = L"Hello, 世界!\0";
        // char str[] = "Hello, 世║!\0";
        // str[4] = L'║';
        // cout << "║" << std::endl;
        // cout << "Hello, 世界!" << std::endl;
        // cout << (wchar_t)str[8] << std::endl;

        // wcout << wstr << endl;
        wcout << "A. Create a new binary search tree" << endl;
        wcout << "B. Add more nodes into the current tree" << endl;
        wcout << "C. Remove some nodes from the current tree" << endl;
        wcout << "D. Switch tree presentation" << endl;
        wcout << "E. Exit" << endl;
        wcout << "Your choice: [A|B|C...][ENTER]" << endl;
        char operation;
        cin >> operation;
        switch (operation) {
            case 'A':
            case 'a':
                createNewTree(tree);
                break;

            case 'B':
            case 'b':
                insertNodes(tree);
                break;

            case 'C':
            case 'c':
                deleteNodes(tree);
                break;
            
            default:
                return 0;
        }
    }

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
    root = createNode(values.data(), values.size());

    present(root);
}

void insertNodes(BSTNode* root) {
    wcout << "Please enter values to insert: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    for (int value : values) {
        insert(root, value);
    }

    present(root);
}

void deleteNodes(BSTNode*& root) {
    wcout << "Please enter values to delete: [int1] [int2] ... [intN][ENTER]" << endl;
    vector<int> values = getInputIntegers();
    for (int value : values) {
        remove(root, value);
    }

    present(root);
}

void present(BSTNode* root) {
    BSTBoxyNode* boxyRoot = createBoxyNode(root);
    wcout << endl;
    measure(boxyRoot);
    present(wcout, boxyRoot);
    wcout << endl;

    deleteBoxyTree(boxyRoot);
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