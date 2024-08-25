import pytest

from testsuite.databases.pgsql import discover

pytest_plugins = [
	'pytest_userver.plugins.core',
	'pytest_userver.plugins.postgresql'
]


@pytest.fixture
def default_user_id():
	return 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'


@pytest.fixture
def default_space_id():
	return '11111111-1111-1111-1111-111111111111'


@pytest.fixture
def service_client_options(
	service_client_options,
	service_client_default_headers,
	default_user_id,
	default_space_id,
):
	opts = service_client_options
	opts['headers'] = {
		**service_client_default_headers,
		'X-User': default_user_id,
		'X-Space-Id': default_space_id,
		'X-Space-IsAdmin': '1',
	}
	return opts


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
	databases = discover.find_schemas(
		'app', [service_source_dir.joinpath('db')],
	)
	return pgsql_local_create(list(databases.values()))
