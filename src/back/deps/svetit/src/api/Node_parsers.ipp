#pragma once

#include "Node.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__Node_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("spaceId")
          .Case("name")
          .Case("description")
          .Case("latitude")
          .Case("longitude")
          .Case("createdAt");
    };

template <typename Value>
svetit::api::Node Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::Node>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::Node res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<
                   USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                   boost::uuids::uuid>>>();
  res.spaceId =
      value["spaceId"]
          .template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<
              USERVER_NAMESPACE::chaotic::Primitive<std::string>,
              boost::uuids::uuid>>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.description =
      value["description"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<std::string>>();
  res.latitude =
      value["latitude"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<double>>();
  res.longitude =
      value["longitude"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<double>>();
  res.createdAt =
      value["createdAt"]
          .template As<std::optional<
              USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__Node_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

