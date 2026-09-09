#include "apikeys.hpp"

#include <sodium.h>

#include "../statement.hpp"

using porla::Data::Models::ApiKeys;

std::string ApiKeys::HashToken(const std::string& token)
{
    unsigned char hash[crypto_hash_sha256_BYTES];
    crypto_hash_sha256(hash, reinterpret_cast<const unsigned char*>(token.data()), token.size());

    char hex[crypto_hash_sha256_BYTES * 2 + 1];
    sodium_bin2hex(hex, sizeof(hex), hash, sizeof(hash));

    return {hex};
}

int ApiKeys::Insert(sqlite3* db, const std::string& name, const std::string& token_hash)
{
    const auto created_at = std::time(nullptr);

    Statement::Prepare(db, "INSERT INTO api_keys (name, token_hash, created_at) VALUES ($1, $2, $3);")
        .Bind(1, std::string_view(name))
        .Bind(2, std::string_view(token_hash))
        .Bind(3, static_cast<int>(created_at))
        .Execute();

    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

std::optional<ApiKeys::ApiKey> ApiKeys::GetByTokenHash(sqlite3* db, const std::string& token_hash)
{
    std::optional<ApiKey> key;

    Statement::Prepare(db, "SELECT id,name,created_at,last_used_at FROM api_keys WHERE token_hash = $1")
        .Bind(1, std::string_view(token_hash))
        .Step(
            [&key](auto const& row)
            {
                const auto last_used_at = row.GetInt32(3);

                key = ApiKey{
                    .id           = row.GetInt32(0),
                    .name         = row.GetStdString(1),
                    .created_at   = static_cast<std::time_t>(row.GetInt32(2)),
                    .last_used_at = last_used_at > 0
                        ? std::optional<std::time_t>(static_cast<std::time_t>(last_used_at))
                        : std::nullopt
                };

                return SQLITE_OK;
            });

    return key;
}

std::vector<ApiKeys::ApiKey> ApiKeys::List(sqlite3* db)
{
    std::vector<ApiKey> keys;

    Statement::Prepare(db, "SELECT id,name,created_at,last_used_at FROM api_keys ORDER BY id")
        .Step(
            [&keys](auto const& row)
            {
                const auto last_used_at = row.GetInt32(3);

                keys.emplace_back(ApiKey{
                    .id           = row.GetInt32(0),
                    .name         = row.GetStdString(1),
                    .created_at   = static_cast<std::time_t>(row.GetInt32(2)),
                    .last_used_at = last_used_at > 0
                        ? std::optional<std::time_t>(static_cast<std::time_t>(last_used_at))
                        : std::nullopt
                });

                return SQLITE_OK;
            });

    return keys;
}

void ApiKeys::Delete(sqlite3* db, int id)
{
    Statement::Prepare(db, "DELETE FROM api_keys WHERE id = $1")
        .Bind(1, id)
        .Execute();
}

void ApiKeys::TouchLastUsed(sqlite3* db, int id)
{
    Statement::Prepare(db, "UPDATE api_keys SET last_used_at = $1 WHERE id = $2")
        .Bind(1, static_cast<int>(std::time(nullptr)))
        .Bind(2, id)
        .Execute();
}
