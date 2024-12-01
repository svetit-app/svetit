#pragma once

#include "CcParam.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__CcParam_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("ccId").Case(
          "paramId");
    };

template <typename Value>
svetit::api::CcParam Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::CcParam>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::CcParam res;

  res.ccId = value["ccId"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                       svetit::api::CcParam::kCcidMinimum>>>();
  res.paramId =
      value["paramId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::CcParam::kParamidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__CcParam_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

