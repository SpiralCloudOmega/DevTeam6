#include <gtest/gtest.h>

/**
 * @brief Basic test example
 */
TEST(BasicTest, Addition) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_EQ(2 + 2, 4);
}

/**
 * @brief Test with multiple assertions
 */
TEST(BasicTest, Comparison) {
    int value = 42;
    
    EXPECT_GT(value, 0);
    EXPECT_LT(value, 100);
    EXPECT_EQ(value, 42);
    EXPECT_NE(value, 0);
}

/**
 * @brief String test example
 */
TEST(StringTest, Equality) {
    std::string str1 = "Hello";
    std::string str2 = "Hello";
    std::string str3 = "World";
    
    EXPECT_EQ(str1, str2);
    EXPECT_NE(str1, str3);
    EXPECT_STREQ(str1.c_str(), "Hello");
}

/**
 * @brief Test fixture example
 */
class CalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code before each test
        result = 0;
    }

    void TearDown() override {
        // Cleanup code after each test
    }

    int result;
};

TEST_F(CalculatorTest, MultiplyByZero) {
    result = 5 * 0;
    EXPECT_EQ(result, 0);
}

TEST_F(CalculatorTest, MultiplyPositiveNumbers) {
    result = 3 * 4;
    EXPECT_EQ(result, 12);
}

// Main function is provided by gtest_main
