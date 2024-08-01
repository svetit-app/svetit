#include "repo/repository.hpp"
#include "service/service.hpp"

#include "api/node.hpp"
#include "api/list.hpp"
#include "api/node-project.hpp"
#include "api/node-project-list.hpp"
#include "api/node-group.hpp"
#include "api/node-group-list.hpp"
#include "api/group-user.hpp"
#include "api/group-user-list.hpp"

#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/server_monitor.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

using namespace svetit::node;

int main(int argc, char* argv[]) {
	auto component_list = components::MinimalServerComponentList()
		.Append<server::handlers::Ping>()
		.Append<components::TestsuiteSupport>()
		.Append<components::HttpClient>()
		.Append<components::Postgres>("database")
		.Append<clients::dns::Component>()
		.Append<server::handlers::TestsControl>()
		.Append<server::handlers::ServerMonitor>()
		.Append<Repository>()
		.Append<Service>()
		.Append<handlers::Node>()
		.Append<handlers::List>()
		.Append<handlers::NodeProject>()
		.Append<handlers::NodeProjectList>()
		.Append<handlers::NodeGroup>()
		.Append<handlers::NodeGroupList>()
		.Append<handlers::GroupUser>()
		.Append<handlers::GroupUserList>()
		;

	return utils::DaemonMain(argc, argv, component_list);
}
