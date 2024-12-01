#pragma once

#include "CcTypeDiType.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__CcTypeDiType_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("ccTypeId")
          .Case("diTypeId");
    };

template <typename Value>
svetit::api::CcTypeDiType Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiType>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::CcTypeDiType res;

  res.ccTypeId = value["ccTypeId"]
                     .template As<USERVER_NAMESPACE::chaotic::Primitive<
                         std::int64_t,
                         USERVER_NAMESPACE::chaotic::Minimum<
                             svetit::api::CcTypeDiType::kCctypeidMinimum>>>();
  res.diTypeId = value["diTypeId"]
                     .template As<USERVER_NAMESPACE::chaotic::Primitive<
                         std::int64_t,
                         USERVER_NAMESPACE::chaotic::Minimum<
                             svetit::api::CcTypeDiType::kDitypeidMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__CcTypeDiType_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

