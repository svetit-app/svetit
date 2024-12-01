#include "Invitation.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Invitation_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Invitation& lhs,
                const svetit::api::Invitation& rhs) {
  return lhs.id == rhs.id && lhs.spaceId == rhs.spaceId &&
         lhs.creatorId == rhs.creatorId && lhs.userId == rhs.userId &&
         lhs.roleId == rhs.roleId && lhs.createdAt == rhs.createdAt && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Invitation& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Invitation Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Invitation> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Invitation& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::Invitation::kIdMinimum>>{*value.id};
  }

  vb["spaceId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.spaceId};

  if (value.creatorId) {
    vb["creatorId"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
        *value.creatorId};
  }

  vb["userId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.userId};

  if (value.roleId) {
    vb["roleId"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::Invitation::kRoleidMinimum>>{
        *value.roleId};
  }

  if (value.createdAt) {
    vb["createdAt"] =
        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{*value.createdAt};
  }

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

