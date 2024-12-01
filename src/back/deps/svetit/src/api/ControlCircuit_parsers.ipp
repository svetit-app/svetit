#pragma once

#include "ControlCircuit.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__ControlCircuit_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("typeId")
          .Case("sectionId")
          .Case("name");
    };

template <typename Value>
svetit::api::ControlCircuit Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ControlCircuit>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::ControlCircuit res;

  res.id =
      value["id"]
          .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::ControlCircuit::kIdMinimum>>>>();
  res.typeId = value["typeId"]
                   .template As<USERVER_NAMESPACE::chaotic::Primitive<
                       std::int64_t,
                       USERVER_NAMESPACE::chaotic::Minimum<
                           svetit::api::ControlCircuit::kTypeidMinimum>>>();
  res.sectionId =
      value["sectionId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t,
              USERVER_NAMESPACE::chaotic::Minimum<
                  svetit::api::ControlCircuit::kSectionidMinimum>>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<64>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__ControlCircuit_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

