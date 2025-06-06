#include "bstbox_input.h"
#include "gtest/gtest.h"

class InputTest : public ::testing::Test {
};

TEST(InputTest, ReadInputInts_ValidInput) {
    char input[] = "10 20 30 40 50 60 70 80 90 100\n";

    size_t size = 10;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 10);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
    EXPECT_EQ(arr[3], 40);
    EXPECT_EQ(arr[4], 50);
    EXPECT_EQ(arr[5], 60);
    EXPECT_EQ(arr[6], 70);
    EXPECT_EQ(arr[7], 80);
    EXPECT_EQ(arr[8], 90);
    EXPECT_EQ(arr[9], 100);
    free(arr);
}

TEST(InputTest, ReadInputInts_PartialInput) {
    char input[] = "5 10 15 20\n";

    size_t size = 10;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 4);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 10);
    EXPECT_EQ(arr[2], 15);
    EXPECT_EQ(arr[3], 20);
    free(arr);
}

TEST(InputTest, ReadInputInts_NoInput) {
    char input[] = "\n";

    size_t size = 4;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 0);
    EXPECT_EQ(arr, nullptr);
}

TEST(InputTest, ReadInputInts_ExcessNumberOfValues) {
    char input[] = "1 2 3 4 5 6\n";

    size_t size = 4;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 6);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[4], 5);
    EXPECT_EQ(arr[5], 6);
    free(arr);
}

TEST(InputTest, ReadInputInts_CommaSeparatedInts) {
    char input[] = "1, 2, 3, 4, 5, 6, 7, 8, 9, 10,\n";

    size_t size = 0;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 10);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[4], 5);
    EXPECT_EQ(arr[5], 6);
    EXPECT_EQ(arr[6], 7);
    EXPECT_EQ(arr[7], 8);
    EXPECT_EQ(arr[8], 9);
    EXPECT_EQ(arr[9], 10);
    free(arr);
}

TEST(InputTest, ReadInputInts_InvalidNumbersInFront) {
    char input[] = "abc 123 def 456 ghi 789\n";

    size_t size = 10;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 3);
    EXPECT_EQ(arr[0], 123);
    EXPECT_EQ(arr[1], 456);
    EXPECT_EQ(arr[2], 789);
    free(arr);
}

TEST(InputTest, ReadInputInts_NegativeAndDecimal) {
    char input[] = "42 -15 0 3.14 100\n";

    size_t size = 10;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 6);
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[1], -15);
    EXPECT_EQ(arr[2], 0);
    EXPECT_EQ(arr[3], 3);
    EXPECT_EQ(arr[4], 14);
    EXPECT_EQ(arr[5], 100);
    free(arr);
}

TEST(InputTest, ReadInputInts_EmptyString) {
    char input[] = "";

    size_t size = 5;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 0);
    EXPECT_EQ(arr, nullptr);
}

TEST(InputTest, ReadInputInts_SpacesOnly) {
    char input[] = "     \n";

    size_t size = 5;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 0);
    EXPECT_EQ(arr, nullptr);
}

TEST(InputTest, ReadInputInts_SpecialCharacters) {
    char input[] = "!@# $%^ &*(123)\n";

    size_t size = 10;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 1);
    EXPECT_EQ(arr[0], 123);
    free(arr);
}

TEST(InputTest, ReadInputInts_MultipleSeparatingSpaces) {
    char input[] = "   12   abc   -34  56.78   90   xyz   \n";

    size_t size = 10;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 5);
    EXPECT_EQ(arr[0], 12);
    EXPECT_EQ(arr[1], -34);
    EXPECT_EQ(arr[2], 56);
    EXPECT_EQ(arr[3], 78);
    EXPECT_EQ(arr[4], 90);
    free(arr);
}


TEST(InputTest, ReadInputInts_MultipleLines) {
    char input[] = "   12 \n  abc   \n -34 \n";

    size_t size = 0;
    int *arr = bstbox_read_ints(input, &size);

    EXPECT_EQ(size, 1);
    EXPECT_EQ(arr[0], 12);
    free(arr);
}
