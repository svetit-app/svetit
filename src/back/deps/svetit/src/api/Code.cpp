#include "Code.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Code_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Code& lhs, const svetit::api::Code& rhs) {
  return lhs.id == rhs.id && lhs.projectId == rhs.projectId &&
         lhs.repositoryId == rhs.repositoryId &&
         lhs.commitHash == rhs.commitHash && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Code& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Code Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::Code> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Code& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t,
        USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Code::kIdMinimum>>{
        *value.id};
  }

  vb["projectId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.projectId};

  vb["repositoryId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.repositoryId};

  vb["commitHash"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.commitHash};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

