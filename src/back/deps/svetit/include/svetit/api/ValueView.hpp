#pragma once

#include "ValueView_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct ValueView {
  static constexpr auto kIdMinimum = 0;

  static constexpr auto kDitypeidMinimum = 1;

  std::optional<std::int64_t> id{};
  std::int64_t diTypeId{};
  std::string value{};
  std::string view{};
};

bool operator==(const svetit::api::ValueView& lhs,
                const svetit::api::ValueView& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ValueView& value);

ValueView Parse(USERVER_NAMESPACE::formats::json::Value json,
                USERVER_NAMESPACE::formats::parse::To<svetit::api::ValueView>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ValueView& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

