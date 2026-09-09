#pragma once

#include "../../config.hpp"
#include "../method.hpp"
#include "webuiconfig_reqres.hpp"

namespace porla::Methods::WebUI
{
    class WebUIConfig : public Method<WebUIConfigReq, WebUIConfigRes>
    {
    public:
        explicit WebUIConfig(const porla::Config& cfg);

    protected:
        void Invoke(const WebUIConfigReq& req, WriteCb<WebUIConfigRes> cb) override;

    private:
        const porla::Config& m_cfg;
    };
}
