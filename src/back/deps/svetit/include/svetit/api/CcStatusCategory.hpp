#pragma once

#include "CcStatusCategory_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcStatusCategory {
  static constexpr auto kIdMinimum = 0;

  // CppStringWithFormat

  std::optional<std::int64_t> id{};
  boost::uuids::uuid projectId{};
  std::string key{};
  std::string name{};
  std::string color{};
};

bool operator==(const svetit::api::CcStatusCategory& lhs,
                const svetit::api::CcStatusCategory& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcStatusCategory& value);

CcStatusCategory Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusCategory>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcStatusCategory& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

