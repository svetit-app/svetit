#pragma once

#include "Group_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Group {
  // CppStringWithFormat

  std::optional<std::int32_t> id{};
  std::string name{};
  std::string description{};
  std::optional<boost::uuids::uuid> spaceId{};
};

bool operator==(const svetit::api::Group& lhs, const svetit::api::Group& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Group& value);

Group Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Group>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Group& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

