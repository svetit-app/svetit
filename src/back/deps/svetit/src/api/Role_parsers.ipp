#pragma once

#include "Role.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__Role_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("id").Case(
          "name");
    };

template <typename Value>
svetit::api::Role Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::Role>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::Role res;

  res.id = value["id"]
               .template As<std::optional<
                   USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<64>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__Role_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

