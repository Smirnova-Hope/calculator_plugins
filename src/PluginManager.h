#pragma once
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <windows.h>
#include "plugin_api.h"

struct LibraryCloser {
    void operator()(HMODULE hDll) const {
        if (hDll) {
            FreeLibrary(hDll);
        }
    }
};

using SafeLibraryPtr = std::unique_ptr<std::remove_pointer_t<HMODULE>, LibraryCloser>;

class PluginManager {
public:
    static void loadPlugins(const std::string& pluginDir,
        std::function<void(const std::string&, std::function<double(double)>)> registerCallback,
        std::vector<SafeLibraryPtr>& handles)
    {
        namespace fs = std::filesystem;

        if (!fs::exists(pluginDir) || !fs::is_directory(pluginDir)) {
            std::cout << "Directory '" << pluginDir << "' not found. Creating one...\n";
            fs::create_directory(pluginDir);
            return;
        }

        for (const auto& entry : fs::directory_iterator(pluginDir)) {
            if (entry.path().extension() == ".dll") {
                HMODULE rawHandle = LoadLibraryW(entry.path().wstring().c_str());
                if (!rawHandle) {
                    std::cerr << "Failed to load library: " << entry.path().filename() << "\n";
                    continue;
                }
                SafeLibraryPtr hDll(rawHandle);

                auto getPlugin = reinterpret_cast<GetPluginFunc>(GetProcAddress(hDll.get(), "GetPlugin"));
                if (!getPlugin) {
                    std::cerr << "Error: 'GetPlugin' not found in " << entry.path().filename() << "\n";
                    continue; 
                }

                const Plugin* plugin = getPlugin();
                if (!plugin || !plugin->name || !plugin->func || plugin->version != 1) {
                    std::cerr << "Error: Invalid plugin structure in " << entry.path().filename() << "\n";
                    continue;
                }

                std::string funcName(plugin->name);
                MathFunc rawFunc = plugin->func;

                registerCallback(funcName, [rawFunc, funcName](double x) -> double {
                    double result = 0.0;
                    try {
                        int status = rawFunc(x, &result);
                        if (status != 0) {
                            throw std::runtime_error("Plugin error code: " + std::to_string(status));
                        }
                    }
                    catch (...) {
                        throw std::runtime_error("Unhandled exception inside plugin: " + funcName);
                    }
                    return result;
                    });

                std::cout << "[Loaded plugin] Function '" << funcName << "' from " << entry.path().filename().string() << "\n";
                handles.push_back(std::move(hDll));
            }
        }
    }
};