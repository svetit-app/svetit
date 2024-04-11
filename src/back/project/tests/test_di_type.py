import pytest

endpoint = '/project/di-type'

body = {
	'id': 3,
	'measureId': 1,
	'saveTimerId': 1,
	'key': 'abc123',
	'name': 'Test',
	'mode': 'readonlyFlag',
	'saveAlgorithm': 'off'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_di_type(service_client):
	"""Di type endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'diType1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['name'] = 'Another Name'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete with valid param"""
	url = endpoint + '?id=3'
	res = await service_client.delete(url)
	assert res.status == 200