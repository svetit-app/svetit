#pragma once

#include "simple_list.hpp"
#include "../model/cc_mode_type_serialize.hpp"

namespace svetit::project {
class Service;
} // namespace svetit::project

namespace svetit::project::handlers {

static constexpr std::string_view kCcModeTypeListName = "handler-cc-mode-type-list";
static constexpr std::string_view kCcModeTypeListFilterName = "ccTypeId";
using CcModeTypeListBase = SimpleList<Service, model::CcModeType, &kCcModeTypeListName, &kCcModeTypeListFilterName>;

class CcModeTypeList final : public CcModeTypeListBase {
public:
	using CcModeTypeListBase::SimpleList;

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

	formats::json::Value getListByProjectId(const formats::json::Value& params) const;
};

} // namespace svetit::project::handlers
