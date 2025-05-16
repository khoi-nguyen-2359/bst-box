#include "bstbox_utils.h"
#include "gtest/gtest.h"

class UtilsTest : public ::testing::Test {
};

TEST(UtilsTest, ToString_ValidInput) {
    char *result = bstbox_to_string(666);
    EXPECT_STREQ(result, "666");
    free(result);

    result = bstbox_to_string(-133759);
    EXPECT_STREQ(result, "-133759");
    free(result);

    result = bstbox_to_string(0);
    EXPECT_STREQ(result, "0");
    free(result);

    result = bstbox_to_string(1000000000);
    EXPECT_STREQ(result, "1000000000");
    free(result);

    result = bstbox_to_string(-1000000000);
    EXPECT_STREQ(result, "-1000000000");
    free(result);
}

