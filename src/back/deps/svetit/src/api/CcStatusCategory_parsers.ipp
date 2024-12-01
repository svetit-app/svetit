#pragma once

#include "CcStatusCategory.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__CcStatusCategory_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("projectId")
          .Case("key")
          .Case("name")
          .Case("color");
    };

template <typename Value>
svetit::api::CcStatusCategory Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusCategory>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::CcStatusCategory res;

  res.id =
      value["id"]
          .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::CcStatusCategory::kIdMinimum>>>>();
  res.projectId = value["projectId"]
                      .template As<USERVER_NAMESPACE::chaotic::WithType<
                          USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                          boost::uuids::uuid>>();
  res.key = value["key"]
                .template As<USERVER_NAMESPACE::chaotic::Primitive<
                    std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                    USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.color = value["color"]
                  .template As<USERVER_NAMESPACE::chaotic::Primitive<
                      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                      USERVER_NAMESPACE::chaotic::MaxLength<16>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__CcStatusCategory_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

