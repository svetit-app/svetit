#pragma once

#include "User.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__User_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("userId")
          .Case("spaceId")
          .Case("isOwner")
          .Case("joinedAt")
          .Case("roleId");
    };

template <typename Value>
svetit::api::User Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::User>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::User res;

  res.userId =
      value["userId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();
  res.spaceId = value["spaceId"]
                    .template As<USERVER_NAMESPACE::chaotic::WithType<
                        USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                        boost::uuids::uuid>>();
  res.isOwner = value["isOwner"]
                    .template As<USERVER_NAMESPACE::chaotic::Primitive<bool>>();
  res.joinedAt =
      value["joinedAt"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>();
  res.roleId = value["roleId"]
                   .template As<USERVER_NAMESPACE::chaotic::Primitive<
                       std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                                         svetit::api::User::kRoleidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__User_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

