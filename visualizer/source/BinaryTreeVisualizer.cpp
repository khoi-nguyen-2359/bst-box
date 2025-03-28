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

int main(int argc, char* argv[]) {
    initializeLogging();
    
    while (true) {
        cout << "Key in node values: [int1] [int2] ... [intN][ENTER]" << endl;
        vector<int> values;
        int response;
        while (cin.peek() != '\n') {
            cin >> response;
            values.push_back(response);
        }
        cin.ignore();
    
        BinaryTreeNode root(values);
        spdlog::debug("Create root");

        BinaryTreeDisplay display(&root);
        cout << endl;
        display.present2();
        cout << endl;
    }
    return 0;
}

void initializeLogging() {
    auto file_logger = spdlog::basic_logger_mt("TreePresenter", "TreePresenterLogs.txt");
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);
    file_logger->flush_on(spdlog::level::debug);
}