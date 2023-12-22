//
// Created by Vihaan Zinjuvadia on 5/17/23.
//

#ifndef FUNCTIONPARSER_PARSER_H
#define FUNCTIONPARSER_PARSER_H


#include <vector>
#include <string>
#include "Node.h"
#include "Utility/UtilityVariables.h"
#include "Utility/UtilityFunctions.h"

class Parser {
private:
    const std::vector<std::string> &tokens;
    std::string current_token;
    int current_index;
    std::string previous_operand;
    Node *previous_node;

    bool is_same_precedence();

    void advance();

    Node* parse_expression(); // Lowest Precedence, first called -->  expr: term ((PLUS | MINUS) term)*
    Node* parse_term();       // Expressions could have terms which are multiplied or divided -->  term: expo ((MUL | DIV) expo)*
    Node* parse_exponents();  // Terms could have exponents
    Node* parse_functions();
    Node* parse_factor();     // Exponents have integers or parenthesis

    Node* handle_same_precedence(Node* node, Node* (Parser::*function)());
    Node* handle_regular(Node* node, Node* (Parser::*function)());

public:
    Parser(const std::vector<std::string> &tokens);
    Node* parse(); /// THIS WHOLE THING RETURNS THE WHOLE AST OF THE EQUATION

};


#endif //FUNCTIONPARSER_PARSER_H
