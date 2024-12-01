#pragma once

#include "ProjectParam.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__ProjectParam_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("projectId")
          .Case("paramId");
    };

template <typename Value>
svetit::api::ProjectParam Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ProjectParam>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::ProjectParam res;

  res.projectId = value["projectId"]
                      .template As<USERVER_NAMESPACE::chaotic::WithType<
                          USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                          boost::uuids::uuid>>();
  res.paramId =
      value["paramId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::ProjectParam::kParamidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__ProjectParam_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

