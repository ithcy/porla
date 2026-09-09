#include "apikey.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <ctime>
#include <string>

#include <sodium.h>

#include "../config.hpp"
#include "../data/models/apikeys.hpp"

using porla::Data::Models::ApiKeys;

int porla::Tools::ApiKeyCreate(int argc, char** argv, std::unique_ptr<porla::Config> cfg)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s auth:apikey:create <name>\n", argv[0]);
        return -1;
    }

    const std::string name = argv[2];

    if (name.empty() || name.size() > 256)
    {
        fprintf(stderr, "Name must be between 1 and 256 characters\n");
        return -1;
    }

    const bool valid_chars = std::all_of(name.begin(), name.end(), [](unsigned char c)
    {
        return std::isalnum(c) || c == '-' || c == '_';
    });

    if (!valid_chars)
    {
        fprintf(stderr, "Name may only contain ASCII letters, digits, '-' and '_'\n");
        return -1;
    }

    unsigned char raw[32];
    randombytes_buf(raw, sizeof(raw));

    char encoded[sodium_base64_ENCODED_LEN(sizeof(raw), sodium_base64_VARIANT_URLSAFE_NO_PADDING)];
    sodium_bin2base64(encoded, sizeof(encoded), raw, sizeof(raw), sodium_base64_VARIANT_URLSAFE_NO_PADDING);

    const std::string token = "porla_" + std::string(encoded);

    ApiKeys::Insert(cfg->db, name, ApiKeys::HashToken(token));

    printf("%s\n", token.c_str());
    fprintf(stderr, "Store this token now - it will not be shown again.\n");

    return 0;
}

int porla::Tools::ApiKeyList(int argc, char** argv, std::unique_ptr<porla::Config> cfg)
{
    const auto keys = ApiKeys::List(cfg->db);

    printf("%-6s %-30s %-25s %-25s\n", "id", "name", "created_at", "last_used_at");

    for (const auto& key : keys)
    {
        char created_buf[32] = "-";
        std::strftime(created_buf, sizeof(created_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&key.created_at));

        char last_used_buf[32] = "never";

        if (key.last_used_at.has_value())
        {
            std::time_t t = key.last_used_at.value();
            std::strftime(last_used_buf, sizeof(last_used_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        }

        printf("%-6d %-30s %-25s %-25s\n", key.id, key.name.c_str(), created_buf, last_used_buf);
    }

    return 0;
}

int porla::Tools::ApiKeyRevoke(int argc, char** argv, std::unique_ptr<porla::Config> cfg)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s auth:apikey:revoke <id>\n", argv[0]);
        return -1;
    }

    int id;

    try
    {
        size_t pos;
        id = std::stoi(argv[2], &pos);

        if (pos != std::string(argv[2]).size())
        {
            throw std::invalid_argument("trailing characters");
        }
    }
    catch (const std::exception&)
    {
        fprintf(stderr, "Invalid id: %s\n", argv[2]);
        return -1;
    }

    ApiKeys::Delete(cfg->db, id);

    printf("Revoked api key %d\n", id);

    return 0;
}
