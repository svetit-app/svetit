#pragma once

#include "Sections_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Section.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Sections {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Section> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Sections& lhs,
                const svetit::api::Sections& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Sections& value);

Sections Parse(USERVER_NAMESPACE::formats::json::Value json,
               USERVER_NAMESPACE::formats::parse::To<svetit::api::Sections>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Sections& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

