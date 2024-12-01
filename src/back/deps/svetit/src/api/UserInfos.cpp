#include "UserInfos.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "UserInfos_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::UserInfos& lhs,
                const svetit::api::UserInfos& rhs) {
  return lhs.list == rhs.list && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::UserInfos& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

UserInfos Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfos> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::UserInfos& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["list"] = USERVER_NAMESPACE::chaotic::Array<
      USERVER_NAMESPACE::chaotic::Primitive<svetit::api::UserInfo>,
      std::vector<svetit::api::UserInfo>,
      USERVER_NAMESPACE::chaotic::MaxItems<100>>{value.list};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

