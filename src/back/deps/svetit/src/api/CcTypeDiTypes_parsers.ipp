#pragma once

#include "CcTypeDiTypes.hpp"

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
    ksvetit__api__CcTypeDiTypes_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("list").Case(
          "total");
    };

template <typename Value>
svetit::api::CcTypeDiTypes Parse(
    Value value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiTypes>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::CcTypeDiTypes res;

  res.list =
      value["list"]
          .template As<USERVER_NAMESPACE::chaotic::Array<
              USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcTypeDiType>,
              std::vector<svetit::api::CcTypeDiType>,
              USERVER_NAMESPACE::chaotic::MaxItems<100>>>();
  res.total =
      value["total"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<
              std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                                svetit::api::CcTypeDiTypes::kTotalMinimum>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__CcTypeDiTypes_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

