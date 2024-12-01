#include "Section.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Section_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Section& lhs,
                const svetit::api::Section& rhs) {
  return lhs.id == rhs.id && lhs.projectId == rhs.projectId &&
         lhs.name == rhs.name && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Section& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Section Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::Section> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Section& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t,
        USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Section::kIdMinimum>>{
        *value.id};
  }

  vb["projectId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.projectId};

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

