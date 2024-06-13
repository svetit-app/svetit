import pytest

endpoint = '/project/cc-param'

body = {
	'ccId': 1,
	'paramId': 3
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_param(service_client):
	"""Cc param endpoint"""
	"""Get with valid params"""
	url = endpoint + '?ccId=1&paramId=2'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ccId' in res.content

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body)
	assert res.status == 201

	"""Delete with valid params"""
	url = endpoint + '?ccId=1&paramId=3'
	res = await service_client.delete(url)
	assert res.status == 204