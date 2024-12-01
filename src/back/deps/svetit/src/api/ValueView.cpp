#include "ValueView.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "ValueView_parsers.ipp"

namespace svetit {
namespace api {

bool operator==(const svetit::api::ValueView& lhs,
                const svetit::api::ValueView& rhs) {
  return lhs.id == rhs.id && lhs.diTypeId == rhs.diTypeId &&
         lhs.value == rhs.value && lhs.view == rhs.view && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ValueView& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

ValueView Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ValueView> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const svetit::api::ValueView& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.id) {
    vb["id"] = USERVER_NAMESPACE::chaotic::Primitive<
        std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                          svetit::api::ValueView::kIdMinimum>>{*value.id};
  }

  vb["diTypeId"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<
                        svetit::api::ValueView::kDitypeidMinimum>>{
      value.diTypeId};

  vb["value"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.value};

  vb["view"] = USERVER_NAMESPACE::chaotic::Primitive<
      std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{value.view};

  return vb.ExtractValue();
}

}  // namespace api
}  // namespace svetit

