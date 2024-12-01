#pragma once

#include "Groups_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Group.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Groups {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Group> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Groups& lhs, const svetit::api::Groups& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Groups& value);

Groups Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::Groups>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Groups& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

