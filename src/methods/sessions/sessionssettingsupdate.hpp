#pragma once

#include "../method.hpp"
#include "sessionssettingsupdate_reqres.hpp"

namespace porla
{
    class Sessions;
}

namespace porla::Methods
{
    class SessionsSettingsUpdate : public Method<SessionsSettingsUpdateReq, SessionsSettingsUpdateRes>
    {
    public:
        explicit SessionsSettingsUpdate(Sessions& sessions);

    protected:
        void Invoke(const SessionsSettingsUpdateReq& req, WriteCb<SessionsSettingsUpdateRes> cb) override;

    private:
        Sessions& m_sessions;
    };
}
