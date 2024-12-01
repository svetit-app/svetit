#pragma once

#include "ProjectParams_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <ProjectParam.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct ProjectParams {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::ProjectParam> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::ProjectParams& lhs,
                const svetit::api::ProjectParams& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ProjectParams& value);

ProjectParams Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ProjectParams>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ProjectParams& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

