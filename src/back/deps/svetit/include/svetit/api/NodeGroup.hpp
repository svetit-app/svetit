#pragma once

#include "NodeGroup_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct NodeGroup {
  // CppStringWithFormat

  static constexpr auto kGroupidMinimum = 0;

  boost::uuids::uuid nodeId{};
  std::int32_t groupId{};
};

bool operator==(const svetit::api::NodeGroup& lhs,
                const svetit::api::NodeGroup& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::NodeGroup& value);

NodeGroup Parse(USERVER_NAMESPACE::formats::json::Value json,
                USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeGroup>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::NodeGroup& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

