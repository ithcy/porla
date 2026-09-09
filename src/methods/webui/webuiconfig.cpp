#include "webuiconfig.hpp"

using porla::Methods::WebUI::WebUIConfig;
using porla::Methods::WebUI::WebUIConfigReq;
using porla::Methods::WebUI::WebUIConfigRes;

WebUIConfig::WebUIConfig(const porla::Config& cfg)
    : m_cfg(cfg)
{
}

void WebUIConfig::Invoke(const WebUIConfigReq& req, WriteCb<WebUIConfigRes> cb)
{
    // Any unset or unrecognized value falls back to "torrent-file" - the
    // historical default before this setting existed.
    const std::string default_add_torrent_tab =
        m_cfg.webui_default_add_torrent_tab == "magnet-link"
            ? "magnet-link"
            : "torrent-file";

    cb.Ok(WebUIConfigRes{
        .default_add_torrent_tab = default_add_torrent_tab
    });
}
