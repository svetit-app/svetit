#include "Space.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Space_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Space& lhs, const svetit::api::Space& rhs) {
  return lhs.id == rhs.id && lhs.name == rhs.name && lhs.key == rhs.key &&
         lhs.requestsAllowed == rhs.requestsAllowed &&
         lhs.createdAt == rhs.createdAt && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Space& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Space Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Space> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Space& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.id};
  }

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.name};

  vb["key"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.key};

  vb["requestsAllowed"] =
      USERVER_NAMESPACE::chaotic::Primitive<bool>{value.requestsAllowed};

  if (value.createdAt) {
    vb["createdAt"] =
        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{*value.createdAt};
  }

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit
