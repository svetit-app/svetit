#pragma once

#include "CcType_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcType {
  static constexpr auto kIdMinimum = 0;

  // CppStringWithFormat

  std::optional<std::int64_t> id{};
  boost::uuids::uuid projectId{};
  std::string key{};
  std::string name{};
  std::string description{};
};

bool operator==(const svetit::api::CcType& lhs, const svetit::api::CcType& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcType& value);

CcType Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::CcType>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

