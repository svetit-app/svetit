#pragma once

#include "NodeGroups_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <NodeGroup.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct NodeGroups {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::NodeGroup> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::NodeGroups& lhs,
                const svetit::api::NodeGroups& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::NodeGroups& value);

NodeGroups Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeGroups>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::NodeGroups& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

