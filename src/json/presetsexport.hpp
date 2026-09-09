#pragma once

#include <algorithm>
#include <vector>

#include <libtorrent/storage_defs.hpp>
#include <nlohmann/json.hpp>

#include "../methods/presetsexport_reqres.hpp"

using json = nlohmann::json;

namespace porla::Methods
{
    static void from_json(const json& j, PresetsExportReq& req)
    {
    }

    static void to_json(json& j, const PresetsExportRes& res)
    {
        std::vector<std::string> keys;

        for (auto const& [key, preset] : res.presets)
        {
            if (preset.dollar_hidden.value_or(false))
                continue;

            keys.push_back(key);
        }

        if (res.sort_alphabetically)
        {
            std::sort(keys.begin(), keys.end());
        }
        else
        {
            std::sort(keys.begin(), keys.end(), [&res](auto const& a, auto const& b)
            {
                auto const it_a = std::find(res.order.begin(), res.order.end(), a);
                auto const it_b = std::find(res.order.begin(), res.order.end(), b);
                return std::distance(res.order.begin(), it_a) < std::distance(res.order.begin(), it_b);
            });
        }

        j = json::array();

        for (auto const& key : keys)
        {
            auto const& preset = res.presets.at(key);

            json p = json::object();
            p["key"]             = key;
            p["category"]        = preset.category        ? json(preset.category.value())        : json();
            p["save_path"]       = preset.save_path        ? json(preset.save_path.value())       : json();
            p["session"]         = preset.session          ? json(preset.session.value())         : json();
            p["download_limit"]  = preset.download_limit   ? json(preset.download_limit.value())  : json();
            p["upload_limit"]    = preset.upload_limit     ? json(preset.upload_limit.value())    : json();
            p["max_connections"] = preset.max_connections  ? json(preset.max_connections.value()) : json();
            p["max_uploads"]     = preset.max_uploads      ? json(preset.max_uploads.value())     : json();
            p["tags"]            = json(preset.tags);

            if (preset.storage_mode)
            {
                p["storage_mode"] = preset.storage_mode.value() == libtorrent::storage_mode_allocate
                    ? json("allocate")
                    : json("sparse");
            }
            else
            {
                p["storage_mode"] = json();
            }

            p["$default"] = preset.is_default ? json(preset.is_default.value()) : json();

            j.push_back(p);
        }
    }
}
