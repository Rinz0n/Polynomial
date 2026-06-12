#include "poly_node.h"

PolyNode::PolyNode() : coeff(0), power(0), next(nullptr) {}

PolyNode::PolyNode(int c, int p) : coeff(c), power(p), next(nullptr) {}

PolyNode::PolyNode(const PolyNode& other)
    : coeff(other.coeff), power(other.power), next(nullptr) {}

PolyNode::~PolyNode() {}

void PolyNode::setCoeff(int c) {
    coeff = c;
}

int PolyNode::getCoeff() const {
    return coeff;
}

void PolyNode::setPower(int p) {
    power = p;
}

int PolyNode::getPower() const {
    return power;
}

PolyNode* PolyNode::getNext() const {
    return next;
}

void PolyNode::setNext(PolyNode* n) {
    next = n;
}