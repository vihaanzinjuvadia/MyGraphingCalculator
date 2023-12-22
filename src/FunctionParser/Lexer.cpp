//
// Created by Vihaan Zinjuvadia on 5/13/23.
//

#include "Lexer.h"
#include <iostream>

#define PI 3.1415926
#define E  2.7182818


Lexer::Lexer(const std::string &function)
    : function {function}
    {}

// just basic tokenizer rn, worry about functions later.
std::vector<std::string> Lexer::tokenize() {
    std::vector<std::string> tokens {};
    for (int i {}; i < function.length(); i++) {
        char at {function.at(i)};

        // Ignoring Whitespace And Other Characters
        if (at == ' ' || at == '\n' || at == '\t') {

        }
        // Handling Operators
        else if (in(std::string(1, at), operators)) {
            tokens.push_back(std::string(1, at));
        }
        // Handling Variable
        else if (at == variable1[0] || at == variable2[0]) {
            tokens.push_back(std::string(1, at));
        }
        // Handling Numbers, Constants and Later, Functions
        else {
            std::string number {};
            std::string special_token {};

            // Handling Number
            int decimal_point_count {};
            if (std::isdigit(at) || at == '.') {
                if (std::isdigit(at)) {
                    while (std::isdigit(at)) {
                        number += at;
                        i++;
                        at = function[i];
                        // decimal point check
                        if (at == '.') {
                            decimal_point_count++;
                            if (decimal_point_count == 1) {
                                number += at;
                                i++;
                                at = function[i];
                            }
                        }
                    }

                    tokens.push_back(number);
                    // To make sure that i++ doesn't happen twice and that we skip over a token;
                    i--;
                }
            }

            // Handling Function
            else if (std::isalpha(at)) {
                while (std::isalpha(at)) {
                    special_token.push_back(at);
                    i++;
                    at = function[i];
                }
                // To make sure that i++ doesn't happen twice and that we skip over a token;
                i--;

                // Function part
                if (in(special_token, special_tokens)) {
                    tokens.push_back(special_token);
                } // Constants Part
                  else if (in(special_token, constants)) {
                    if (special_token == "pi") {
                        special_token = std::to_string(PI);
                        tokens.push_back(special_token);
                    } else {
                        special_token = std::to_string(E);
                        tokens.push_back(special_token);
                    }
                } else {
                    std::cout << "INVALID TOKEN!: " << special_token << std::endl;
                    std::exit(1);
                }
            }
            else {
                std::cout << "INVALID TOKEN!: " << at << std::endl;
            }
        }
    }

    // Handling Implicit Multiplication
    for (int i {}; i < tokens.size()-1;i++) {
        auto at = tokens.at(i);
        auto next = tokens.at(i+1);
        // CASE: NUMBER (<VARIABLE || NUMBER, FUNCTION>)
        if (std::isdigit(at.at(0)) && next == "(") {
            tokens.insert(tokens.begin() + i + 1, "*");
        }
        // CASE: NUMBER <VARIABLE || FUNCTION>
        else if (std::isdigit(at.at(0)) && ((next.size() == 1 && (next.at(0) == variable1[0] || next.at(0) == variable2[0])) || in(next, special_tokens))) {
            tokens.insert(tokens.begin() + i + 1, "*");
        }
        // CASE: Variable Variable
        else if (((at.size() == 1 && (at.at(0) == variable1[0] || at.at(0) == variable2[0]))) && ((next.size() == 1 && (next.at(0) == variable1[0] || next.at(0) == variable2[0])))) {
            tokens.insert(tokens.begin() + i + 1, "*");
        }
        // CASE: Variable (<VARIABLE || FUNCTION>)
        else if (((at.size() == 1 && (at.at(0) == variable1[0] || at.at(0) == variable2[0]))) && next == "(") {
            tokens.insert(tokens.begin() + i + 1, "*");
        }
        // CASE: VARIABLE FUNCTION
        else if (((at.size() == 1 && (at.at(0) == variable1[0] || at.at(0) == variable2[0]))) && in(next, special_tokens)) {
            tokens.insert(tokens.begin() + i + 1, "*");
        }
        // CASE: RIGHT_PAREN (<VARIABLE || FUNCTION || LEFT_PAREN>)
        else if (at == ")" && ((next.size() == 1 && (next.at(0) == variable1[0] || next.at(0) == variable2[0])) || in(next, special_tokens) || next == "(")) {
            tokens.insert(tokens.begin() + i + 1, "*");
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
            throw std::logic_error("CANT HAVE MORE RIGHT PARENTHESES THAN LEFT!");
        }
    }
    if (left_paren_count != right_paren_count) {
        throw std::logic_error("MUST HAVE EQUAL AMOUNT OF PARENTHESES");
    }

    return tokens;
}