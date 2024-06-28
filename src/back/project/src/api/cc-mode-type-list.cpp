#include "cc-mode-type-list.hpp"

namespace svetit::project::handlers {

formats::json::Value CcModeTypeList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		if (params.HasMember("projectId"))
			return getListByProjectId(params);

		return getList(std::move(params));
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value CcModeTypeList::getListByProjectId(const formats::json::Value& params) const
{
	const auto paging = parsePaging(params);

	formats::json::ValueBuilder res;
	res = _table->GetListByProjectId(
		paging.start, paging.limit,
		params[headers::kSpaceId].As<boost::uuids::uuid>(),
		params["projectId"].As<boost::uuids::uuid>());
	return res.ExtractValue();
}

} // namespace svetit::project::handlers
