//
// Created by Vihaan Zinjuvadia on 5/29/23.
//
#include "UtilityVariables.h"


std::string variable1 {"x"};
std::string variable2 {"y"};
const std::vector<std::string> special_tokens = {
        "sin", "cos", "tan", "csc", "sec", "cot", "arcsin", "arccos", "arctan",
        "arccsc", "arcsec", "arccot", "sinh", "cosh", "tanh", "csch", "sech",
        "coth", "arcsinh", "arccosh", "arctanh", "arccsch", "arcsech", "arccoth",
        "sqrt","cbrt", "log", "ln", "abs"
};
const std::vector<std::string> operators = {
        "+", "-", "*", "/", "^", "(", ")"
};
const std::vector<std::string> constants = {
        "pi", "e"
};
