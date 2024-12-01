#pragma once

#include "Project_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Project {
  // CppStringWithFormat

  // CppStringWithFormat

  enum class Sync {
    kProjecttonode,
    kNodetoproject,
  };

  static constexpr Sync kSyncValues[] = {
      Sync::kProjecttonode,
      Sync::kNodetoproject,
  };

  std::optional<boost::uuids::uuid> id{};
  boost::uuids::uuid spaceId{};
  std::string key{};
  std::string name{};
  std::optional<std::string> description{};
  std::optional<std::int64_t> changedAt{};
  svetit::api::Project::Sync sync{};
};

bool operator==(const svetit::api::Project& lhs,
                const svetit::api::Project& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Project::Sync& value);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Project& value);

Project::Sync Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync>);

Project Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::Project>);

Project::Sync FromString(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync>);

Project::Sync Parse(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Project::Sync& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Project& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

std::string ToString(svetit::api::Project::Sync value);

}  // namespace api
}  // namespace svetit

