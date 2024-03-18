import pytest

pytest_plugins = ['pytest_userver.plugins.core']


@pytest.fixture(scope="session")
def allowed_url_prefixes_extra():
	return ["http://localhost:8081/"]
