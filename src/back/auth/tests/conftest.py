import pytest

from testsuite.databases.pgsql import discover

pytest_plugins = [
	'pytest_userver.plugins.core',
	'pytest_userver.plugins.postgresql'
]

USERVER_CONFIG_HOOKS = ['config_oidc_url']


@pytest.fixture(scope='session')
def config_oidc_url(mockserver_info):
	def do_patch(config_yaml, config_vars):
		config_vars['oidc-provider-url'] = mockserver_info.url('realm')

	return do_patch


@pytest.fixture(autouse=True)
def mock_oidc_configuration(mockserver, mockserver_info):
	@mockserver.json_handler('/realm/.well-known/openid-configuration')
	def mock(request):
		return {
			'issuer': mockserver_info.url(''),
			'authorization_endpoint': mockserver_info.url('realm/auth'),
			'end_session_endpoint': mockserver_info.url('realm/logout'),
			'token_endpoint': mockserver_info.url('realm/token'),
			'jwks_uri': mockserver_info.url('realm/jwks'),
			'userinfo_endpoint': mockserver_info.url('realm/userinfo'),
			'id_token_signing_alg_values_supported': ['RS256'],
		}

	return mock


@pytest.fixture(autouse=True)
def mock_oidc_jwks(mockserver):
	key = {
		'kid': "7imKszf0kA1YgJPHlLHoVoukM3kJnNxOWbta_vJaSQE",
		"kty": "RSA",
		"alg": "RS256",
		"use": "sig",
		"n": (
			"riEG6X_lA_uRTemxe5lpkOtEEh1ahaShrElmB_xi3lT606OrCP0EM1CxI6ZRQ_"
			"x6XaGGlr0WGDSuSvucDwpviou6X66WgoKq72v25QkbmICk7mOTsI4F4qUfbOcY"
			"cSOzoU3YNrvCBi4Ezh71QTsYCiu066avYmFUxk_4EDL7oE1XsbHiK1Kale_hxK"
			"WUfcjxijxZA0p0e-N4T-ZHZG7GuFveqqBZjHrk1B9alB3ZwfJLDOEh7ff7rP2a"
			"lYkWvZFWiBy500pBNqPPZ8X1w32-fVitOvGmomkvVQsxpXUZsn_P1J81rdOdZT"
			"HKrAVQ9FtElfBu-JKzP37F6jMMtpBs1Q"
		),
		"e": "AQAB",
		"x5c": [(
			"MIIClTCCAX0CBgGTAwWzUDANBgkqhkiG9w0BAQsFADAOMQwwCgYDVQQDDANhc2"
			"QwHhcNMjQxMTA2MTk0NzA1WhcNMzQxMTA2MTk0ODQ1WjAOMQwwCgYDVQQDDANh"
			"c2QwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCuIQbpf+UD+5FN6b"
			"F7mWmQ60QSHVqFpKGsSWYH/GLeVPrTo6sI/QQzULEjplFD/HpdoYaWvRYYNK5K"
			"+5wPCm+Ki7pfrpaCgqrva/blCRuYgKTuY5OwjgXipR9s5xhxI7OhTdg2u8IGLg"
			"TOHvVBOxgKK7Trpq9iYVTGT/gQMvugTVexseIrUpqV7+HEpZR9yPGKPFkDSnR7"
			"43hP5kdkbsa4W96qoFmMeuTUH1qUHdnB8ksM4SHt9/us/ZqViRa9kVaIHLnTSk"
			"E2o89nxfXDfb59WK068aaiaS9VCzGldRmyf8/UnzWt051lMcqsBVD0W0SV8G74"
			"krM/fsXqMwy2kGzVAgMBAAEwDQYJKoZIhvcNAQELBQADggEBAARiWB9YnoD1li"
			"eLyuJhpPXaYH2PCzVAWPbvgpL8yH5/WdnVNEB4MzYTM60/unWl8RQBVUr7jYA1"
			"5AS9xt/hm2OhqJnCF07VsLpeirk0V7OnE/QTYt3wbuQyEV1NbR+UlZpEcA2tp6"
			"T8SMrwA7JbhJCDB3HC5ML32A7EX+tqyCYYDLVXtfVwrmJUhprnt/IcGtgdOghl"
			"8CyuoLtpmrOkDSlBtHfwOooEqw+hGaKJmKCnK8SNHVtvE0sKeeXCww9SBoKb1n"
			"CWLYwqXG4g59HFnxHUiLGgSZjButYp9sIpDG6nbB1yiCFauGJKThg1e8Upro1o"
			"ZSPlgZtdQBO9HKRD5s0="
		)],
		"x5t": "HL3N7PhwQQ5IrV_onKrdDxuEl-c",
		"x5t#S256": "fSR9fC47ERv2unDEgFHhVB51KJwgVYoIzoE1ue1PDvw"
	}

	@mockserver.json_handler('/realm/jwks')
	def mock(request):
		return {
			'keys': [key],
		}

	return mock


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
	databases = discover.find_schemas(
		'app', [service_source_dir.joinpath('db')],
	)
	return pgsql_local_create(list(databases.values()))


# Allow call Keycloak endpoints
@pytest.fixture(scope="session")
def allowed_url_prefixes_extra():
	return ["http://localhost:8081/"]
