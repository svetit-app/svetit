#pragma once

#include "Users_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <User.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Users {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::User> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Users& lhs, const svetit::api::Users& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Users& value);

Users Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Users>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Users& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

