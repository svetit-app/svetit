#pragma once

#include "NodeProject.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__NodeProject_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("nodeId").Case(
          "projectId");
    };

template <typename Value>
svetit::api::NodeProject Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeProject>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::NodeProject res;

  res.nodeId = value["nodeId"]
                   .template As<USERVER_NAMESPACE::chaotic::WithType<
                       USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                       boost::uuids::uuid>>();
  res.projectId = value["projectId"]
                      .template As<USERVER_NAMESPACE::chaotic::WithType<
                          USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                          boost::uuids::uuid>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__NodeProject_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

