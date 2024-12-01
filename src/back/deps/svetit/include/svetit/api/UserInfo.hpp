#pragma once

#include "UserInfo_fwd.hpp"

#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct UserInfo {
  // CppStringWithFormat

  std::optional<boost::uuids::uuid> id{};
  std::string displayName{};
  std::string login{};
  std::string firstname{};
  std::string lastname{};
  std::string email{};
};

bool operator==(const svetit::api::UserInfo& lhs,
                const svetit::api::UserInfo& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::UserInfo& value);

UserInfo Parse(USERVER_NAMESPACE::formats::json::Value json,
               USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfo>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::UserInfo& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

