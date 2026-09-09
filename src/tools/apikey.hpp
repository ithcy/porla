#pragma once

#include <memory>

namespace porla { class Config; }

namespace porla::Tools
{
    int ApiKeyCreate(int argc, char* argv[], std::unique_ptr<porla::Config> cfg);
    int ApiKeyList(int argc, char* argv[], std::unique_ptr<porla::Config> cfg);
    int ApiKeyRevoke(int argc, char* argv[], std::unique_ptr<porla::Config> cfg);
}
