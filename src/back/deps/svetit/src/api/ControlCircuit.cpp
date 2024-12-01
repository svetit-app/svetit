#include "ControlCircuit.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "ControlCircuit_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::ControlCircuit& lhs,
                const svetit::api::ControlCircuit& rhs) {
  return lhs.id == rhs.id && lhs.typeId == rhs.typeId &&
         lhs.sectionId == rhs.sectionId && lhs.name == rhs.name && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ControlCircuit& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

ControlCircuit Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ControlCircuit> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::ControlCircuit& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::ControlCircuit::kIdMinimum>>{*value.id};
  }

  vb["typeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::ControlCircuit::kTypeidMinimum>>{
      value.typeId};

  vb["sectionId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::ControlCircuit::kSectionidMinimum>>{
      value.sectionId};

  vb["name"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.name};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

