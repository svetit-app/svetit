#include "CcTypeParam.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "CcTypeParam_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::CcTypeParam& lhs,
                const svetit::api::CcTypeParam& rhs) {
  return lhs.ccTypeId == rhs.ccTypeId && lhs.paramId == rhs.paramId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcTypeParam& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

CcTypeParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeParam> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::CcTypeParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["ccTypeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::CcTypeParam::kCctypeidMinimum>>{
      value.ccTypeId};

  vb["paramId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::CcTypeParam::kParamidMinimum>>{
      value.paramId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

