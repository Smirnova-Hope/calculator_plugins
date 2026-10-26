#include <iostream>
#include <vector>
#include <string>
#include "expression_parser.h"
#include "plugin_manager.h"

int main() {
    Calculator calculator;
    std::vector<SafeLibraryPtr> loadedHandles;

    PluginManager::loadPlugins("plugins", [&calculator](const std::string& name, auto func) {
        calculator.registerFunction(name, func);
        }, loadedHandles);

    std::string expression;
    std::cout << "\n Enter exit to quit.\n\n";

    while (true) {
        std::cout << "Enter the expression: ";
        if (!std::getline(std::cin, expression)) break;
        if (expression == "exit") break;
        if (expression.find_first_not_of(" \t\r\n") == std::string::npos) continue;

        try {
            double result = calculator.calculate(expression);
            std::cout << "= " << result << '\n';
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    std::cout << "Calculator closed cleanly.\n";
    return 0;
}