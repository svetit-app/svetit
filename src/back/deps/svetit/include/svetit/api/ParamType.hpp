#pragma once

#include "ParamType_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct ParamType {
  static constexpr auto kIdMinimum = 0;

  // CppStringWithFormat

  // CppStringWithFormat

  enum class Valuetype {
    kInt,
    kBool,
    kFloat,
    kString,
    kBytes,
    kTime,
    kRange,
    kCombo,
  };

  static constexpr Valuetype kValuetypeValues[] = {
      Valuetype::kInt,    Valuetype::kBool,  Valuetype::kFloat,
      Valuetype::kString, Valuetype::kBytes, Valuetype::kTime,
      Valuetype::kRange,  Valuetype::kCombo,
  };

  std::optional<std::int64_t> id{};
  std::optional<boost::uuids::uuid> projectId{};
  std::optional<boost::uuids::uuid> parentId{};
  std::string key{};
  std::string name{};
  std::string description{};
  svetit::api::ParamType::Valuetype valueType{};
};

bool operator==(const svetit::api::ParamType& lhs,
                const svetit::api::ParamType& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ParamType::Valuetype& value);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ParamType& value);

ParamType::Valuetype Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>);

ParamType Parse(USERVER_NAMESPACE::formats::json::Value json,
                USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType>);

ParamType::Valuetype FromString(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>);

ParamType::Valuetype Parse(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ParamType::Valuetype& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ParamType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

std::string ToString(svetit::api::ParamType::Valuetype value);

}  // namespace api
}  // namespace svetit

