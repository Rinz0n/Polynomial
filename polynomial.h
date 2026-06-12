#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <cmath>
#include "poly_node.h"
#include "polynomial_exceptions.h"

class Polynomial {
private:
    PolyNode* head;
    char variable;  

    void addTerm(int coeff, int power);
    void sortByPowerDesc();
    void removeZeroTerms();
    PolyNode* getNodeAt(int index) const;
    int getLength() const;
    char detectVariable(const std::string& str) const;
    bool isVariableChar(char c) const;

public:
    Polynomial();
    Polynomial(const Polynomial& other);
    Polynomial& operator=(const Polynomial& other);
    ~Polynomial();

    void parseFromString(const std::string& str);
    std::string toString() const;
    void combineLikeTerms();
    void clear();
    bool isEmpty() const;
    void show() const;
    char getVariable() const;
    bool hasVariable() const;

    void loadFromFile(const std::string& filename);
    void appendToFile(const std::string& filename);
};

#endif