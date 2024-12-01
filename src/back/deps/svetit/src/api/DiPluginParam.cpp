#include "DiPluginParam.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "DiPluginParam_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::DiPluginParam& lhs,
                const svetit::api::DiPluginParam& rhs) {
  return lhs.diTypeId == rhs.diTypeId && lhs.paramId == rhs.paramId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiPluginParam& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

DiPluginParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiPluginParam> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::DiPluginParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["diTypeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::DiPluginParam::kDitypeidMinimum>>{
      value.diTypeId};

  vb["paramId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::DiPluginParam::kParamidMinimum>>{
      value.paramId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

