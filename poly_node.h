#ifndef POLY_NODE_H
#define POLY_NODE_H

class PolyNode {
public:
    int coeff;        // коэффициент
    int power;        // степень
    PolyNode* next;

    PolyNode();
    PolyNode(int c, int p);
    PolyNode(const PolyNode& other);
    ~PolyNode();

    void setCoeff(int c);
    int getCoeff() const;
    void setPower(int p);
    int getPower() const;
    PolyNode* getNext() const;
    void setNext(PolyNode* n);
};

#endif