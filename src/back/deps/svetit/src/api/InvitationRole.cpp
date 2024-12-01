#include "InvitationRole.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "InvitationRole_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::InvitationRole& lhs,
                const svetit::api::InvitationRole& rhs) {
  return lhs.roleId == rhs.roleId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::InvitationRole& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

InvitationRole Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::InvitationRole> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::InvitationRole& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["roleId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::InvitationRole::kRoleidMinimum>>{
      value.roleId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

