#pragma once

#include "channel.h"
#include "math.h"

#define MODULE void
typedef MODULE(*MODULE_POINTER)();
#define DRIVE(channel_name, expression) \
    { \
        channel_name.drive(expression); \
    }
#define BLOCK(...) \
    { \
        __VA_ARGS__ \
    }
#define LET(var_declaration) \
    const auto var_declaration;
#define SLET(var_declaration) \
    static auto var_declaration;
