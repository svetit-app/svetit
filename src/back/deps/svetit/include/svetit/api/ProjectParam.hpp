#pragma once

#include "ProjectParam_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct ProjectParam {
  // CppStringWithFormat

  static constexpr auto kParamidMinimum = 0;

  boost::uuids::uuid projectId{};
  std::int64_t paramId{};
};

bool operator==(const svetit::api::ProjectParam& lhs,
                const svetit::api::ProjectParam& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ProjectParam& value);

ProjectParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ProjectParam>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ProjectParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

