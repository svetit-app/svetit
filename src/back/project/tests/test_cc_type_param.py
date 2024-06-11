import pytest

endpoint = '/project/cc-type-param'

body = {
	'ccTypeId': 1,
	'paramId': 5
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_type_param(service_client):
	"""Cc type param endpoint"""
	"""Get with valid params"""
	url = endpoint + '?ccTypeId=1&paramId=3'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ccTypeId' in res.content

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body)
	assert res.status == 201

	"""Delete with valid params"""
	url = endpoint + '?ccTypeId=1&paramId=5'
	res = await service_client.delete(url)
	assert res.status == 204