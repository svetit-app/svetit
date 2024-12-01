#include "CcDi.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "CcDi_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::CcDi& lhs, const svetit::api::CcDi& rhs) {
  return lhs.ccId == rhs.ccId && lhs.diId == rhs.diId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcDi& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

CcDi Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::CcDi> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::CcDi& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["ccId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t,
      USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcDi::kCcidMinimum>>{
      value.ccId};

  vb["diId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t,
      USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcDi::kDiidMinimum>>{
      value.diId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

