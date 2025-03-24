#include "../../treeview/source/BinaryTreeNode.h"
#include "../../treeview/source/BinaryTreeDisplay.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <iostream>

using std::string;
using std::cout;

void initializeLogging();

int main(int argc, char* argv[]) {
    initializeLogging();
    int values1[] = {8,4,12,2,6,10,14,1,3,5,7,9,11,13,15};
    int values2[] = {8,4,12,2,6,10,14,1,3,5,11,13,15};
    int values3[] = {8,4,12,2,6,10,14,3,5,11,15};
    int values4[] = {8,4,12,2,14};
    int values5[] = {8,4,12,2,14,10};
    int values6[] = {84,122,1410};
    BinaryTreeNode root(values6, 3);
    spdlog::debug("Create root");
    // root.insert(5);
    // root.insert(15);
    // root.insert(12);
    // root.insert(13);
    // root.insert(14);
    // root.insert(16);
    // root.insert(1);

    BinaryTreeDisplay display(&root);
    display.present();
    cout << endl;

    return 0;
}

void initializeLogging() {
    spdlog::set_level(spdlog::level::debug);
    auto file_logger = spdlog::basic_logger_mt("TreePresenter", "TreePresenterLogs.txt");
    spdlog::set_default_logger(file_logger);
  }