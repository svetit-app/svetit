#include "Node.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Node_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Node& lhs, const svetit::api::Node& rhs) {
  return lhs.id == rhs.id && lhs.spaceId == rhs.spaceId &&
         lhs.name == rhs.name && lhs.description == rhs.description &&
         lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude &&
         lhs.createdAt == rhs.createdAt && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Node& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Node Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::Node> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Node& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.id};
  }

  if (value.spaceId) {
    vb["spaceId"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.spaceId};
  }

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  vb["description"] =
      USERVER_NAMESPACE::chaotic::Primitive<std::string>{value.description};

  vb["latitude"] =
      USERVER_NAMESPACE::chaotic::Primitive<double>{value.latitude};

  vb["longitude"] =
      USERVER_NAMESPACE::chaotic::Primitive<double>{value.longitude};

  if (value.createdAt) {
    vb["createdAt"] =
        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{*value.createdAt};
  }

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

