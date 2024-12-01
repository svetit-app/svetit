#pragma once

#include "DiPluginParam.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__DiPluginParam_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("diTypeId")
          .Case("paramId");
    };

template <typename Value>
svetit::api::DiPluginParam Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiPluginParam>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::DiPluginParam res;

  res.diTypeId = value["diTypeId"]
                     .template As<USERVER_NAMESPACE::chaotic::Primitive<
                         std::int64_t,
                         USERVER_NAMESPACE::chaotic::Minimum<
                             svetit::api::DiPluginParam::kDitypeidMinimum>>>();
  res.paramId = value["paramId"]
                    .template As<USERVER_NAMESPACE::chaotic::Primitive<
                        std::int64_t,
                        USERVER_NAMESPACE::chaotic::Minimum<
                            svetit::api::DiPluginParam::kParamidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__DiPluginParam_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

