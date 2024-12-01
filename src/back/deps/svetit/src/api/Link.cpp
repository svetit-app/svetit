#include "Link.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Link_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Link& lhs, const svetit::api::Link& rhs) {
  return lhs.id == rhs.id && lhs.spaceId == rhs.spaceId &&
         lhs.creatorId == rhs.creatorId && lhs.name == rhs.name &&
         lhs.createdAt == rhs.createdAt && lhs.expiredAt == rhs.expiredAt &&
         true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Link& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Link Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::Link> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Link& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.id};
  }

  vb["spaceId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.spaceId};

  if (value.creatorId) {
    vb["creatorId"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
        *value.creatorId};
  }

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.name};

  if (value.createdAt) {
    vb["createdAt"] =
        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{*value.createdAt};
  }

  vb["expiredAt"] =
      USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{value.expiredAt};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

