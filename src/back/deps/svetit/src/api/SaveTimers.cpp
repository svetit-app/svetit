#include "SaveTimers.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "SaveTimers_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::SaveTimers& lhs,
                const svetit::api::SaveTimers& rhs) {
  return lhs.list == rhs.list && lhs.total == rhs.total && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::SaveTimers& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

SaveTimers Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SaveTimers> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::SaveTimers& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["list"] = USERVER_NAMESPACE::chaotic::Array<
      USERVER_NAMESPACE::chaotic::Primitive<svetit::api::SaveTimer>,
      std::vector<svetit::api::SaveTimer>,
      USERVER_NAMESPACE::chaotic::MaxItems<100>>{value.list};

  vb["total"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::SaveTimers::kTotalMinimum>>{value.total};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

