#include "DeviceItem.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "DeviceItem_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::DeviceItem& lhs,
                const svetit::api::DeviceItem& rhs) {
  return lhs.id == rhs.id && lhs.deviceId == rhs.deviceId &&
         lhs.typeId == rhs.typeId && lhs.name == rhs.name && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DeviceItem& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

DeviceItem Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DeviceItem> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::DeviceItem& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::DeviceItem::kIdMinimum>>{*value.id};
  }

  vb["deviceId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::DeviceItem::kDeviceidMinimum>>{
      value.deviceId};

  vb["typeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::DeviceItem::kTypeidMinimum>>{value.typeId};

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

