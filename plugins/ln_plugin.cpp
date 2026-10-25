#pragma once
#include "plugin_api.h"
#include <cmath>
#include <stdexcept>

int lnFunc(double x, double* result) {
    if (result == nullptr) return 1;
    if (x <= 0.0) {
        return -1;
    }
    *result = std::log(x);
    return 0;
}

static Plugin plugin = { 1, "ln", lnFunc };

PLUGIN_EXPORT const Plugin* GetPlugin() {
    return &plugin;
}