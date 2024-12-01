#include "DiType.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "DiType_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::DiType& lhs,
                const svetit::api::DiType& rhs) {
  return lhs.id == rhs.id && lhs.measureId == rhs.measureId &&
         lhs.saveTimerId == rhs.saveTimerId && lhs.key == rhs.key &&
         lhs.name == rhs.name && lhs.mode == rhs.mode &&
         lhs.saveAlgorithm == rhs.saveAlgorithm && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiType::Mode& value) {
  return lh << ToString(value);
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiType::Savealgorithm& value) {
  return lh << ToString(value);
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiType& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

DiType::Mode Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

DiType::Savealgorithm Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>
        to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

DiType Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

svetit::api::DiType::Mode FromString(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode>) {
  const auto result = ksvetit__api__DiType__Mode_Mapping.TryFindBySecond(value);
  if (result.has_value()) {
    return *result;
  }
  throw std::runtime_error(fmt::format(
      "Invalid enum value ({}) for type svetit::api::DiType::Mode", value));
}

svetit::api::DiType::Mode Parse(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode> to) {
  return FromString(value, to);
}

svetit::api::DiType::Savealgorithm FromString(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>) {
  const auto result =
      ksvetit__api__DiType__Savealgorithm_Mapping.TryFindBySecond(value);
  if (result.has_value()) {
    return *result;
  }
  throw std::runtime_error(fmt::format(
      "Invalid enum value ({}) for type svetit::api::DiType::Savealgorithm",
      value));
}

svetit::api::DiType::Savealgorithm Parse(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>
        to) {
  return FromString(value, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DiType::Mode& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  return USERVER_NAMESPACE::formats::json::ValueBuilder(ToString(value))
      .ExtractValue();
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DiType::Savealgorithm& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  return USERVER_NAMESPACE::formats::json::ValueBuilder(ToString(value))
      .ExtractValue();
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::DiType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t,
        USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiType::kIdMinimum>>{
        *value.id};
  }

  vb["measureId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::DiType::kMeasureidMinimum>>{
      value.measureId};

  vb["saveTimerId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::DiType::kSavetimeridMinimum>>{
      value.saveTimerId};

  vb["key"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.key};

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  vb["mode"] = USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiType::Mode>{
      value.mode};

  vb["saveAlgorithm"] =
      USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiType::Savealgorithm>{
          value.saveAlgorithm};

  return vb.ExtractValue();
}

std::string ToString(svetit::api::DiType::Mode value) {
  const auto result = ksvetit__api__DiType__Mode_Mapping.TryFindByFirst(value);
  if (result.has_value()) {
    return std::string{*result};
  }
  throw std::runtime_error("Bad enum value");
}

std::string ToString(svetit::api::DiType::Savealgorithm value) {
  const auto result =
      ksvetit__api__DiType__Savealgorithm_Mapping.TryFindByFirst(value);
  if (result.has_value()) {
    return std::string{*result};
  }
  throw std::runtime_error("Bad enum value");
}

}  // namespace api
}  // namespace svetit

