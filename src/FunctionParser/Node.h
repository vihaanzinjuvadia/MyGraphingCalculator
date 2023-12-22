//
// Created by Vihaan Zinjuvadia on 5/14/23.
//

#ifndef FUNCTIONPARSER_NODE_H
#define FUNCTIONPARSER_NODE_H

#include <string>

class Node {
public:
    std::string value;
    double* numeric_value;
    Node* left;
    Node* right;

    Node(const std::string &v);
};


#endif //FUNCTIONPARSER_NODE_H
