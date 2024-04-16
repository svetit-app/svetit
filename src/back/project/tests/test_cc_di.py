import pytest

endpoint = '/project/cc-di'

body = {
	'ccId': 1,
	'diId': 2
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_di(service_client):
	"""Cc di endpoint"""
	"""Get with valid params"""
	url = endpoint + '?ccId=1&diId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ccId' in res.content

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body)
	assert res.status == 201

	"""Delete with valid params"""
	url = endpoint + '?ccId=1&diId=2'
	res = await service_client.delete(url)
	assert res.status == 200