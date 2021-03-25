#include <iostream>
#include "Calculator.h"

std::string ReadString(std::istream &input_stream = std::cin) {
    std::string input_string;
    input_stream >> input_string;
    return input_string;
}

int main() {
    std::string string_expression = ReadString();
    Calculator new_calculator;

    try {
        new_calculator.evaluate(string_expression);
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}