#pragma once

#include "Role_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Role {
  std::optional<std::int32_t> id{};
  std::string name{};
};

bool operator==(const svetit::api::Role& lhs, const svetit::api::Role& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Role& value);

Role Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::Role>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Role& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

