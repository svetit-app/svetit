#pragma once

#include "SectionParam_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct SectionParam {
  static constexpr auto kSectionidMinimum = 0;

  static constexpr auto kParamidMinimum = 0;

  std::int64_t sectionId{};
  std::int64_t paramId{};
};

bool operator==(const svetit::api::SectionParam& lhs,
                const svetit::api::SectionParam& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::SectionParam& value);

SectionParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SectionParam>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::SectionParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

