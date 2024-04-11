import pytest

endpoint = '/project/cc-type-param'

body = {
	'ccTypeId': 1,
	'paramId': 5,
	'isDeleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_type_param(service_client):
	"""Cc type param endpoint"""
	"""Get with valid params"""
	url = endpoint + '?ccTypeId=1&paramId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ccTypeId' in res.content

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['isDeleted'] = True
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete with valid params"""
	url = endpoint + '?ccTypeId=1&paramId=5'
	res = await service_client.delete(url)
	assert res.status == 200