#include "CcParam.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "CcParam_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::CcParam& lhs,
                const svetit::api::CcParam& rhs) {
  return lhs.ccId == rhs.ccId && lhs.paramId == rhs.paramId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcParam& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

CcParam Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::CcParam> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::CcParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["ccId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t,
      USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcParam::kCcidMinimum>>{
      value.ccId};

  vb["paramId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::CcParam::kParamidMinimum>>{value.paramId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

