#include "DevicePluginParam.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "DevicePluginParam_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::DevicePluginParam& lhs,
                const svetit::api::DevicePluginParam& rhs) {
  return lhs.deviceId == rhs.deviceId && lhs.paramId == rhs.paramId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DevicePluginParam& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

DevicePluginParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DevicePluginParam> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::DevicePluginParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["deviceId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::DevicePluginParam::kDeviceidMinimum>>{
      value.deviceId};

  vb["paramId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::DevicePluginParam::kParamidMinimum>>{
      value.paramId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

