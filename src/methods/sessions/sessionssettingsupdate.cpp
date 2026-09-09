#include "sessionssettingsupdate.hpp"

#include <boost/log/trivial.hpp>
#include <libtorrent/settings_pack.hpp>

#include "../../sessions.hpp"

namespace lt = libtorrent;

using porla::Methods::SessionsSettingsUpdate;
using porla::Methods::SessionsSettingsUpdateReq;
using porla::Methods::SessionsSettingsUpdateRes;

SessionsSettingsUpdate::SessionsSettingsUpdate(porla::Sessions& sessions)
    : m_sessions(sessions)
{
}

void SessionsSettingsUpdate::Invoke(const SessionsSettingsUpdateReq& req, WriteCb<SessionsSettingsUpdateRes> cb)
{
    const auto& state = req.name.has_value()
        ? m_sessions.Get(req.name.value())
        : m_sessions.Default();

    if (state == nullptr)
    {
        return cb.Error(-1, "Session not found");
    }

    lt::settings_pack pack;

    for (auto const& [key, value] : req.settings)
    {
        const int type = lt::setting_by_name(key);

        if (type == -1)
        {
            BOOST_LOG_TRIVIAL(warning) << "Unknown session setting '" << key << "', ignoring";
            continue;
        }

        if (porla::Sessions::DisallowedSetting(key))
        {
            BOOST_LOG_TRIVIAL(warning) << "Setting '" << key << "' is not allowed to be changed, ignoring";
            continue;
        }

        switch (type & lt::settings_pack::type_mask)
        {
        case lt::settings_pack::bool_type_base:
            if (!value.is_boolean())
            {
                BOOST_LOG_TRIVIAL(warning) << "Value for setting '" << key << "' is not a boolean, ignoring";
                continue;
            }
            pack.set_bool(type, value.get<bool>());
            break;
        case lt::settings_pack::int_type_base:
            if (!value.is_number_integer())
            {
                BOOST_LOG_TRIVIAL(warning) << "Value for setting '" << key << "' is not an integer, ignoring";
                continue;
            }
            pack.set_int(type, value.get<int>());
            break;
        case lt::settings_pack::string_type_base:
            if (!value.is_string())
            {
                BOOST_LOG_TRIVIAL(warning) << "Value for setting '" << key << "' is not a string, ignoring";
                continue;
            }
            pack.set_str(type, value.get<std::string>());
            break;
        default:
            continue;
        }
    }

    state->session->apply_settings(pack);

    cb.Ok(SessionsSettingsUpdateRes{});
}
