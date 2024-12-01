#include "Error.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Error_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Error& lhs, const svetit::api::Error& rhs) {
  return lhs.code == rhs.code && lhs.message == rhs.message && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Error& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Error Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Error> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Error& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["code"] = USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{value.code};

  vb["message"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.message};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

