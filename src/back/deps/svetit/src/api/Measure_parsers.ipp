#pragma once

#include "Measure.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__Measure_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("projectId")
          .Case("name");
    };

template <typename Value>
svetit::api::Measure Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::Measure>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::Measure res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
                   std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                     svetit::api::Measure::kIdMinimum>>>>();
  res.projectId = value["projectId"]
                      .template As<USERVER_NAMESPACE::chaotic::WithType<
                          USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                          boost::uuids::uuid>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<10>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__Measure_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

