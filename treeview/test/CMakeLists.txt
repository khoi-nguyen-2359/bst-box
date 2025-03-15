set(CMAKE_CXX_STANDARD 14)

enable_testing()

find_package(GTest REQUIRED)

# include_directories("/opt/homebrew/opt/googletest/include")
# link_directories("/opt/homebrew/opt/googletest/lib")

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(TreeViewTest ${TEST_SOURCES})

target_link_libraries(TreeViewTest
    PRIVATE
        TreeView
        GTest::gtest
)

# include(GoogleTest)
gtest_discover_tests(TreeViewTest)