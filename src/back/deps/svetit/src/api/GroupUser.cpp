#include "GroupUser.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "GroupUser_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::GroupUser& lhs,
                const svetit::api::GroupUser& rhs) {
  return lhs.nodeId == rhs.nodeId && lhs.userId == rhs.userId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::GroupUser& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

GroupUser Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::GroupUser> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::GroupUser& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["nodeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::GroupUser::kNodeidMinimum>>{value.nodeId};

  vb["userId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<40>>{value.userId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

