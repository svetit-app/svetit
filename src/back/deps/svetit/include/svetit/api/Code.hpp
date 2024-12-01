#pragma once

#include "Code_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Code {
  static constexpr auto kIdMinimum = 0;

  // CppStringWithFormat

  // CppStringWithFormat

  std::optional<std::int64_t> id{};
  boost::uuids::uuid projectId{};
  boost::uuids::uuid repositoryId{};
  std::string commitHash{};
};

bool operator==(const svetit::api::Code& lhs, const svetit::api::Code& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Code& value);

Code Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::Code>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Code& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

