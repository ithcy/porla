#pragma once

#include <string>

namespace porla::Methods::WebUI
{
    struct WebUIConfigReq {};

    struct WebUIConfigRes
    {
        std::string default_add_torrent_tab;
    };
}
