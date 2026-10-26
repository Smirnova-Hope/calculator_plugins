#pragma once
#include <string>
#include <map>
#include <functional>

class Calculator {
private:
    std::string input;
    std::size_t pos = 0;

    using MathFunction = std::function<double(double)>;
    std::map<std::string, MathFunction> functions;

    std::string parseIdentifier();
    void skipSpaces();
    bool match(char expected);
    double parseExpression();
    double parseTerm();
    double parseUnary();
    double parsePower();
    double parsePrimary();
    double parseNumber();

public:
    void registerFunction(const std::string& name, MathFunction func);
    double calculate(const std::string& expression);
};