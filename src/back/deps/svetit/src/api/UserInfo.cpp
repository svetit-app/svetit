#include "UserInfo.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "UserInfo_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::UserInfo& lhs,
                const svetit::api::UserInfo& rhs) {
  return lhs.id == rhs.id && lhs.displayName == rhs.displayName &&
         lhs.login == rhs.login && lhs.firstname == rhs.firstname &&
         lhs.lastname == rhs.lastname && lhs.email == rhs.email && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::UserInfo& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

UserInfo Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfo> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::UserInfo& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.id};
  }

  vb["displayName"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.displayName};

  vb["login"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.login};

  vb["firstname"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.firstname};

  vb["lastname"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.lastname};

  vb["email"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.email};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

