#pragma once

#include "Link_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Link {
  // CppStringWithFormat

  // CppStringWithFormat

  std::optional<boost::uuids::uuid> id{};
  boost::uuids::uuid spaceId{};
  std::optional<std::string> creatorId{};
  std::string name{};
  std::optional<std::int64_t> createdAt{};
  std::int64_t expiredAt{};
};

bool operator==(const svetit::api::Link& lhs, const svetit::api::Link& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Link& value);

Link Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::Link>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Link& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

