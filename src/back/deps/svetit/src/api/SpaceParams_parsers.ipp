#pragma once

#include "SpaceParams.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__SpaceParams_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("canCreate")
          .Case("invitationSize");
    };

template <typename Value>
svetit::api::SpaceParams Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SpaceParams>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::SpaceParams res;

  res.canCreate =
      value["canCreate"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<bool>>();
  res.invitationSize =
      value["invitationSize"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int32_t,
              USERVER_NAMESPACE::chaotic::Minimum<
                  svetit::api::SpaceParams::kInvitationsizeMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__SpaceParams_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

