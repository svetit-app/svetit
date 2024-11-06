#include "userver/formats/json/serialize.hpp"
#include "userver/server/http/http_method.hpp"
#include <memory>
#include <string_view>
#include <svetit/schemas.hpp>

#include <benchmark/benchmark.h>

#include <userver/server/request/request_base.hpp>
#include <userver/utils/impl/transparent_hash.hpp>
#include <userver/utest/utest.hpp>

USERVER_NAMESPACE_BEGIN

namespace engine {
class TaskProcessor;
}  // namespace engine

namespace server {
namespace handlers {
class HttpRequestStatistics;
class HttpHandlerBase;
}  // namespace handlers

namespace http {

class HttpRequestImpl final : public request::RequestBase {
 public:
  ~HttpRequestImpl() override;
  HttpRequestImpl(request::ResponseDataAccounter& data_accounter,
                  engine::io::Sockaddr remote_address);

  const std::string& GetRequestPath() const override;
  bool IsFinal() const override;
  bool IsUpgradeWebsocket() const override;
  void DoUpgrade(std::unique_ptr<engine::io::RwBase>&& socket,
                 engine::io::Sockaddr&& peer_name) const override;
  request::ResponseBase& GetResponse() const override;
  void WriteAccessLogs(const logging::LoggerPtr& logger_access,
                       const logging::LoggerPtr& logger_access_tskv,
                       const std::string& remote_address) const override;
  void SetMatchedPathLength(size_t length) override;
  void AccountResponseTime() override;
  void MarkAsInternalServerError() const override;

  HttpMethod method_{HttpMethod::kUnknown};
  unsigned short http_major_{1};
  unsigned short http_minor_{1};
  std::string url_;
  std::string request_path_;
  std::string request_body_;
  std::string path_suffix_;
  utils::impl::TransparentMap<std::string, std::vector<std::string>,
                              utils::StrCaseHash>
      request_args_;
  utils::impl::TransparentMap<std::string, std::vector<FormDataArg>,
                              utils::StrCaseHash>
      form_data_args_;
  std::vector<std::string> path_args_;
  utils::impl::TransparentMap<std::string, size_t, utils::StrCaseHash>
      path_args_by_name_index_;
  HttpRequest::HeadersMap headers_;
  HttpRequest::CookiesMap cookies_;
  bool is_final_{false};
#ifndef NDEBUG
  mutable bool args_referenced_{false};
#endif

  using UpgradeCallback = std::function<void(
      std::unique_ptr<engine::io::RwBase>&&, engine::io::Sockaddr&&)>;
  UpgradeCallback upgrade_websocket_cb_;

  mutable HttpResponse response_;
  engine::io::Sockaddr remote_address_;
  engine::TaskProcessor* task_processor_{nullptr};
  const handlers::HttpHandlerBase* handler_{nullptr};
  handlers::HttpRequestStatistics* request_statistics_{nullptr};
};

}  // namespace http
}  // namespace server

USERVER_NAMESPACE_END

server::request::ResponseDataAccounter accounter;
std::shared_ptr<server::http::HttpRequestImpl> reqImpl;

const auto req = []() {
	reqImpl = std::make_shared<server::http::HttpRequestImpl>(accounter, engine::io::Sockaddr{});
	reqImpl->url_ = "http:://localhost/test";
	reqImpl->method_ = server::http::HttpMethod::kPatch;
	reqImpl->headers_.emplace(std::string_view{"X-Space-Is-Admin"}, std::string{"1"});
	reqImpl->headers_.emplace(std::string_view{"X-Space-Id"}, "fc6441cf-40e6-460e-ac1f-2b838d0d493f");
	return server::http::HttpRequest{*reqImpl};
}();
const auto reqBody = formats::json::FromString(R"json({
  "name": "test name",
  "spaceId": "fc6441cf-40e6-460e-ac1f-2b838d0d493f",
  "sync": "nodeToProject"
})json");

using namespace svetit;

constexpr const std::string_view reqSchema = R"json(
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "properties": {
    "X-Space-Is-Admin": {
      "in": "header",
      "maxLength": 1,
      "minLength": 1,
      "type": "string"
    },
    "X-Space-Id": {
      "format": "uuid",
      "in": "header",
      "maxLength": 36,
      "minLength": 36,
      "pattern": "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$",
      "type": "string"
    }
  },
  "requestBody": "project.json",
  "required": [
    "X-Space-Id",
    "X-Space-Is-Admin"
  ],
  "type": "object"
})json";

constexpr const std::string_view bodySchema = R"json(
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "properties": {
    "name": {
      "maxLength": 64,
      "minLength": 1,
      "type": "string"
    },
    "spaceId": {
      "format": "uuid",
      "maxLength": 36,
      "minLength": 36,
      "pattern": "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$",
      "type": "string"
    },
    "sync": {
      "enum": [
        "projectToNode",
        "nodeToProject"
      ],
      "type": "string"
    }
  },
  "required": [
    "spaceId",
    "name",
    "sync"
  ],
  "type": "object"
})json";

const auto schemasMap = []() {
	std::map<server::http::HttpMethod, RequestAndJsonSchema> res;
	auto schema = LoadSchema(
		formats::json::FromString(reqSchema),
		formats::json::FromString(bodySchema)
	);
	res.emplace(server::http::HttpMethod::kPatch, std::move(schema));
	return res;
}();

void Simple(benchmark::State& state) {
	for ([[maybe_unused]] auto _ : state) {
		auto json = ValidateRequest(schemasMap, req, reqBody);
		benchmark::DoNotOptimize(json);
	}
}

BENCHMARK(Simple);
