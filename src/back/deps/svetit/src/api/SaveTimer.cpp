#include "SaveTimer.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "SaveTimer_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::SaveTimer& lhs,
                const svetit::api::SaveTimer& rhs) {
  return lhs.id == rhs.id && lhs.projectId == rhs.projectId &&
         lhs.intervalMsec == rhs.intervalMsec && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::SaveTimer& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

SaveTimer Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SaveTimer> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::SaveTimer& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::SaveTimer::kIdMinimum>>{*value.id};
  }

  vb["projectId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.projectId};

  vb["intervalMsec"] =
      USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{value.intervalMsec};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

