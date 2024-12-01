#include "CcTypeDiType.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "CcTypeDiType_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::CcTypeDiType& lhs,
                const svetit::api::CcTypeDiType& rhs) {
  return lhs.ccTypeId == rhs.ccTypeId && lhs.diTypeId == rhs.diTypeId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcTypeDiType& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

CcTypeDiType Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiType> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::CcTypeDiType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["ccTypeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::CcTypeDiType::kCctypeidMinimum>>{
      value.ccTypeId};

  vb["diTypeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::CcTypeDiType::kDitypeidMinimum>>{
      value.diTypeId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

