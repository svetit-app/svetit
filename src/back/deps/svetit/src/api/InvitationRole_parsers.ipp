#pragma once

#include "InvitationRole.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__InvitationRole_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("roleId");
    };

template <typename Value>
svetit::api::InvitationRole Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::InvitationRole>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::InvitationRole res;

  res.roleId = value["roleId"]
                   .template As<USERVER_NAMESPACE::chaotic::Primitive<
                       std::int32_t,
                       USERVER_NAMESPACE::chaotic::Minimum<
                           svetit::api::InvitationRole::kRoleidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__InvitationRole_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

