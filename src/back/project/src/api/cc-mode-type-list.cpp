#include "cc-mode-type-list.hpp"
#include "../service/service.hpp"

namespace svetit::project::handlers {

formats::json::Value CcModeTypeList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		if (params.HasMember("projectId"))
			return getListByProjectId(params);

		auto table = _s.Repo().Table<model::CcModeType>();
		return getList(table, params);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value CcModeTypeList::getListByProjectId(const formats::json::Value& params) const
{
	auto table = _s.Repo().Table<model::CcModeType>();
	const auto paging = parsePaging(params);

	formats::json::ValueBuilder res;
	res = table->GetListByProjectId(
		params[headers::kSpaceId].As<boost::uuids::uuid>(),
		params["projectId"].As<boost::uuids::uuid>(),
		paging.start, paging.limit);
	return res.ExtractValue();
}

} // namespace svetit::project::handlers
