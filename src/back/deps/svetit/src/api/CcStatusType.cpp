#include "CcStatusType.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "CcStatusType_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::CcStatusType& lhs,
                const svetit::api::CcStatusType& rhs) {
  return lhs.id == rhs.id && lhs.ccTypeId == rhs.ccTypeId &&
         lhs.categoryId == rhs.categoryId && lhs.key == rhs.key &&
         lhs.text == rhs.text && lhs.inform == rhs.inform && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcStatusType& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

CcStatusType Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusType> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::CcStatusType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::CcStatusType::kIdMinimum>>{*value.id};
  }

  vb["ccTypeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::CcStatusType::kCctypeidMinimum>>{
      value.ccTypeId};

  vb["categoryId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::CcStatusType::kCategoryidMinimum>>{
      value.categoryId};

  vb["key"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<64>>{value.key};

  vb["text"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>,
      USERVER_NAMESPACE::chaotic::MaxLength<512>>{value.text};

  vb["inform"] = USERVER_NAMESPACE::chaotic::Primitive<bool>{value.inform};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

