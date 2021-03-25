#include <sstream>
#include "gtest/gtest.h"
#include "../Calculator.h"


double GetResult(const std::string &expression) {
    std::stringstream ss;
    Calculator calculator;

    calculator.evaluate(expression, ss);
    std::string result;
    ss >> result;
    return std::stod(result);
}

TEST(EvaluateExpressionTest, CorrectInputInt) {
    EXPECT_EQ(GetResult("-1 + 5 - 3"), 1);

    EXPECT_EQ(GetResult("3"), 3);

    EXPECT_EQ(GetResult("1+2"), 3);

    EXPECT_EQ(GetResult("1+2*3"), 7);

    EXPECT_EQ(GetResult("1+3+(5*6)/10+8"), 15);

    EXPECT_EQ(GetResult("1  +3+   (5 *6)/  10+8"), 15);

    EXPECT_EQ(GetResult("7+(7+(7+(7+(7-7)*10)))"), 28);

    EXPECT_EQ(GetResult("(((10*(7+7)+7)+7)+7)+7"), 168);
}

TEST(EvaluateExpressionTest, CorrectInputDouble) {
    EXPECT_EQ(GetResult("-10 + (8*2.5) - (3 / 1,5)"), 8);

    EXPECT_EQ(GetResult("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)"), 11);

    EXPECT_EQ(GetResult("1 + 1."), 2);

    EXPECT_EQ(GetResult("1 + 1,"), 2);

    EXPECT_EQ(GetResult("(2+3.)*10.8"), 54);

    EXPECT_EQ(GetResult("10.7+10.7*9.56"), 112.99);

    EXPECT_EQ(GetResult(" 1 /   3"), 0.33);

    EXPECT_EQ(GetResult("0*0+.9"), 0.9);

    EXPECT_EQ(GetResult("0*0+(-.9)"), -0.9);

    EXPECT_EQ(GetResult("1*0+(,9)"), 0.9);

    EXPECT_EQ(GetResult("1*0+(+.9)"), 0.9);

    EXPECT_EQ(GetResult("0. + (.9) * -5"), -4.5);
}

TEST(EvaluateExpressionTest, IncorrectInput) {
    EXPECT_ANY_THROW(GetResult("+"));

    EXPECT_ANY_THROW(GetResult("1/0"));

    EXPECT_ANY_THROW(GetResult("(1+2"));

    EXPECT_ANY_THROW(GetResult(")+4"));

    EXPECT_ANY_THROW(GetResult("5 + 5.6.5"));

    EXPECT_ANY_THROW(GetResult("0. + "));

    EXPECT_ANY_THROW(GetResult("0. + a"));

    EXPECT_ANY_THROW(GetResult("7jq"));

    EXPECT_ANY_THROW(GetResult("a + bc"));
}
