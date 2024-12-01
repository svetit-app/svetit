#include "SpaceParams.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "SpaceParams_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::SpaceParams& lhs,
                const svetit::api::SpaceParams& rhs) {
  return lhs.canCreate == rhs.canCreate &&
         lhs.invitationSize == rhs.invitationSize && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::SpaceParams& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

SpaceParams Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SpaceParams> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::SpaceParams& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["canCreate"] =
      USERVER_NAMESPACE::chaotic::Primitive<bool>{value.canCreate};

  vb["invitationSize"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::SpaceParams::kInvitationsizeMinimum>>{
      value.invitationSize};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

