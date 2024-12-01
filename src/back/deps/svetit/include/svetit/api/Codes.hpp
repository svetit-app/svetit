#pragma once

#include "Codes_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Code.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Codes {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Code> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Codes& lhs, const svetit::api::Codes& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Codes& value);

Codes Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Codes>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Codes& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

