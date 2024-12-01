#pragma once

#include "NodeProject_fwd.hpp"

#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct NodeProject {
  // CppStringWithFormat

  // CppStringWithFormat

  boost::uuids::uuid nodeId{};
  boost::uuids::uuid projectId{};
};

bool operator==(const svetit::api::NodeProject& lhs,
                const svetit::api::NodeProject& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::NodeProject& value);

NodeProject Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeProject>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::NodeProject& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

