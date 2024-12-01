#pragma once

#include "Translation_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Translation {
  static constexpr auto kIdMinimum = 0;

  // CppStringWithFormat

  std::optional<std::int64_t> id{};
  boost::uuids::uuid projectId{};
  std::string lang{};
  std::string key{};
  std::string value{};
};

bool operator==(const svetit::api::Translation& lhs,
                const svetit::api::Translation& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Translation& value);

Translation Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Translation>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Translation& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

