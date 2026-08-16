//
// Created by Vihaan Zinjuvadia on 5/14/23.
//

#ifndef FUNCTIONPARSER_NODE_H
#define FUNCTIONPARSER_NODE_H

#include <string>

class Node {
public:
    std::string value;
    Node* left;
    Node* right;

    explicit Node(const std::string &v);
    ~Node();

    // A node owns its children, so copying one would double free the subtree.
    Node(const Node &) = delete;
    Node& operator=(const Node &) = delete;
};


#endif //FUNCTIONPARSER_NODE_H
