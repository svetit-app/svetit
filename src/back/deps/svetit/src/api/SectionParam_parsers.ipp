#pragma once

#include "SectionParam.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__SectionParam_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("sectionId")
          .Case("paramId");
    };

template <typename Value>
svetit::api::SectionParam Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SectionParam>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::SectionParam res;

  res.sectionId = value["sectionId"]
                      .template As<USERVER_NAMESPACE::chaotic::Primitive<
                          std::int64_t,
                          USERVER_NAMESPACE::chaotic::Minimum<
                              svetit::api::SectionParam::kSectionidMinimum>>>();
  res.paramId =
      value["paramId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::SectionParam::kParamidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__SectionParam_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

