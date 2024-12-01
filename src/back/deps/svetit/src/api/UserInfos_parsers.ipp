#pragma once

#include "UserInfos.hpp"

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
    ksvetit__api__UserInfos_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("list");
    };

template <typename Value>
svetit::api::UserInfos Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfos>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::UserInfos res;

  res.list =
      value["list"]
          .template As<USERVER_NAMESPACE::chaotic::Array<
              USERVER_NAMESPACE::chaotic::Primitive<svetit::api::UserInfo>,
              std::vector<svetit::api::UserInfo>,
              USERVER_NAMESPACE::chaotic::MaxItems<100>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__UserInfos_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

