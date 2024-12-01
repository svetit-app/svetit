#include "Project.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "Project_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::Project& lhs,
                const svetit::api::Project& rhs) {
  return lhs.id == rhs.id && lhs.spaceId == rhs.spaceId && lhs.key == rhs.key &&
         lhs.name == rhs.name && lhs.description == rhs.description &&
         lhs.changedAt == rhs.changedAt && lhs.sync == rhs.sync && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Project::Sync& value) {
  return lh << ToString(value);
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Project& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

Project::Sync Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

Project Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::Project> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

svetit::api::Project::Sync FromString(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync>) {
  const auto result =
      ksvetit__api__Project__Sync_Mapping.TryFindBySecond(value);
  if (result.has_value()) {
    return *result;
  }
  throw std::runtime_error(fmt::format(
      "Invalid enum value ({}) for type svetit::api::Project::Sync", value));
}

svetit::api::Project::Sync Parse(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync> to) {
  return FromString(value, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Project::Sync& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  return USERVER_NAMESPACE::formats::json::ValueBuilder(ToString(value))
      .ExtractValue();
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::Project& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::WithType<
        USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
        *value.id};
  }

  vb["spaceId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.spaceId};

  vb["key"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<32>>{value.key};

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  if (value.description) {
    vb["description"] =
        USERVER_NAMESPACE::chaotic::Primitive<std::string>{*value.description};
  }

  if (value.changedAt) {
    vb["changedAt"] =
        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{*value.changedAt};
  }

  vb["sync"] =
      USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Project::Sync>{
          value.sync};

  return vb.ExtractValue();
}

std::string ToString(svetit::api::Project::Sync value) {
  const auto result = ksvetit__api__Project__Sync_Mapping.TryFindByFirst(value);
  if (result.has_value()) {
    return std::string{*result};
  }
  throw std::runtime_error("Bad enum value");
}

}  // namespace api
}  // namespace svetit

