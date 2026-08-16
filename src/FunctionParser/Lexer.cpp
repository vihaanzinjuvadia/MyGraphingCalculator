//
// Created by Vihaan Zinjuvadia on 5/13/23.
//

#include "Lexer.h"
#include <cctype>
#include <stdexcept>

namespace {
    // Full double precision: std::to_string would round these to six decimals.
    const std::string PI_LITERAL {"3.14159265358979323846"};
    const std::string E_LITERAL  {"2.71828182845904523536"};

    // std::isdigit/std::isalpha are undefined for negative char values.
    bool is_digit(const char c) {
        return std::isdigit(static_cast<unsigned char>(c)) != 0;
    }
    bool is_alpha(const char c) {
        return std::isalpha(static_cast<unsigned char>(c)) != 0;
    }
    bool is_number_token(const std::string &token) {
        return !token.empty() && (is_digit(token.front()) || token.front() == '.');
    }
    bool is_variable_token(const std::string &token) {
        return token == variable1 || token == variable2;
    }
    bool ends_a_value(const std::string &token) {
        return token == ")" || is_number_token(token) || is_variable_token(token);
    }
    bool starts_a_value(const std::string &token) {
        return token == "(" || is_number_token(token) || is_variable_token(token) ||
               in(token, special_tokens);
    }
}

Lexer::Lexer(const std::string &function)
    : function {function}
    {}

std::vector<std::string> Lexer::tokenize() {
    std::vector<std::string> tokens {};

    for (std::size_t i {}; i < function.length(); i++) {
        const char at {function.at(i)};

        // Ignoring Whitespace And Other Characters
        if (at == ' ' || at == '\n' || at == '\t' || at == '\r') {
            continue;
        }

        // Handling Operators
        if (in(std::string(1, at), operators)) {
            tokens.push_back(std::string(1, at));
        }
        // Handling Variable
        else if (is_variable_token(std::string(1, at))) {
            tokens.push_back(std::string(1, at));
        }
        // Handling Numbers
        else if (is_digit(at) || at == '.') {
            std::string number {};
            bool seen_decimal_point {false};

            while (i < function.length()) {
                const char c {function.at(i)};
                if (is_digit(c)) {
                    number += c;
                } else if (c == '.') {
                    if (seen_decimal_point) {
                        throw std::runtime_error("NUMBER HAS MORE THAN ONE DECIMAL POINT: " + number + ".");
                    }
                    seen_decimal_point = true;
                    number += c;
                } else {
                    break;
                }
                i++;
            }
            // To make sure that i++ doesn't happen twice and that we skip over a token;
            i--;

            if (number == ".") {
                throw std::runtime_error("A LONE '.' IS NOT A NUMBER");
            }
            tokens.push_back(number);
        }
        // Handling Functions and Constants
        else if (is_alpha(at)) {
            std::string special_token {};
            while (i < function.length() && is_alpha(function.at(i))) {
                special_token += function.at(i);
                i++;
            }
            // To make sure that i++ doesn't happen twice and that we skip over a token;
            i--;

            // Function part
            if (in(special_token, special_tokens)) {
                tokens.push_back(special_token);
            } // Constants Part
              else if (in(special_token, constants)) {
                tokens.push_back(special_token == "pi" ? PI_LITERAL : E_LITERAL);
            } else {
                throw std::runtime_error("INVALID TOKEN: '" + special_token + "'");
            }
        }
        else {
            throw std::runtime_error("INVALID CHARACTER: '" + std::string(1, at) + "'");
        }
    }

    // Handling Implicit Multiplication, e.g. 2x, 3(x+1), xy, (x+1)(x-1), 2sin(x)
    // i + 1 < size() rather than i < size() - 1, which wraps around on empty input.
    for (std::size_t i {}; i + 1 < tokens.size(); i++) {
        if (ends_a_value(tokens.at(i)) && starts_a_value(tokens.at(i + 1))) {
            tokens.insert(tokens.begin() + static_cast<long>(i) + 1, "*");
            i++;
        }
    }

    // Check for correct number of parentheses
    int left_paren_count {};
    int right_paren_count {};

    for (const auto &token: tokens) {
        if (token == "(") {
            left_paren_count++;
        }
        else if (token == ")") {
            right_paren_count++;
        }

        if (right_paren_count > left_paren_count) {
            throw std::runtime_error("CANT HAVE MORE RIGHT PARENTHESES THAN LEFT!");
        }
    }
    if (left_paren_count != right_paren_count) {
        throw std::runtime_error("MUST HAVE EQUAL AMOUNT OF PARENTHESES");
    }

    return tokens;
}
