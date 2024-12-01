#pragma once

#include "Error.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__Error_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("code").Case(
          "message");
    };

template <typename Value>
svetit::api::Error Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::Error>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::Error res;

  res.code =
      value["code"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>>();
  res.message =
      value["message"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__Error_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

