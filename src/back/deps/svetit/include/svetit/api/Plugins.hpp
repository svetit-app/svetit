#pragma once

#include "Plugins_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Plugin.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Plugins {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Plugin> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Plugins& lhs,
                const svetit::api::Plugins& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Plugins& value);

Plugins Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::Plugins>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Plugins& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

