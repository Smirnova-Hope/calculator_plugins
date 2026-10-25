#pragma once


#include "plugin_api.h"
#include <cmath>

int sinFunc(double x, double* result)
{
    if (result == nullptr)
        return 1;

    const double pi = std::acos(-1.0);
    *result = std::sin(x * pi / 180.0);

    return 0;
}

Plugin plugin = { 1, "sin", sinFunc };

PLUGIN_EXPORT const Plugin* GetPlugin()
{
    return &plugin;
}
