#pragma once

#include "Nodes_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Node.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Nodes {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Node> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Nodes& lhs, const svetit::api::Nodes& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Nodes& value);

Nodes Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Nodes>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Nodes& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

