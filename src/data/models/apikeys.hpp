#pragma once

#include <ctime>
#include <optional>
#include <string>
#include <vector>

#include <sqlite3.h>

namespace porla::Data::Models
{
    class ApiKeys
    {
    public:
        struct ApiKey
        {
            int         id;
            std::string name;
            std::time_t created_at;
            std::optional<std::time_t> last_used_at;
        };

        static std::string HashToken(const std::string& token);

        static int Insert(sqlite3* db, const std::string& name, const std::string& token_hash);
        static std::optional<ApiKey> GetByTokenHash(sqlite3* db, const std::string& token_hash);
        static std::vector<ApiKey> List(sqlite3* db);
        static void Delete(sqlite3* db, int id);
        static void TouchLastUsed(sqlite3* db, int id);
    };
}
