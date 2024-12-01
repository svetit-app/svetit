#pragma once

#include "Error_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

// Common Error
struct Error {
  std::int32_t code{};
  std::string message{};
};

bool operator==(const svetit::api::Error& lhs, const svetit::api::Error& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Error& value);

Error Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Error>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Error& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

