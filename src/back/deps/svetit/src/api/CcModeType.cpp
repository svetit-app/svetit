#include "CcModeType.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "CcModeType_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::CcModeType& lhs,
                const svetit::api::CcModeType& rhs) {
  return lhs.id == rhs.id && lhs.ccTypeId == rhs.ccTypeId &&
         lhs.key == rhs.key && lhs.name == rhs.name && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcModeType& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

CcModeType Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcModeType> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::CcModeType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::CcModeType::kIdMinimum>>{*value.id};
  }

  vb["ccTypeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::CcModeType::kCctypeidMinimum>>{
      value.ccTypeId};

  vb["key"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.key};

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

