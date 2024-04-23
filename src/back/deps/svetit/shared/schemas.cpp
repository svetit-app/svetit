#include "schemas.hpp"

#include <string>
#include <boost/algorithm/string.hpp>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/fs/blocking/read.hpp>

namespace svetit {

std::string GenerateJsonDocument(
	const formats::json::Value& schemaDocumentParams,
	const server::http::HttpRequest& req
) {
	std::string jsonDocumentStr;
	if (schemaDocumentParams.HasMember("properties")){
		auto properties = schemaDocumentParams["properties"];
		jsonDocumentStr = "{";
		for (auto i = properties.begin(); i != properties.end(); ++i)	{
			auto param = i->GetPath().substr(strlen("properties."));
			if (req.HasArg(param) && !req.GetArg(param).empty()) {
				jsonDocumentStr += "\"" + param + "\"" + ":";
				auto type = (*i)["type"].As<std::string>();
				auto value = req.GetArg(param);
				if (type == "string") {
					jsonDocumentStr += "\"" + value + "\"";
				} else if (type == "number") {
					jsonDocumentStr += value;
				} else if (type == "integer") {
					jsonDocumentStr += value;
				} else if (type == "boolean") {
					jsonDocumentStr += value;
				} else {
					throw std::exception();
				}
				jsonDocumentStr += ",";
			}
		}
		if (jsonDocumentStr.back() == ',')
			jsonDocumentStr.pop_back();
		jsonDocumentStr += "}";
		return jsonDocumentStr;
	}
}

std::string GetBodySchemaFromRequestBody(const std::string& jsonSchemaParams, const std::string& path) {
	const auto jsonSchemaParamsJson = formats::json::FromString(jsonSchemaParams);
	const auto requestBody = jsonSchemaParamsJson["requestBody"];
	const auto requestBodyPath = path + boost::algorithm::to_lower_copy(requestBody.As<std::string>());
	const auto requestBodyFileContents = fs::blocking::ReadFileContents(requestBodyPath);
	return requestBodyFileContents;
}

} // svetit
