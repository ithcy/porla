#pragma once

#include <functional>
#include <string>

#include <sqlite3.h>

#include "handler.hpp"

namespace porla::Http
{
    class JwtHandler
    {
    public:
        JwtHandler(sqlite3* db, const std::string& secret_key, Handler next);

        void operator()(uWS::HttpResponse<false>* res, uWS::HttpRequest* req);

    private:
        sqlite3*    m_db;
        std::string m_secret_key;
        Handler     m_next;
    };
}
