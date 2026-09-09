#pragma once

#include <nlohmann/json.hpp>

#include "../methods/webui/webuiconfig_reqres.hpp"

using json = nlohmann::json;

namespace porla::Methods::WebUI
{
    static void from_json(const json& j, WebUIConfigReq& req)
    {
    }

    static void to_json(json& j, const WebUIConfigRes& res)
    {
        j = json::object();
        j["default_add_torrent_tab"] = res.default_add_torrent_tab;
    }
}
