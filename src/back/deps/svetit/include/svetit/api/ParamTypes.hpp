#pragma once

#include "ParamTypes_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <ParamType.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct ParamTypes {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::ParamType> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::ParamTypes& lhs,
                const svetit::api::ParamTypes& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ParamTypes& value);

ParamTypes Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamTypes>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ParamTypes& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

