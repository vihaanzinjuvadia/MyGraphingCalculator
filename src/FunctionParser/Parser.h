//
// Created by Vihaan Zinjuvadia on 5/17/23.
//

#ifndef FUNCTIONPARSER_PARSER_H
#define FUNCTIONPARSER_PARSER_H


#include <vector>
#include <string>
#include <memory>
#include "Node.h"
#include "Utility/UtilityVariables.h"
#include "Utility/UtilityFunctions.h"

/*
expression : term ((PLUS | MINUS) term)*
term       : unary ((MUL | DIV) unary)*
unary      : (PLUS | MINUS) unary | power
power      : atom (EXPO unary)?                 // right associative, so 2^3^2 is 2^(3^2)
atom       : NUMBER | VARIABLE | FUNCTION atom | LPAREN expression RPAREN
*/
class Parser {
private:
    // Held by value so a Parser built straight from Lexer::tokenize() isn't left
    // referring to a temporary.
    std::vector<std::string> tokens;
    std::string current_token;
    std::size_t current_index;

    bool at_end() const;
    void advance();
    void expect(const std::string &token);

    std::unique_ptr<Node> parse_expression();
    std::unique_ptr<Node> parse_term();
    std::unique_ptr<Node> parse_unary();
    std::unique_ptr<Node> parse_power();
    std::unique_ptr<Node> parse_atom();

public:
    explicit Parser(const std::vector<std::string> &tokens);

    // Caller owns the returned tree; deleting the root deletes all of it.
    Node* parse();
};


#endif //FUNCTIONPARSER_PARSER_H
