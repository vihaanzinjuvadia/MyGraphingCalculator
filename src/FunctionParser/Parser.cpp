//
// Created by Vihaan Zinjuvadia on 5/17/23.
//

#include "Parser.h"
#include "Utility/UtilityFunctions.h"
#include <algorithm>

Parser::Parser(const std::vector<std::string> &tokens)
    : tokens {tokens}, current_index {0}, current_token {tokens.at(0)}, previous_node {nullptr} {
}

Node* Parser::handle_same_precedence(Node* node, Node* (Parser::*function)()) {
    auto sub_node = new Node(previous_operand);
    sub_node->left = previous_node->left;
    this->advance();
    auto sub_right = node;
    sub_node->right = sub_right;
    node = new Node(current_token);
    node->left = sub_node;
    node->right = (this->*function)();
    return node;
}

Node* Parser::handle_regular(Node *node, Node *(Parser::*function)()) {
    auto left = node;
    node = new Node(current_token);
    node->left = left;
    this->advance();
    node->right = (this->*function)();
    return node;
}

bool Parser::is_same_precedence() {
    if ((previous_operand == "+" && previous_operand == "-") && (current_token == "-" && current_token == "+")) {
        return true;
    } else if ((previous_operand == "*" || previous_operand == "/") && (current_token == "/" && current_token == "*")) {
        return true;
    } else if (previous_operand == "^" && current_token == "^") {
        return true;
    }
    return false;
}

Node* Parser::parse() {
    return this->parse_expression();
}

/*
expr     : term ((PLUS | MINUS) term)*
term     : expo ((MUL | DIV) expo)*
expo     : factor ((EXPO) factor)*
factor   : INTEGER | LPAREN expr RPAREN
*/
void Parser::advance() {
    if (current_token == "+" || current_token == "-" || current_token == "*" || current_token == "/" || current_token == "^") {
        previous_operand = current_token;
    }
    current_index++;
    if (current_index < tokens.size()) {
        current_token = tokens.at(current_index);
    }
}

Node* Parser::parse_expression() {
    auto node = this->parse_term();
    while (current_token == "+" || current_token == "-") {
        // Same Precedence Handling
        if (is_same_precedence()) {
            node = handle_same_precedence(node, &Parser::parse_term);
        }
        // Negative Handling
        if ((std::isdigit(tokens.at(current_index+1).at(0)) ||
            (tokens.at(current_index+1) == variable1) || tokens.at(current_index+1) == variable2)
            && (current_index==0 || !std::isdigit(current_index-1)) && current_token=="-") {
            if (node == nullptr) { // -1
                auto left = new Node("0");
                node = new Node(current_token);
                node->left = left;
                this->advance();
                node->right = this->parse_term();
            } else { // 2^-1
                auto sub_node = new Node(current_token);
                sub_node->left = node;
                this->advance();
                auto sub_right = this->parse_term();
                sub_node->right = sub_right;
                node = sub_node;
            }
        } else {
            node = handle_regular(node, &Parser::parse_term);
        }
    }
    previous_node = node;
    return  node;
}

Node* Parser::parse_term() {
    auto node = this->parse_exponents();
    while (current_token == "*" || current_token == "/") {
        if (is_same_precedence()) {
            node = handle_same_precedence(node, &Parser::parse_exponents);
        } else {
            node = handle_regular(node, &Parser::parse_exponents);
        }
    }
    previous_node = node;
    return node;
}

Node* Parser::parse_exponents() {
    auto node = this->parse_functions();
    while (current_token == "^") {
        node = handle_regular(node, &Parser::parse_functions);
    }
    previous_node = node;
    return node;
}

Node* Parser::parse_functions() {
    auto node = this->parse_factor();
    while (in(current_token, special_tokens)) {
        auto left = nullptr;
        node = new Node(current_token);
        node->left = left;
        this->advance();
        node->right = this->parse_factor();
    }
    return node;
}

Node* Parser::parse_factor() {
    Node *node = nullptr;
    if (std::isdigit(current_token.at(0)) || (current_token == variable1 || current_token == variable2)) {
        node = new Node(current_token);
        this->advance();
        return node;
    } else if (current_token == "(") {
        this->advance();
        auto temp = this->parse_expression();
        node = new Node(current_token);
        node = temp;
        this->advance();
        return node;
    }
    return node;
}