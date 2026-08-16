//
// Created by Vihaan Zinjuvadia on 5/28/23.
//

#ifndef FUNCTIONPARSER_UTILITYFUNCTIONS_H
#define FUNCTIONPARSER_UTILITYFUNCTIONS_H

#include <string>
#include <vector>
#include "../Node.h"

/////////////////////////////
//    EVALUATION OF AST    //
/////////////////////////////
// Undefined results (1/0, sqrt of a negative, ...) come back as NaN instead of throwing,
// so one bad sample point doesn't take down the whole graph.
double operator_evaluate(double v1, double v2, const std::string &operand);
double function_evaluate(double value, const std::string &function);

// Returns the value with replace1/replace2 substituted for the two variables. Doesn't
// modify the tree, so one tree can be evaluated as many times as needed.
double evaluate_tree(const Node *n, double replace1, double replace2);

////////////////////////
//    PRINTING AST    //
////////////////////////
void printTreeInorder(const Node *n);

/////////////////////////////////////////////////////////////
//    CHECKING IF A VALUE EXISTS IN A VECTOR OF STRINGS    //
/////////////////////////////////////////////////////////////
bool in(const std::string &str, const std::vector<std::string> &vec);


#endif //FUNCTIONPARSER_UTILITYFUNCTIONS_H
