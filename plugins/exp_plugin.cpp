#include "plugin_api.h"
#include <cmath>

int expFunc(double x, double* result) {
    if (result == nullptr) return 1;
    *result = std::exp(x);
return 0;
}

static Plugin plugin = { 1, "exp", expFunc };
PLUGIN_EXPORT const Plugin* GetPlugin() {
    return &plugin;
}