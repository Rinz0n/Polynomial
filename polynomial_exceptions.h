#ifndef POLYNOMIAL_EXCEPTIONS_H
#define POLYNOMIAL_EXCEPTIONS_H

#include <exception>
#include <string>

class PolynomialException : public std::exception {
private:
    std::string message;
public:
    PolynomialException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class FileOpenException : public PolynomialException {
public:
    FileOpenException(const std::string& filename)
        : PolynomialException("Не удалось открыть файл: " + filename) {}
};

class ParseErrorException : public PolynomialException {
public:
    ParseErrorException(const std::string& details)
        : PolynomialException("Ошибка парсинга многочлена: " + details) {}
};

class EmptyPolynomialException : public PolynomialException {
public:
    EmptyPolynomialException()
        : PolynomialException("Многочлен пуст, операция невозможна") {}
};

class MultipleVariableException : public PolynomialException {
public:
    MultipleVariableException(char var1, char var2)
        : PolynomialException("Ошибка: в многочлене используются разные переменные ('" +
                              std::string(1, var1) + "' и '" + std::string(1, var2) +
                              "'). Допустима только одна переменная.") {}
};

class NoVariableException : public PolynomialException {
public:
    NoVariableException()
        : PolynomialException("Ошибка: многочлен не содержит переменной. Пример: 5y^2 + 3y") {}
};

#endif