#pragma once

#include "CcStatusType.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__CcStatusType_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("ccTypeId")
          .Case("categoryId")
          .Case("key")
          .Case("text")
          .Case("inform");
    };

template <typename Value>
svetit::api::CcStatusType Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusType>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::CcStatusType res;

  res.id =
      value["id"]
          .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::CcStatusType::kIdMinimum>>>>();
  res.ccTypeId = value["ccTypeId"]
                     .template As<USERVER_NAMESPACE::chaotic::Primitive<
                         std::int64_t,
                         USERVER_NAMESPACE::chaotic::Minimum<
                             svetit::api::CcStatusType::kCctypeidMinimum>>>();
  res.categoryId =
      value["categoryId"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t,
              USERVER_NAMESPACE::chaotic::Minimum<
                  svetit::api::CcStatusType::kCategoryidMinimum>>>();
  res.key = value["key"]
                .template As<USERVER_NAMESPACE::chaotic::Primitive<
                    std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                    USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.text = value["text"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<512>>>();
  res.inform = value["inform"]
                   .template As<USERVER_NAMESPACE::chaotic::Primitive<bool>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__CcStatusType_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

