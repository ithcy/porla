#include "presetsexport.hpp"

using porla::Methods::PresetsExport;
using porla::Methods::PresetsExportReq;
using porla::Methods::PresetsExportRes;

PresetsExport::PresetsExport(const porla::Config& cfg)
    : m_cfg(cfg)
{
}

void PresetsExport::Invoke(const PresetsExportReq& req, WriteCb<PresetsExportRes> cb)
{
    cb.Ok(PresetsExportRes{
        .presets             = m_cfg.presets,
        .order               = m_cfg.preset_order,
        .sort_alphabetically = m_cfg.webui_sort_presets_alphabetically.value_or(true)
    });
}
