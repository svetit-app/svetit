#pragma once

#include "ValueView.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__ValueView_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("diTypeId")
          .Case("value")
          .Case("view");
    };

template <typename Value>
svetit::api::ValueView Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ValueView>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::ValueView res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
                   std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                     svetit::api::ValueView::kIdMinimum>>>>();
  res.diTypeId =
      value["diTypeId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::ValueView::kDitypeidMinimum>>>();
  res.value = value["value"]
                  .template As<USERVER_NAMESPACE::chaotic::Primitive<
                      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();
  res.view = value["view"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__ValueView_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

