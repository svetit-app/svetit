#pragma once

#include "Translations_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Translation.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Translations {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Translation> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Translations& lhs,
                const svetit::api::Translations& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Translations& value);

Translations Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Translations>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Translations& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

