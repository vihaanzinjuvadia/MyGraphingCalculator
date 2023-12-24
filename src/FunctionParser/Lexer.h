//
// Created by Vihaan Zinjuvadia on 5/13/23.
//

#ifndef FUNCTIONPARSER_LEXER_H
#define FUNCTIONPARSER_LEXER_H

#include <string>
#include <vector>
#include "Utility/UtilityVariables.h"
#include "Utility/UtilityFunctions.h"


class Lexer {
private:
    // Later have 2 variables for graphing advanced functions
    std::string function;
public:
    Lexer(const std::string &function);
    std::vector<std::string> tokenize();
};


#endif //FUNCTIONPARSER_LEXER_H
