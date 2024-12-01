#pragma once

#include "Plugin_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Plugin {
  static constexpr auto kIdMinimum = 0;

  // CppStringWithFormat

  std::optional<std::int64_t> id{};
  boost::uuids::uuid projectId{};
  std::string name{};
  std::string description{};
  std::string key{};
};

bool operator==(const svetit::api::Plugin& lhs, const svetit::api::Plugin& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Plugin& value);

Plugin Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::Plugin>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Plugin& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

