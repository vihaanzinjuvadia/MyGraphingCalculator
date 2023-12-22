//
// Created by Vihaan Zinjuvadia on 5/29/23.
//
#include "UtilityFunctions.h"
#include "UtilityVariables.h"
#include <iostream>

/////////////////////////////
//    EVALUATION OF AST    //
/////////////////////////////
double operator_evaluate(const double v1, const double v2, const std::string &operand) {
    double sum = 0;

    if (operand == "+") {
        sum += (v1 + v2);
    } else if (operand == "-") {
        sum += (v1 - v2);
    } else if (operand == "*") {
        sum += (v1 * v2);
    } else if (operand == "/") {
        sum += (v1/v2);
    } else if (operand == "^") {
        sum += pow(v1, v2);
    }

    return sum;
}
double function_evaluate(const double value, const std::string &function) {
    double sum = 0;

    if (function == "sin") {
        sum += sin(value);
    } else if (function == "cos") {
        sum += cos(value);
    } else  if (function == "tan") {
        sum += tan(value);
    } else  if (function == "csc") {
        sum += 1/sin(value);
    } else if (function == "sec") {
        sum += 1/cos(value);
    } else  if (function == "cot") {
        sum += 1/tan(value);
    } else  if (function == "arcsin") {
        sum += asin(value);
    } else if (function == "arccos") {
        sum += acos(value);
    } else  if (function == "arctan") {
        sum += atan(value);
    } else  if (function == "arccsc") {
        sum += asin(1/value);
    } else if (function == "arcsec") {
        sum += acos(1/value);
    } else  if (function == "arccot") {
        sum += M_PI_2 - atan(value);
    } else  if (function == "sinh") {
        sum += sinh(value);
    } else if (function == "cosh") {
        sum += cosh(value);
    } else  if (function == "tanh") {
        sum += tanh(value);
    } else  if (function == "csch") {
        sum += 1/sinh(value);
    } else if (function == "sech") {
        sum += 1/cosh(value);
    } else  if (function == "coth") {
        sum += 1/tanh(value);
    } else  if (function == "arcsinh") {
        sum += asinh(value);
    } else  if (function == "arccosh") {
        sum += acosh(value);
    } else if (function == "arctanh") {
        sum += atanh(value);
    } else  if (function == "arccsch") {
        sum += log(1/value + 1/sqrt(1/(value*value) + 1));
    } else  if (function == "arcsech") {
        sum += log(1/value + 1/sqrt(1/(value*value) - 1));
    } else  if (function == "arccoth") {
        sum += 0.5 * log((value+1)/(value-1));
    } else if (function == "sqrt") {
        if (value < 0) {
            throw std::logic_error("SQRT CANNOT TAKE IN A VALUE LESS THAN 0");
        }
        sum += sqrt(value);
    } else  if (function == "cbrt") {
        sum += pow(value, 1.0/3);
    } else  if (function == "log") {
        if (value <= 0) {
            throw std::logic_error("LOG CANNOT TAKE IN A VALUE LESS THAN OR EQUAL TO 0");
        }
        sum += log10(value);
    } else  if (function == "ln") {
        if (value <= 0) {
            throw std::logic_error("LN CANNOT TAKE IN A VALUE LESS THAN OR EQUAL TO 0");
        }
        sum += log(value);
    } else if (function == "abs") {
        return abs(value);
    }

    return sum;
}
void evaluate_tree(Node *n, const double replace1, const double replace2) {
    // replacing variable here! we dont like this
//    if (n->value == std::string(1, variable1)) {
//        n->value = std::to_string(replace1);
//    } else if (n->value == std::string(1, variable2)) {
//        n->value = std::to_string(replace2);
//    }

    // recursively traveling down the AST
    if (n->left) {
        evaluate_tree(n->left, replace1, replace2);
    }
    if (n->right) {
        evaluate_tree(n->right, replace1, replace2);
    }

    // if a node is a function node, its left child will be null and the right child will be the input to the function.
    if (n->left == nullptr && n->right) {
        auto* value = new double;
        // if n->right>num_val is not null, use num_val, else use n->right->value
        if (n->right->numeric_value != nullptr) {
            *value = function_evaluate(*n->right->numeric_value, n->value);
            n->right->numeric_value = nullptr;
        }
        else if (n->right->value == variable1) {
            *value = function_evaluate(replace1, n->value);
        }
        else if (n->right->value == variable2) {
            *value = function_evaluate(replace2, n->value);
        }
        else {
            *value = function_evaluate(std::stod(n->right->value), n->value);
        }
        // shrinking tree here, fix it
        n->numeric_value = value;
//        n->right = nullptr;
    }

    // operator node evaluation
    if (n->left && n->right && (std::isdigit(n->left->value.at(0)) ||
                                n->left->value == variable1 ||
                                n->left->value == variable2 ||
                                n->left->value.at(0) == '-' ||
                                n->left->numeric_value != nullptr)

                            && (std::isdigit(n->right->value.at(0)) ||
                                n->right->value == variable1 ||
                                n->right->value == variable2 ||
                                n->right->value.at(0) == '-' ||
                                n->right->numeric_value != nullptr)) {
        auto* value = new double;
        // left = nv, right = var1 || var2 || n || nv
        // right = nv, left = var1 || var2 || n
        // COMMENT THIS GIANT MESS!
        if (n->left->numeric_value != nullptr && n->right->value == variable1) {
            *value = operator_evaluate(*n->left->numeric_value, replace1, n->value);
            n->left->numeric_value = nullptr;
        }
        else if (n->left->numeric_value != nullptr && n->right->value == variable2) {
            *value = operator_evaluate(*n->left->numeric_value, replace2, n->value);
            n->left->numeric_value = nullptr;
        }
        else if (n->left->numeric_value != nullptr && std::isdigit(n->right->value[0])) {
            *value = operator_evaluate(*n->left->numeric_value, std::stod(n->right->value), n->value);
            n->left->numeric_value = nullptr;
        }
        else if (n->left->numeric_value != nullptr && n->right->numeric_value != nullptr) {
            *value = operator_evaluate(*n->left->numeric_value, *n->right->numeric_value, n->value);
            n->left->numeric_value = nullptr;
            n->right->numeric_value = nullptr;
        }
        else if (n->right->numeric_value != nullptr && n->left->value == variable1) {
            *value = operator_evaluate(replace1, *n->right->numeric_value, n->value);
            n->right->numeric_value = nullptr;
        }
        else if (n->right->numeric_value != nullptr && n->left->value == variable2) {
            *value = operator_evaluate(replace2, *n->right->numeric_value, n->value);
            n->right->numeric_value = nullptr;
        }
        else if (n->right->numeric_value != nullptr && std::isdigit(n->left->value[0])) {
            *value = operator_evaluate(std::stod(n->left->value), *n->right->numeric_value, n->value);
            n->right->numeric_value = nullptr;
        }
        else if (n->left->value == variable1 && n->right->value == variable2) {
            *value = operator_evaluate(replace1, replace2, n->value);
        }
        else if (n->left->value == variable2 && n->right->value == variable1) {
            *value = operator_evaluate(replace2, replace1, n->value);
        }
        else if (n->left->value == variable1 && std::isdigit(n->right->value.at(0))) {
            *value = operator_evaluate(replace1, std::stod(n->right->value), n->value);
        }
        else if (n->left->value == variable2 && std::isdigit(n->right->value.at(0))) {
            *value = operator_evaluate(replace2, std::stod(n->right->value), n->value);
        }
        else if (n->right->value == variable1 && std::isdigit(n->left->value.at(0))) {
            *value = operator_evaluate(std::stod(n->left->value), replace1, n->value);
        }
        else if (n->right->value == variable2 && std::isdigit(n->left->value.at(0))) {
            *value = operator_evaluate(std::stod(n->left->value), replace2, n->value);
        }
        else if (std::isdigit(n->right->value.at(0)) && std::isdigit(n->left->value.at(0))) {
            *value = operator_evaluate(std::stod(n->left->value), std::stod(n->right->value), n->value);
        }
//        std::cout << *value << std::endl;
        // shrinking tree here, fix it
        n->numeric_value = value;
//        n->left = nullptr;
//        n->right = nullptr;
    }

    if (std::isdigit(n->value[0])) {
        auto* value = new double;
        *value = std::stod(n->value);
        n->numeric_value = value;
    }

    return;
}

////////////////////////
//    PRINTING AST    //
////////////////////////
void printTreeInorder(const Node *n) {
    if (n->left != nullptr) {
        printTreeInorder(n->left);
    }

    std::cout << n->value << "\n";

    if (n->right != nullptr) {
        printTreeInorder(n->right);
    }
}

/////////////////////////////////////////////////////////////
//    CHECKING IF A VALUE EXISTS IN A VECTOR OF STRINGS    //
/////////////////////////////////////////////////////////////
bool in(const std::string &str, const std::vector<std::string> &vec) {
    return std::any_of(vec.begin(), vec.end(),
                       [&str] (const std::string& s) {return s==str;});
}