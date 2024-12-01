#pragma once

#include "UserInfo.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__UserInfo_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("displayName")
          .Case("login")
          .Case("firstname")
          .Case("lastname")
          .Case("email");
    };

template <typename Value>
svetit::api::UserInfo Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfo>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::UserInfo res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<
                   USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                   boost::uuids::uuid>>>();
  res.displayName =
      value["displayName"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();
  res.login = value["login"]
                  .template As<USERVER_NAMESPACE::chaotic::Primitive<
                      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();
  res.firstname =
      value["firstname"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();
  res.lastname =
      value["lastname"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();
  res.email = value["email"]
                  .template As<USERVER_NAMESPACE::chaotic::Primitive<
                      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__UserInfo_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

