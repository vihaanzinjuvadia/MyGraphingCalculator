//
// Created by Vihaan Zinjuvadia on 5/28/23.
//

#ifndef FUNCTIONPARSER_UTILITYFUNCTIONS_H
#define FUNCTIONPARSER_UTILITYFUNCTIONS_H

#include "../Node.h"

/////////////////////////////
//    EVALUATION OF AST    //
/////////////////////////////
extern double operator_evaluate(const std::string &v1, const std::string &v2, const std::string &operand);
extern double function_evaluate(const std::string &v2, const std::string &function);
extern void evaluate_tree(Node *n, const double replace1, const double replace2);

////////////////////////
//    PRINTING AST    //
////////////////////////
extern void printTreeInorder(const Node *n);

/////////////////////////////////////////////////////////////
//    CHECKING IF A VALUE EXISTS IN A VECTOR OF STRINGS    //
/////////////////////////////////////////////////////////////
extern bool in(const std::string &str, const std::vector<std::string> &vec);


#endif //FUNCTIONPARSER_UTILITYFUNCTIONS_H
