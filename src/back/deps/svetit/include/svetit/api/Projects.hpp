#pragma once

#include "Projects_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Project.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Projects {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Project> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Projects& lhs,
                const svetit::api::Projects& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Projects& value);

Projects Parse(USERVER_NAMESPACE::formats::json::Value json,
               USERVER_NAMESPACE::formats::parse::To<svetit::api::Projects>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Projects& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

