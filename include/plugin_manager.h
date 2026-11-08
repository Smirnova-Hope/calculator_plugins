#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <windows.h>

struct LibraryCloser {
    void operator()(HMODULE hDll) const;
};

using SafeLibraryPtr = std::unique_ptr<std::remove_pointer_t<HMODULE>, LibraryCloser>;

class PluginManager {
public:
    static void loadPlugins(const std::string& pluginDir,
    const std::function<void(const std::string&, const MathFunction&)>& registerCallback,
    std::vector<SafeLibraryPtr>& handles);
};
