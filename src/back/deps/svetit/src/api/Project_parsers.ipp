#pragma once

#include "Project.hpp"

#include <userver/chaotic/exception.hpp>
#include <userver/chaotic/object.hpp>
#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svetit {
namespace api {

static constexpr USERVER_NAMESPACE::utils::TrivialBiMap
    ksvetit__api__Project__Sync_Mapping = [](auto selector) {
      return selector()
          .template Type<svetit::api::Project::Sync, std::string_view>()
          .Case(svetit::api::Project::Sync::kProjecttonode, "projectToNode")
          .Case(svetit::api::Project::Sync::kNodetoproject, "nodeToProject");
    };

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    ksvetit__api__Project_PropertiesNames = [](auto selector) {
      return selector()
          .template Type<std::string_view>()
          .Case("id")
          .Case("spaceId")
          .Case("key")
          .Case("name")
          .Case("description")
          .Case("changedAt")
          .Case("sync");
    };

template <typename Value>
svetit::api::Project::Sync Parse(
    Value val,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync>) {
  const auto value = val.template As<std::string>();
  const auto result =
      ksvetit__api__Project__Sync_Mapping.TryFindBySecond(value);
  if (result.has_value()) {
    return *result;
  }
  USERVER_NAMESPACE::chaotic::ThrowForValue(
      fmt::format("Invalid enum value ({}) for type svetit::api::Project::Sync",
                  value),
      val);
}

template <typename Value>
svetit::api::Project Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<svetit::api::Project>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  svetit::api::Project res;

  res.id = value["id"]
               .template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<
                   USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                   boost::uuids::uuid>>>();
  res.spaceId = value["spaceId"]
                    .template As<USERVER_NAMESPACE::chaotic::WithType<
                        USERVER_NAMESPACE::chaotic::Primitive<std::string>,
                        boost::uuids::uuid>>();
  res.key = value["key"]
                .template As<USERVER_NAMESPACE::chaotic::Primitive<
                    std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                    USERVER_NAMESPACE::chaotic::MaxLength<32>>>();
  res.name = value["name"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
                     USERVER_NAMESPACE::chaotic::MaxLength<64>>>();
  res.description =
      value["description"]
          .template As<std::optional<
              USERVER_NAMESPACE::chaotic::Primitive<std::string>>>();
  res.changedAt =
      value["changedAt"]
          .template As<std::optional<
              USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>>();
  res.sync = value["sync"]
                 .template As<USERVER_NAMESPACE::chaotic::Primitive<
                     svetit::api::Project::Sync>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, ksvetit__api__Project_PropertiesNames);

  return res;
}

}  // namespace api
}  // namespace svetit

