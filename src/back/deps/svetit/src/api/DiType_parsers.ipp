#pragma once

#include "DiType.hpp"

#include <userver/chaotic/exception.hpp>
#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialBiMap
    ksvetit__api__DiType__Mode_Mapping = [](auto selector) {
      return selector()
          .template Type<svetit::api::DiType::Mode, std::string_view>()
          .Case(svetit::api::DiType::Mode::kReadonlyflag, "readonlyFlag")
          .Case(svetit::api::DiType::Mode::kReadwriteflag, "readwriteFlag")
          .Case(svetit::api::DiType::Mode::kReadonly, "readonly")
          .Case(svetit::api::DiType::Mode::kReadwrite, "readwrite")
          .Case(svetit::api::DiType::Mode::kFile, "file")
          .Case(svetit::api::DiType::Mode::kButton, "button")
          .Case(svetit::api::DiType::Mode::kVideostream, "videoStream");
    };

static constexpr USERVER_NAMESPACE::utils::TrivialBiMap
    ksvetit__api__DiType__Savealgorithm_Mapping = [](auto selector) {
      return selector()
          .template Type<svetit::api::DiType::Savealgorithm, std::string_view>()
          .Case(svetit::api::DiType::Savealgorithm::kOff, "off")
          .Case(svetit::api::DiType::Savealgorithm::kImmediately, "immediately")
          .Case(svetit::api::DiType::Savealgorithm::kBytimer, "byTimer")
          .Case(svetit::api::DiType::Savealgorithm::kBytimerorimmediately,
                "byTimerOrImmediately");
    };

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__DiType_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("measureId")
          .Case("saveTimerId")
          .Case("key")
          .Case("name")
          .Case("mode")
          .Case("saveAlgorithm");
    };

template <typename Value>
svetit::api::DiType::Mode Parse(
    Value val,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode>) {
  const auto value = val.template As<std::string>();
  const auto result = ksvetit__api__DiType__Mode_Mapping.TryFindBySecond(value);
  if (result.has_value()) {
    return *result;
  }
  USERVER_NAMESPACE::chaotic::ThrowForValue(
      fmt::format("Invalid enum value ({}) for type svetit::api::DiType::Mode",
                  value),
      val);
}

template <typename Value>
svetit::api::DiType::Savealgorithm Parse(
    Value val,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>) {
  const auto value = val.template As<std::string>();
  const auto result =
      ksvetit__api__DiType__Savealgorithm_Mapping.TryFindBySecond(value);
  if (result.has_value()) {
    return *result;
  }
  USERVER_NAMESPACE::chaotic::ThrowForValue(
      fmt::format(
          "Invalid enum value ({}) for type svetit::api::DiType::Savealgorithm",
          value),
      val);
}

template <typename Value>
svetit::api::DiType Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::DiType res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
                   std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                     svetit::api::DiType::kIdMinimum>>>>();
  res.measureId =
      value["measureId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::DiType::kMeasureidMinimum>>>();
  res.saveTimerId =
      value["saveTimerId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::DiType::kSavetimeridMinimum>>>();
  res.key = value["key"]
                .template As<USERVER_NAMESPACE::chaotic::Primitive<
                    std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                    USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.mode = value["mode"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     svetit::api::DiType::Mode>>();
  res.saveAlgorithm = value["saveAlgorithm"]
                          .template As<USERVER_NAMESPACE::chaotic::Primitive<
                              svetit::api::DiType::Savealgorithm>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__DiType_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

