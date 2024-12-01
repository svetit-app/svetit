#pragma once

#include "ParamType.hpp"

#include <userver/chaotic/exception.hpp>
#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialBiMap
    ksvetit__api__ParamType__Valuetype_Mapping = [](auto selector) {
      return selector()
          .template Type<svetit::api::ParamType::Valuetype, std::string_view>()
          .Case(svetit::api::ParamType::Valuetype::kInt, "int")
          .Case(svetit::api::ParamType::Valuetype::kBool, "bool")
          .Case(svetit::api::ParamType::Valuetype::kFloat, "float")
          .Case(svetit::api::ParamType::Valuetype::kString, "string")
          .Case(svetit::api::ParamType::Valuetype::kBytes, "bytes")
          .Case(svetit::api::ParamType::Valuetype::kTime, "time")
          .Case(svetit::api::ParamType::Valuetype::kRange, "range")
          .Case(svetit::api::ParamType::Valuetype::kCombo, "combo");
    };

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__ParamType_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("projectId")
          .Case("parentId")
          .Case("key")
          .Case("name")
          .Case("description")
          .Case("valueType");
    };

template <typename Value>
svetit::api::ParamType::Valuetype Parse(
    Value val,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>) {
  const auto value = val.template As<std::string>();
  const auto result =
      ksvetit__api__ParamType__Valuetype_Mapping.TryFindBySecond(value);
  if (result.has_value()) {
    return *result;
  }
  USERVER_NAMESPACE::chaotic::ThrowForValue(
      fmt::format(
          "Invalid enum value ({}) for type svetit::api::ParamType::Valuetype",
          value),
      val);
}

template <typename Value>
svetit::api::ParamType Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::ParamType res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
                   std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                     svetit::api::ParamType::kIdMinimum>>>>();
  res.projectId =
      value["projectId"]
          .template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<
              USERVER_NAMESPACE::chaotic::Primitive<std::string>,
              boost::uuids::uuid>>>();
  res.parentId = value["parentId"]
                     .template As<USERVER_NAMESPACE::chaotic::WithType<
                         USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                         boost::uuids::uuid>>();
  res.key = value["key"]
                .template As<USERVER_NAMESPACE::chaotic::Primitive<
                    std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                    USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.description =
      value["description"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
              USERVER_NAMESPACE::chaotic::MaxLength<512>>>();
  res.valueType = value["valueType"]
                      .template As<USERVER_NAMESPACE::chaotic::Primitive<
                          svetit::api::ParamType::Valuetype>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__ParamType_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

