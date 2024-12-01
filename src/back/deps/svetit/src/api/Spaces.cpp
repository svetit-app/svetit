#include "Spaces.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Spaces_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Spaces& lhs,
                const svetit::api::Spaces& rhs) {
  return lhs.list == rhs.list && lhs.total == rhs.total && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Spaces& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Spaces Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::Spaces> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Spaces& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["list"] = USERVER_NAMESPACE::chaotic::Array<
      USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Space>,
      std::vector<svetit::api::Space>,
      USERVER_NAMESPACE::chaotic::MaxItems<100>>{value.list};

  vb["total"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t,
      USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Spaces::kTotalMinimum>>{
      value.total};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

