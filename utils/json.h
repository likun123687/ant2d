#pragma once
#include <fmt/core.h>
#define JSON_NOEXCEPTION 1

#define JSON_TRY_USER if(true)
#define JSON_CATCH_USER(exception) if(false)
#define JSON_THROW_USER(exception)                           \
    {fmt::print("Error in {} : {} (function {})-{}",__FILE__, \
            __LINE__, __FUNCTION__, (exception).what());\
    std::abort();}

#include <third_party/nlohmann/json.hpp>
