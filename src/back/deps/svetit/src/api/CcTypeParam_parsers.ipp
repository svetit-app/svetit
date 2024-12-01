#pragma once

#include "CcTypeParam.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__CcTypeParam_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("ccTypeId")
          .Case("paramId");
    };

template <typename Value>
svetit::api::CcTypeParam Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeParam>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::CcTypeParam res;

  res.ccTypeId =
      value["ccTypeId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::CcTypeParam::kCctypeidMinimum>>>();
  res.paramId =
      value["paramId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::CcTypeParam::kParamidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__CcTypeParam_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

