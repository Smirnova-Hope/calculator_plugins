#pragma once
#include <string>
#include <map>
#include <functional>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include <cmath>

class Calculator {
private:
    std::string input;
    std::size_t pos = 0;

    using MathFunction = std::function<double(double)>;
    std::map<std::string, MathFunction> functions;

    std::string parseIdentifier() {
        skipSpaces();
        std::size_t start = pos;
        while (pos < input.size()) {
            unsigned char ch = static_cast<unsigned char>(input[pos]);
            if (!std::isalpha(ch) && ch != '_') break;
            ++pos;
        }
        if (start == pos) {
            throw std::runtime_error("Expected a number, function, ()");
        }
        return input.substr(start, pos - start);
    }

    void skipSpaces() {
        while (pos < input.size() && std::isspace(static_cast<unsigned char>(input[pos]))) {
            ++pos;
        }
    }

    bool match(char expected) {
        skipSpaces();
        if (pos < input.size() && input[pos] == expected) {
            ++pos;
            return true;
        }
        return false;
    }

    double parseExpression() {
        double result = parseTerm();
        while (true) {
            if (match('+')) result += parseTerm();
            else if (match('-')) result -= parseTerm();
            else break;
        }
        return result;
    }

    double parseTerm() {
        double result = parseUnary();
        while (true) {
            if (match('*')) result *= parseUnary();
            else if (match('/')) {
                double divisor = parseUnary();
                if (divisor == 0.0) throw std::runtime_error("Division by zero");
                result /= divisor;
            }
            else break;
        }
        return result;
    }

    double parseUnary() {
        if (match('+')) {
            return parseUnary();
        }
        if (match('-')) {
            return -parseUnary();
        }
        return parsePower();
    }

    double parsePower() {
        double base = parsePrimary();
        if (match('^')) {
            double exponent = parseUnary(); // Позволяет использовать отрицательные степени (например, 2^-2)
            base = std::pow(base, exponent);
            if (!std::isfinite(base)) {
                throw std::runtime_error("Math error: power overflow or invalid result");
            }
        }
        return base;
    }

    double parsePrimary() {
        if (match('(')) {
            double result = parseExpression();
            if (!match(')')) throw std::runtime_error("Missing closing )");
            return result;
        }
        skipSpaces();
        if (pos < input.size() && (std::isalpha(static_cast<unsigned char>(input[pos])) || input[pos] == '_')) {
            std::string name = parseIdentifier();
            auto it = functions.find(name);
            if (it == functions.end()) throw std::runtime_error("Unknown function: " + name);
            if (!match('(')) throw std::runtime_error("Expected ( after function name");
            double argument = parseExpression();
            if (!match(')')) throw std::runtime_error("Missing ) after function argument");
            return it->second(argument);
        }
        return parseNumber();
    }

    double parseNumber() {
        skipSpaces();
        if (pos >= input.size()) throw std::runtime_error("Expected a number");
        const char* start = input.c_str() + pos;
        char* end = nullptr;
        double value = std::strtod(start, &end);
        if (end == start) throw std::runtime_error("Expected a number");
        pos += static_cast<std::size_t>(end - start);
        if (!std::isfinite(value)) throw std::runtime_error("Number is out of range");
        return value;
    }

public:
    void registerFunction(const std::string& name, MathFunction func) {
        functions[name] = func;
    }

    double calculate(const std::string& expression) {
        input = expression;
        pos = 0;
        double result = parseExpression();
        skipSpaces();
        if (pos != input.size()) {
            throw std::runtime_error("Unexpected character: " + input.substr(pos, 1));
        }
        if (!std::isfinite(result)) {
            throw std::runtime_error("Result is out of range");
        }
        return result;
    }
};