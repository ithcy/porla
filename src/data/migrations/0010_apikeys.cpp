#include "0010_apikeys.hpp"

#include <boost/log/trivial.hpp>

using porla::Data::Migrations::ApiKeys;

int ApiKeys::Migrate(sqlite3* db)
{
    BOOST_LOG_TRIVIAL(info) << "Creating 'api_keys' table";

    return sqlite3_exec(
        db,
        "CREATE TABLE api_keys ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "token_hash TEXT NOT NULL UNIQUE,"
        "created_at INTEGER NOT NULL,"
        "last_used_at INTEGER NOT NULL DEFAULT 0"
        ");",
        nullptr,
        nullptr,
        nullptr);
}
