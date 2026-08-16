//
// Created by Vihaan Zinjuvadia on 5/29/23.
//
#include "UtilityFunctions.h"
#include "UtilityVariables.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <algorithm>

namespace {
    constexpr double NOT_A_NUMBER = std::numeric_limits<double>::quiet_NaN();
}

/////////////////////////////
//    EVALUATION OF AST    //
/////////////////////////////
double operator_evaluate(const double v1, const double v2, const std::string &operand) {
    if (operand == "+") {
        return v1 + v2;
    } else if (operand == "-") {
        return v1 - v2;
    } else if (operand == "*") {
        return v1 * v2;
    } else if (operand == "/") {
        return v2 == 0.0 ? NOT_A_NUMBER : v1 / v2;
    } else if (operand == "^") {
        return std::pow(v1, v2);
    }
    return NOT_A_NUMBER;
}

double function_evaluate(const double value, const std::string &function) {
    if (function == "sin") {
        return std::sin(value);
    } else if (function == "cos") {
        return std::cos(value);
    } else if (function == "tan") {
        return std::tan(value);
    } else if (function == "csc") {
        return 1 / std::sin(value);
    } else if (function == "sec") {
        return 1 / std::cos(value);
    } else if (function == "cot") {
        return 1 / std::tan(value);
    } else if (function == "arcsin") {
        return std::asin(value);
    } else if (function == "arccos") {
        return std::acos(value);
    } else if (function == "arctan") {
        return std::atan(value);
    } else if (function == "arccsc") {
        return std::asin(1 / value);
    } else if (function == "arcsec") {
        return std::acos(1 / value);
    } else if (function == "arccot") {
        return M_PI_2 - std::atan(value);
    } else if (function == "sinh") {
        return std::sinh(value);
    } else if (function == "cosh") {
        return std::cosh(value);
    } else if (function == "tanh") {
        return std::tanh(value);
    } else if (function == "csch") {
        return 1 / std::sinh(value);
    } else if (function == "sech") {
        return 1 / std::cosh(value);
    } else if (function == "coth") {
        return 1 / std::tanh(value);
    } else if (function == "arcsinh") {
        return std::asinh(value);
    } else if (function == "arccosh") {
        return std::acosh(value);
    } else if (function == "arctanh") {
        return std::atanh(value);
    } else if (function == "arccsch") {
        return std::log(1 / value + 1 / std::sqrt(1 / (value * value) + 1));
    } else if (function == "arcsech") {
        return std::log(1 / value + 1 / std::sqrt(1 / (value * value) - 1));
    } else if (function == "arccoth") {
        return 0.5 * std::log((value + 1) / (value - 1));
    } else if (function == "sqrt") {
        return value < 0 ? NOT_A_NUMBER : std::sqrt(value);
    } else if (function == "cbrt") {
        return std::cbrt(value);
    } else if (function == "log") {
        return value <= 0 ? NOT_A_NUMBER : std::log10(value);
    } else if (function == "ln") {
        return value <= 0 ? NOT_A_NUMBER : std::log(value);
    } else if (function == "abs") {
        return std::fabs(value);
    }
    return NOT_A_NUMBER;
}

double evaluate_tree(const Node *n, const double replace1, const double replace2) {
    if (n == nullptr) {
        throw std::runtime_error("INCOMPLETE EXPRESSION");
    }

    // Leaf: one of the two variables, or a literal number.
    if (n->left == nullptr && n->right == nullptr) {
        if (n->value == variable1) {
            return replace1;
        }
        if (n->value == variable2) {
            return replace2;
        }
        try {
            return std::stod(n->value);
        } catch (const std::exception &) {
            throw std::runtime_error("EXPECTED A NUMBER BUT FOUND: " + n->value);
        }
    }

    // Function node: left child is null and the argument is on the right.
    if (n->left == nullptr) {
        return function_evaluate(evaluate_tree(n->right, replace1, replace2), n->value);
    }

    return operator_evaluate(evaluate_tree(n->left, replace1, replace2),
                             evaluate_tree(n->right, replace1, replace2),
                             n->value);
}

////////////////////////
//    PRINTING AST    //
////////////////////////
void printTreeInorder(const Node *n) {
    if (n == nullptr) {
        return;
    }
    printTreeInorder(n->left);
    std::cout << n->value << "\n";
    printTreeInorder(n->right);
}

/////////////////////////////////////////////////////////////
//    CHECKING IF A VALUE EXISTS IN A VECTOR OF STRINGS    //
/////////////////////////////////////////////////////////////
bool in(const std::string &str, const std::vector<std::string> &vec) {
    return std::any_of(vec.begin(), vec.end(),
                       [&str] (const std::string& s) {return s==str;});
}
