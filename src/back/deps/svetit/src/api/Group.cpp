#include "Group.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Group_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Group& lhs, const svetit::api::Group& rhs) {
  return lhs.id == rhs.id && lhs.name == rhs.name &&
         lhs.description == rhs.description && lhs.spaceId == rhs.spaceId &&
         true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Group& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Group Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Group> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Group& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{*value.id};
  }

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  vb["description"] =
      USERVER_NAMESPACE::chaotic::Primitive<std::string>{value.description};

  if (value.spaceId) {
    vb["spaceId"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.spaceId};
  }

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

