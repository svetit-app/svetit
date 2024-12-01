#pragma once

#include "Links_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Link.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Links {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Link> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Links& lhs, const svetit::api::Links& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Links& value);

Links Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Links>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Links& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

