#pragma once

#include "DiType_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct DiType {
  static constexpr auto kIdMinimum = 0;

  static constexpr auto kMeasureidMinimum = 1;

  static constexpr auto kSavetimeridMinimum = 1;

  enum class Mode {
    kReadonlyflag,
    kReadwriteflag,
    kReadonly,
    kReadwrite,
    kFile,
    kButton,
    kVideostream,
  };

  static constexpr Mode kModeValues[] = {
      Mode::kReadonlyflag, Mode::kReadwriteflag, Mode::kReadonly,
      Mode::kReadwrite,    Mode::kFile,          Mode::kButton,
      Mode::kVideostream,
  };

  enum class Savealgorithm {
    kOff,
    kImmediately,
    kBytimer,
    kBytimerorimmediately,
  };

  static constexpr Savealgorithm kSavealgorithmValues[] = {
      Savealgorithm::kOff,
      Savealgorithm::kImmediately,
      Savealgorithm::kBytimer,
      Savealgorithm::kBytimerorimmediately,
  };

  std::optional<std::int64_t> id{};
  std::int64_t measureId{};
  std::int64_t saveTimerId{};
  std::string key{};
  std::string name{};
  svetit::api::DiType::Mode mode{};
  svetit::api::DiType::Savealgorithm saveAlgorithm{};
};

bool operator==(const svetit::api::DiType& lhs, const svetit::api::DiType& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiType::Mode& value);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiType::Savealgorithm& value);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiType& value);

DiType::Mode Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode>);

DiType::Savealgorithm Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>);

DiType Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType>);

DiType::Mode FromString(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode>);

DiType::Mode Parse(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode>);

DiType::Savealgorithm FromString(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>);

DiType::Savealgorithm Parse(
    std::string_view value,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DiType::Mode& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DiType::Savealgorithm& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DiType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

std::string ToString(svetit::api::DiType::Mode value);

std::string ToString(svetit::api::DiType::Savealgorithm value);

}  // namespace api
}  // namespace svetit

