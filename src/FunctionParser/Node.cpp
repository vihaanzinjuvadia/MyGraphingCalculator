//
// Created by Vihaan Zinjuvadia on 5/14/23.
//

#include "Node.h"

Node::Node(const std::string &v)
    : value {v}, left {nullptr}, right {nullptr} {
}

Node::~Node() {
    delete left;
    delete right;
}
