#include "Device.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Device_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Device& lhs,
                const svetit::api::Device& rhs) {
  return lhs.id == rhs.id && lhs.projectId == rhs.projectId &&
         lhs.pluginId == rhs.pluginId && lhs.name == rhs.name &&
         lhs.checkIntervalMsec == rhs.checkIntervalMsec && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Device& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Device Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::Device> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Device& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t,
        USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Device::kIdMinimum>>{
        *value.id};
  }

  vb["projectId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.projectId};

  vb["pluginId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::Device::kPluginidMinimum>>{value.pluginId};

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  vb["checkIntervalMsec"] = USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{
      value.checkIntervalMsec};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

