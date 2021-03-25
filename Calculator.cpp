#include "Calculator.h"

#include <cmath>
#include <iomanip>

enum kSymbol {
    number,
    operation,
    start,
    floating_symbol,
    opening_bracket,
    ending_bracket
};

std::unordered_map<char, int> Calculator::priority_map_ = {
        {'+', 0},
        {'-', 0},
        {'*', 1},
        {'/', 1},
};

std::set<std::string> Calculator::operations_ = {
        "+", "-", "/", "*"
};

void add_char_to_vector(std::vector<std::string> &vec, std::stack<char> &char_stack) {
    std::string op;
    op.push_back(char_stack.top());
    vec.push_back(op);
    char_stack.pop();
}

void Calculator::evaluate(const std::string &expression, std::ostream &output_stream) {
    expression_ = expression;
    infix_to_postfix();
    postfix_to_result();

    int last_nums = static_cast<int>(result_ * 100) % 100;


    if (last_nums == 0) {
        output_stream << std::fixed << std::setprecision(0) << result_;
    } else if (last_nums % 10 == 0) {
        output_stream << std::fixed << std::setprecision(1) << result_;
    } else {
        output_stream << std::fixed << std::setprecision(2) << result_;
    }
    clear_calculator();
}

void Calculator::infix_to_postfix() {
    kSymbol prev_symbol = start;
    std::string digit;
    size_t index = 0;
    bool signed_value_flag = false;

    while (index < expression_.size()) {
        if (expression_[index] == ' ') {
            ++index;
            continue;
        }

        if (isdigit(expression_[index])) {
            if (prev_symbol == number || prev_symbol == ending_bracket || prev_symbol == floating_symbol) {
                throw std::runtime_error("Invalid expression syntax");
            }

            digit += read_number(index);
            prev_symbol = number;
        }

        if (index == expression_.size() && !digit.empty()) {
            postfix_expression_.push_back(digit);
            break;
        }

        if (expression_[index] == '.' | expression_[index] == ',') {
            if (prev_symbol == floating_symbol || prev_symbol == ending_bracket) {
                throw std::runtime_error("Invalid expression syntax");
            }
            if (digit.empty()) {
                digit = "0";
            }
            digit += '.';

            prev_symbol = floating_symbol;
            ++index;
            digit += read_number(index);
        }

        if (!digit.empty()) {
            postfix_expression_.push_back(digit);
            digit = "";
            signed_value_flag = false;

        } else if (signed_value_flag) {
            throw std::runtime_error("Invalid expression syntax");

        } else if (expression_[index] == '(') {
            if (prev_symbol == ending_bracket) {
                throw std::runtime_error("Invalid expression syntax");
            }
            prev_symbol = opening_bracket;

            operations_stack_.emplace('(');
            ++index;

        } else if (expression_[index] == ')') {
            if (prev_symbol == operation || prev_symbol == start || prev_symbol == opening_bracket) {
                throw std::runtime_error("Invalid expression syntax");
            }
            prev_symbol = ending_bracket;

            while (operations_stack_.top() != '(' && !operations_stack_.empty()) {
                add_char_to_vector(postfix_expression_, operations_stack_);
            }
            if (operations_stack_.empty()) {
                throw std::runtime_error("Invalid expression syntax");
            }

            operations_stack_.pop();
            ++index;

        } else if (priority_map_.find(expression_[index]) != priority_map_.end()) {
            if ((prev_symbol == operation || prev_symbol == opening_bracket || prev_symbol == start) &&
                (expression_[index] == '+' || expression_[index] == '-')) {
                digit = "0";
                postfix_expression_.emplace_back(digit);
                digit = "";
                signed_value_flag = true;
                operations_stack_.push(expression_[index]);
                ++index;
                prev_symbol = operation;
                continue;
            }

            if (prev_symbol == operation) {
                throw std::runtime_error("Invalid expression syntax");
            }
            prev_symbol = operation;

            while (!operations_stack_.empty() && get_priority(expression_[index]) <= get_priority(operations_stack_.top())) {
                add_char_to_vector(postfix_expression_, operations_stack_);
            }

            operations_stack_.push(expression_[index]);
            ++index;
        } else {
            throw std::runtime_error(&"Unknown symbol: "[expression_[index]]);
        }
    }

    if (prev_symbol == opening_bracket || prev_symbol == operation) {
        throw std::runtime_error("Invalid expression syntax");
    }

    while (!operations_stack_.empty()) {
        if (operations_stack_.top() == '(') {
            throw std::runtime_error("Invalid expression syntax");
        }
        add_char_to_vector(postfix_expression_, operations_stack_);
    }

}

std::string Calculator::read_number(size_t &index) {
    std::string digit;
    while (index < expression_.size() && isdigit(expression_[index])) {
        digit += expression_[index];
        ++index;
    }

    return digit;
}

void Calculator::postfix_to_result() {
    std::stack<double> evaluation_stack;
    for (auto &value : postfix_expression_) {
        if (operations_.find(value) != operations_.end()) {
            double value2 = evaluation_stack.top();
            evaluation_stack.pop();
            double value1 = evaluation_stack.top();
            evaluation_stack.pop();
            double result = compute_operation(value, value1, value2);
            evaluation_stack.push(result);
        } else {
            double number = std::stod(value, nullptr);
            evaluation_stack.push(number);
        }
    }

    if (evaluation_stack.size() != 1) {
        throw std::runtime_error("Invalid expression syntax");
    }

    result_ = evaluation_stack.top();
}

double Calculator::compute_operation(std::string &operation, double v1, double v2) {
    if (operation == "+") {
        return v1 + v2;
    }

    if (operation == "-") {
        return v1 - v2;
    }

    if (operation == "*") {
        return v1 * v2;
    }

    if (v2 == 0) {
        throw std::runtime_error("Zero division");
    }

    return v1 / v2;
}

void Calculator::clear_calculator() {
    expression_ = "";
    postfix_expression_.clear();
    operations_stack_ = {};
}