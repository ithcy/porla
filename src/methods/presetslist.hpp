#pragma once

#include "../config.hpp"
#include "method.hpp"
#include "presetslist_reqres.hpp"

namespace porla::Methods
{
    class PresetsList : public Method<PresetsListReq, PresetsListRes>
    {
    public:
        explicit PresetsList(const porla::Config& cfg);

    protected:
        void Invoke(const PresetsListReq& req, WriteCb<PresetsListRes> cb) override;

    private:
        const porla::Config& m_cfg;
    };
}
