#include "node-group.hpp"
#include "../model/node_group_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>

namespace svetit::node::handlers {

NodeGroup::NodeGroup(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value NodeGroup::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return Get(res, params);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res, params);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res, params);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value NodeGroup::Get(
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();
	const auto nodeId = params["nodeId"].As<boost::uuids::uuid>();
	const auto groupId = params["groupId"].As<int>();
	res = _s.GetNodeGroup(nodeId, groupId, userId);
	return res.ExtractValue();
}

formats::json::Value NodeGroup::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();

	const auto nodeId = params["nodeId"].As<boost::uuids::uuid>();
	const auto groupId = params["groupId"].As<int>();

	_s.DeleteNodeGroup(nodeId, groupId, userId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

formats::json::Value NodeGroup::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();

	auto nodeGroup = body.As<model::NodeGroup>();

	_s.CreateNodeGroup(nodeGroup, userId);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

} // namespace svetit::node::handlers
