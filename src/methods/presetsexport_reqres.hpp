#pragma once

#include <map>
#include <string>
#include <vector>

#include "../config.hpp"

namespace porla::Methods
{
    struct PresetsExportReq {};

    struct PresetsExportRes
    {
        std::map<std::string, Config::Preset> presets;
        std::vector<std::string>              order;
        bool                                   sort_alphabetically;
    };
}
