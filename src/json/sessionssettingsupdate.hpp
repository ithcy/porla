#pragma once

#include <nlohmann/json.hpp>

#include "../methods/sessions/sessionssettingsupdate_reqres.hpp"
#include "utils.hpp"

namespace porla::Methods
{
    NLOHMANN_JSONIFY_ALL_THINGS(
        SessionsSettingsUpdateReq,
        name,
        settings)

    static void to_json(json& j, const SessionsSettingsUpdateRes& res)
    {
        j = json::object();
    }
}
