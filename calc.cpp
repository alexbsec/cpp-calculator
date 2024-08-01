#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <stack>
#include <algorithm>

std::vector<char> OPERATORS = {'+', '-', '*', '/'};

bool is_opening_bracket(char c) {
    return (c == '(' || c == '[' || c == '{');
}

bool is_closing_bracket(char c) {
    return (c == ')' || c == ']' || c == '}');
}

bool has_higher_precedence(char c1, char c2) {
    int precedence1 = (c1 == '+' || c1 == '-') ? 1 : 2;
    int precedence2 = (c2 == '+' || c2 == '-') ? 1 : 2;
    return precedence1 >= precedence2;
}

int calculate_postfix_exp(const std::vector<std::string>& postfix) {
    std::stack<int> operands;
    for (const std::string& token : postfix) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (operands.size() < 2) {
                std::cerr << "Error: not enough operands for operation '" << token << "'" << std::endl;
                return 0;
            }
            int val2 = operands.top(); operands.pop();
            int val1 = operands.top(); operands.pop();

            int res = 0;
            if (token == "+")
                res = val1 + val2;
            else if (token == "-")
                res = val1 - val2;
            else if (token == "*")
                res = val1 * val2;
            else if (token == "/") {
                if (val2 == 0) {
                    std::cerr << "Error: division by zero" << std::endl;
                    return 0;
                }
                res = val1 / val2;
            }
            operands.push(res);
        } else {
            try {
                operands.push(std::stoi(token));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: invalid number '" << token << "'" << std::endl;
                return 0;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: number out of range '" << token << "'" << std::endl;
                return 0;
            }
        }
    }

    if (operands.size() != 1) {
        std::cerr << "Error: invalid postfix expression" << std::endl;
        return 0;
    }

    return operands.top();
}

std::vector<std::string> infix_to_postfix(const std::string& infix_exp) {
    std::vector<std::string> postfix;
    std::stack<char> operators;
    std::string number_buffer;

    for (size_t i = 0; i < infix_exp.length(); ++i) {
        char token = infix_exp[i];

        if (std::isspace(token)) {
            continue; // Skip spaces
        }

        if (std::isdigit(token)) {
            number_buffer += token;
        } else {
            if (!number_buffer.empty()) {
                postfix.push_back(number_buffer);
                number_buffer.clear();
            }

            // Handle unary minus
            if (token == '-') {
                // Check previous non-space character to determine if minus is unary
                bool is_unary = (i == 0);
                for (int j = i - 1; j >= 0; --j) {
                    if (std::isspace(infix_exp[j])) {
                        continue;
                    } else if (is_opening_bracket(infix_exp[j]) || std::count(OPERATORS.begin(), OPERATORS.end(), infix_exp[j])) {
                        is_unary = true;
                        break;
                    } else {
                        is_unary = false;
                        break;
                    }
                }

                if (is_unary) {
                    postfix.push_back("0");
                    operators.push('-');
                } else {
                    while (!operators.empty() && !is_opening_bracket(operators.top()) &&
                           has_higher_precedence(operators.top(), token)) {
                        postfix.push_back(std::string(1, operators.top()));
                        operators.pop();
                    }
                    operators.push(token);
                }
            } else if (std::count(OPERATORS.begin(), OPERATORS.end(), token)) {
                while (!operators.empty() && !is_opening_bracket(operators.top()) &&
                       has_higher_precedence(operators.top(), token)) {
                    postfix.push_back(std::string(1, operators.top()));
                    operators.pop();
                }
                operators.push(token);
            } else if (is_opening_bracket(token)) {
                operators.push(token);
            } else if (is_closing_bracket(token)) {
                while (!operators.empty() && !is_opening_bracket(operators.top())) {
                    postfix.push_back(std::string(1, operators.top()));
                    operators.pop();
                }
                if (!operators.empty()) operators.pop(); // Pop the opening bracket
            } else {
                std::cerr << "Error: unexpected token '" << token << "'" << std::endl;
                return {}; 
            }
        }
    }

    if (!number_buffer.empty()) {
        postfix.push_back(number_buffer);
    }

    while (!operators.empty()) {
        postfix.push_back(std::string(1, operators.top()));
        operators.pop();
    }

    return postfix;
}

int main() {
    std::string exp;
    while (true) {
        std::cout << "Enter expression: ";
        std::getline(std::cin, exp);
        std::vector<std::string> postfix = infix_to_postfix(exp);

        std::cout << "Postfix: ";
        for (const std::string& token : postfix) {
            std::cout << token << " ";
        }
        std::cout << std::endl;

        int res = calculate_postfix_exp(postfix);
        std::cout << "Result: " << res << std::endl;
    }

    return 0;
}
