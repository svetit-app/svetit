#pragma once

#include "Measures.hpp"

#include <userver/chaotic/array.hpp>
#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__Measures_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("list").Case(
          "total");
    };

template <typename Value>
svetit::api::Measures Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::Measures>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::Measures res;

  res.list =
      value["list"]
          .template As<USERVER_NAMESPACE::chaotic::Array<
              USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Measure>,
              std::vector<svetit::api::Measure>,
              USERVER_NAMESPACE::chaotic::MaxItems<100>>>();
  res.total =
      value["total"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::Measures::kTotalMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__Measures_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit
