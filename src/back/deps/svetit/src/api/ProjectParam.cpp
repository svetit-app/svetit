#include "ProjectParam.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "ProjectParam_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::ProjectParam& lhs,
                const svetit::api::ProjectParam& rhs) {
  return lhs.projectId == rhs.projectId && lhs.paramId == rhs.paramId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ProjectParam& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

ProjectParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ProjectParam> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::ProjectParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["projectId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.projectId};

  vb["paramId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::ProjectParam::kParamidMinimum>>{
      value.paramId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

