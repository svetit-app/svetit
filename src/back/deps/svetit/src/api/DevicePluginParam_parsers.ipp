#pragma once

#include "DevicePluginParam.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__DevicePluginParam_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("deviceId")
          .Case("paramId");
    };

template <typename Value>
svetit::api::DevicePluginParam Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DevicePluginParam>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::DevicePluginParam res;

  res.deviceId =
      value["deviceId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t,
              USERVER_NAMESPACE::chaotic::Minimum<
                  svetit::api::DevicePluginParam::kDeviceidMinimum>>>();
  res.paramId =
      value["paramId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t,
              USERVER_NAMESPACE::chaotic::Minimum<
                  svetit::api::DevicePluginParam::kParamidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__DevicePluginParam_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

