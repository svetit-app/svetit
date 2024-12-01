#pragma once

#include "SaveTimer.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__SaveTimer_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("projectId")
          .Case("intervalMsec");
    };

template <typename Value>
svetit::api::SaveTimer Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SaveTimer>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::SaveTimer res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
                   std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                     svetit::api::SaveTimer::kIdMinimum>>>>();
  res.projectId = value["projectId"]
                      .template As<USERVER_NAMESPACE::chaotic::WithType<
                          USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                          boost::uuids::uuid>>();
  res.intervalMsec =
      value["intervalMsec"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__SaveTimer_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

