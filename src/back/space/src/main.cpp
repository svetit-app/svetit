#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "repo/repository.hpp"
#include "service/service.hpp"
#include "service/tokens.hpp"

#include "api/info.hpp"
#include "api/invitation.hpp"
#include "api/link.hpp"
#include "api/list-available.hpp"
#include "api/list.hpp"
#include "api/space.hpp"
#include "api/user-list.hpp"
#include "api/user-manage.hpp"
#include "api/introspect.hpp"

using namespace svetit::space;

int main(int argc, char* argv[]) {
	auto component_list = components::MinimalServerComponentList()
		.Append<server::handlers::Ping>()
		.Append<components::TestsuiteSupport>()
		.Append<components::HttpClient>()
		.Append<components::Postgres>("database")
		.Append<clients::dns::Component>()
		.Append<server::handlers::TestsControl>()

		.Append<Repository>()
		.Append<Service>()
		.Append<tokens::Tokens>()
		.Append<handlers::Info>()
		.Append<handlers::Invitation>()
		.Append<handlers::Link>()
		.Append<handlers::ListAvailable>()
		.Append<handlers::List>()
		.Append<handlers::Space>()
		.Append<handlers::UserList>()
		.Append<handlers::UserManage>()
		.Append<handlers::Introspect>()
		;

	return utils::DaemonMain(argc, argv, component_list);
}