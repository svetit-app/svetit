#pragma once

#include "Node_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Node {
  // CppStringWithFormat

  // CppStringWithFormat

  std::optional<boost::uuids::uuid> id{};
  std::optional<boost::uuids::uuid> spaceId{};
  std::string name{};
  std::string description{};
  double latitude{};
  double longitude{};
  std::optional<std::int64_t> createdAt{};
};

bool operator==(const svetit::api::Node& lhs, const svetit::api::Node& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Node& value);

Node Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::Node>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Node& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

