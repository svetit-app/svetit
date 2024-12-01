#pragma once

#include "GroupUser.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__GroupUser_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("nodeId").Case(
          "userId");
    };

template <typename Value>
svetit::api::GroupUser Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::GroupUser>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::GroupUser res;

  res.nodeId =
      value["nodeId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::GroupUser::kNodeidMinimum>>>();
  res.userId = value["userId"]
                   .template As<USERVER_NAMESPACE::chaotic::Primitive<
                       std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                       USERVER_NAMESPACE::chaotic::MaxLength<40>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__GroupUser_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

