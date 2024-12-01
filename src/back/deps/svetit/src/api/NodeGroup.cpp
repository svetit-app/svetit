#include "NodeGroup.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "NodeGroup_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::NodeGroup& lhs,
                const svetit::api::NodeGroup& rhs) {
  return lhs.nodeId == rhs.nodeId && lhs.groupId == rhs.groupId && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::NodeGroup& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

NodeGroup Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeGroup> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::NodeGroup& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  vb["nodeId"] = USERVER_NAMESPACE::chaotic::WithType<
      USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
      value.nodeId};

  vb["groupId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::NodeGroup::kGroupidMinimum>>{
      value.groupId};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

