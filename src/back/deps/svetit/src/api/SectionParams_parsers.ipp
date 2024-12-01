#pragma once

#include "SectionParams.hpp"

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
    ksvetit__api__SectionParams_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("list").Case(
          "total");
    };

template <typename Value>
svetit::api::SectionParams Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SectionParams>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::SectionParams res;

  res.list =
      value["list"]
          .template As<USERVER_NAMESPACE::chaotic::Array<
              USERVER_NAMESPACE::chaotic::Primitive<svetit::api::SectionParam>,
              std::vector<svetit::api::SectionParam>,
              USERVER_NAMESPACE::chaotic::MaxItems<100>>>();
  res.total =
      value["total"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::SectionParams::kTotalMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__SectionParams_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

