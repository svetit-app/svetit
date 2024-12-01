#include "User.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "User_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::User& lhs, const svetit::api::User& rhs) {
  return lhs.userId == rhs.userId && lhs.spaceId == rhs.spaceId &&
         lhs.isOwner == rhs.isOwner && lhs.joinedAt == rhs.joinedAt &&
         lhs.roleId == rhs.roleId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::User& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

User Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::User> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::User& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["userId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.userId};

  vb["spaceId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.spaceId};

  vb["isOwner"] = USERVER_NAMESPACE::chaotic::Primitive<bool>{value.isOwner};

  vb["joinedAt"] =
      USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{value.joinedAt};

  vb["roleId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int32_t,
      USERVER_NAMESPACE::chaotic::Minimum<svetit::api::User::kRoleidMinimum>>{
      value.roleId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

