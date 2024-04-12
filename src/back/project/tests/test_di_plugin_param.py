import pytest

endpoint = '/project/di-plugin-param'

body = {
	'diTypeId': 1,
	'paramId': 6
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_di_plugin_param(service_client):
	"""Di plugin param endpoint"""
	"""Get with valid params"""
	url = endpoint + '?diTypeId=1&paramId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'diTypeId' in res.content

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body)
	assert res.status == 201

	"""Delete with valid params"""
	url = endpoint + '?diTypeId=1&paramId=6'
	res = await service_client.delete(url)
	assert res.status == 200