#pragma once

#include "NodeProjects_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <NodeProject.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct NodeProjects {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::NodeProject> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::NodeProjects& lhs,
                const svetit::api::NodeProjects& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::NodeProjects& value);

NodeProjects Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeProjects>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::NodeProjects& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

