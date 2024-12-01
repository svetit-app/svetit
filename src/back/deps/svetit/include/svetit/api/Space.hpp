#pragma once

#include "Space_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Space {
  // CppStringWithFormat

  std::optional<boost::uuids::uuid> id{};
  std::string name{};
  std::string key{};
  bool requestsAllowed{};
  std::optional<std::int64_t> createdAt{};
};

bool operator==(const svetit::api::Space& lhs, const svetit::api::Space& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Space& value);

Space Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Space>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Space& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

