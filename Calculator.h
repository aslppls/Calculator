#pragma once

#include <iostream>
#include <stack>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class Calculator {
public:
    Calculator() = default;

    void evaluate(const std::string &expression, std::ostream &output_stream = std::cout);

private:
    std::string expression_{};
    std::vector<std::string> postfix_expression_{};
    std::stack<char> operations_stack_{};
    double result_ = 0;

    static std::unordered_map<char, int> priority_map_;

    std::string read_number(size_t &index);

    static int get_priority(const char &symbol) {
        if (priority_map_.find(symbol) != priority_map_.end()) {
            return priority_map_[symbol];
        }
        return -1;
    }

    void infix_to_postfix();

    void postfix_to_result();

    void clear_calculator();

    static double compute_operation(std::string &operation, double v1, double v2);

    friend void add_char_to_vector(std::vector<std::string> &vec, std::stack<char> &char_stack);

    static std::set<std::string> operations_;
};

