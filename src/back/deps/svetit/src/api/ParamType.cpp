#include "ParamType.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "ParamType_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::ParamType& lhs,
                const svetit::api::ParamType& rhs) {
  return lhs.id == rhs.id && lhs.projectId == rhs.projectId &&
         lhs.parentId == rhs.parentId && lhs.key == rhs.key &&
         lhs.name == rhs.name && lhs.description == rhs.description &&
         lhs.valueType == rhs.valueType && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ParamType::Valuetype& value) {
  return lh << ToString(value);
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ParamType& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

ParamType::Valuetype Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>
        to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

ParamType Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

svetit::api::ParamType::Valuetype FromString(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>) {
  const auto result =
      ksvetit__api__ParamType__Valuetype_Mapping.TryFindBySecond(value);
  if (result.has_value()) {
    return *result;
  }
  throw std::runtime_error(fmt::format(
      "Invalid enum value ({}) for type svetit::api::ParamType::Valuetype",
      value));
}

svetit::api::ParamType::Valuetype Parse(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>
        to) {
  return FromString(value, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ParamType::Valuetype& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  return USERVER_NAMESPACE::formats::json::ValueBuilder(ToString(value))
      .ExtractValue();
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::ParamType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::ParamType::kIdMinimum>>{*value.id};
  }

  if (value.projectId) {
    vb["projectId"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.projectId};
  }

  if (value.parentId) {
    vb["parentId"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.parentId};
  }

  vb["key"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.key};

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  vb["description"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<512>>{value.description};

  vb["valueType"] =
      USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ParamType::Valuetype>{
          value.valueType};

  return vb.ExtractValue();
}

std::string ToString(svetit::api::ParamType::Valuetype value) {
  const auto result =
      ksvetit__api__ParamType__Valuetype_Mapping.TryFindByFirst(value);
  if (result.has_value()) {
    return std::string{*result};
  }
  throw std::runtime_error("Bad enum value");
}

}  // namespace api
}  // namespace svetit

