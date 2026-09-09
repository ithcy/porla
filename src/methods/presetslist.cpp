#include "presetslist.hpp"

using porla::Methods::PresetsList;
using porla::Methods::PresetsListReq;
using porla::Methods::PresetsListRes;

PresetsList::PresetsList(const porla::Config& cfg)
    : m_cfg(cfg)
{
}

void PresetsList::Invoke(const PresetsListReq& req, WriteCb<PresetsListRes> cb)
{
    cb.Ok(PresetsListRes{
        .presets             = m_cfg.presets,
        .order               = m_cfg.preset_order,
        .sort_alphabetically = m_cfg.webui_sort_presets_alphabetically.value_or(true)
    });
}
