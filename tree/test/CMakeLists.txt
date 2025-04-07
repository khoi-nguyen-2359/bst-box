set(CMAKE_CXX_STANDARD 14)

enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(TreeViewTest ${TEST_SOURCES})

target_link_libraries(TreeViewTest
    PRIVATE
        TreeView
        GTest::gtest
)
target_include_directories(TreeViewTest PRIVATE ${CMAKE_SOURCE_DIR}/treeview/source)

gtest_discover_tests(TreeViewTest)