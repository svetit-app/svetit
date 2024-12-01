#pragma once

#include "NodeGroup.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__NodeGroup_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("nodeId").Case(
          "groupId");
    };

template <typename Value>
svetit::api::NodeGroup Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeGroup>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::NodeGroup res;

  res.nodeId = value["nodeId"]
                   .template As<USERVER_NAMESPACE::chaotic::WithType<
                       USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                       boost::uuids::uuid>>();
  res.groupId =
      value["groupId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::NodeGroup::kGroupidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__NodeGroup_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

