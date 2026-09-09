#pragma once

#include "../config.hpp"
#include "method.hpp"
#include "presetsexport_reqres.hpp"

namespace porla::Methods
{
    class PresetsExport : public Method<PresetsExportReq, PresetsExportRes>
    {
    public:
        explicit PresetsExport(const porla::Config& cfg);

    protected:
        void Invoke(const PresetsExportReq& req, WriteCb<PresetsExportRes> cb) override;

    private:
        const porla::Config& m_cfg;
    };
}
