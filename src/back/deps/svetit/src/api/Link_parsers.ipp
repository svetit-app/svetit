#pragma once

#include "Link.hpp"

#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__Link_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("spaceId")
          .Case("creatorId")
          .Case("name")
          .Case("createdAt")
          .Case("expiredAt");
    };

template <typename Value>
svetit::api::Link Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::Link>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::Link res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<
                   USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                   boost::uuids::uuid>>>();
  res.spaceId = value["spaceId"]
                    .template As<USERVER_NAMESPACE::chaotic::WithType<
                        USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                        boost::uuids::uuid>>();
  res.creatorId =
      value["creatorId"]
          .template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<
              std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>();
  res.createdAt =
      value["createdAt"]
          .template As<std::optional<
              USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>>();
  res.expiredAt =
      value["expiredAt"]
          .template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__Link_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

