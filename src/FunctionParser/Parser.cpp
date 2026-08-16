//
// Created by Vihaan Zinjuvadia on 5/17/23.
//

#include "Parser.h"
#include "Utility/UtilityFunctions.h"
#include <cctype>
#include <stdexcept>

namespace {
    bool is_number_token(const std::string &token) {
        if (token.empty()) {
            return false;
        }
        const unsigned char first = static_cast<unsigned char>(token.front());
        return std::isdigit(first) || token.front() == '.';
    }
}

Parser::Parser(const std::vector<std::string> &tokens)
    : tokens {tokens}, current_token {}, current_index {0} {
    if (!this->tokens.empty()) {
        current_token = this->tokens.front();
    }
}

bool Parser::at_end() const {
    return current_index >= tokens.size();
}

void Parser::advance() {
    current_index++;
    current_token = at_end() ? std::string {} : tokens.at(current_index);
}

void Parser::expect(const std::string &token) {
    if (at_end() || current_token != token) {
        throw std::runtime_error("EXPECTED '" + token + "' BUT FOUND '" +
                                 (at_end() ? std::string("END OF EXPRESSION") : current_token) + "'");
    }
    advance();
}

Node* Parser::parse() {
    if (tokens.empty()) {
        throw std::runtime_error("EMPTY EXPRESSION");
    }

    auto tree = parse_expression();

    // Leftover tokens mean the input wasn't a single expression, e.g. "1 2".
    if (!at_end()) {
        throw std::runtime_error("UNEXPECTED TOKEN: '" + current_token + "'");
    }

    return tree.release();
}

std::unique_ptr<Node> Parser::parse_expression() {
    auto node = parse_term();
    while (!at_end() && (current_token == "+" || current_token == "-")) {
        auto op = std::make_unique<Node>(current_token);
        advance();
        auto right = parse_term();
        op->left = node.release();
        op->right = right.release();
        node = std::move(op);
    }
    return node;
}

std::unique_ptr<Node> Parser::parse_term() {
    auto node = parse_unary();
    while (!at_end() && (current_token == "*" || current_token == "/")) {
        auto op = std::make_unique<Node>(current_token);
        advance();
        auto right = parse_unary();
        op->left = node.release();
        op->right = right.release();
        node = std::move(op);
    }
    return node;
}

std::unique_ptr<Node> Parser::parse_unary() {
    if (!at_end() && (current_token == "+" || current_token == "-")) {
        const bool negate {current_token == "-"};
        advance();
        auto operand = parse_unary();
        if (!negate) {
            return operand;
        }
        // 0 - operand, so the evaluator only ever sees binary operators.
        auto op = std::make_unique<Node>("-");
        op->left = new Node("0");
        op->right = operand.release();
        return op;
    }
    return parse_power();
}

std::unique_ptr<Node> Parser::parse_power() {
    auto node = parse_atom();
    if (!at_end() && current_token == "^") {
        advance();
        // parse_unary keeps ^ right associative and lets the exponent carry a sign,
        // so 2^3^2 and 2^-1 both come out right.
        auto right = parse_unary();
        auto op = std::make_unique<Node>("^");
        op->left = node.release();
        op->right = right.release();
        return op;
    }
    return node;
}

std::unique_ptr<Node> Parser::parse_atom() {
    if (at_end()) {
        throw std::runtime_error("UNEXPECTED END OF EXPRESSION");
    }

    if (current_token == "(") {
        advance();
        auto inner = parse_expression();
        expect(")");
        return inner;
    }

    if (in(current_token, special_tokens)) {
        auto function = std::make_unique<Node>(current_token);
        advance();
        // Left stays null; that's how the evaluator spots a function node.
        function->right = parse_atom().release();
        return function;
    }

    if (current_token == variable1 || current_token == variable2 || is_number_token(current_token)) {
        auto leaf = std::make_unique<Node>(current_token);
        advance();
        return leaf;
    }

    throw std::runtime_error("UNEXPECTED TOKEN: '" + current_token + "'");
}
