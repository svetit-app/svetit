import pytest

endpoint = '/project/di-type'

body_invalid = {
	'id': -1,
	'measureId': 'abc',
	'saveTimerId': 'xyz',
	'key': 123,
	'name': 456,
	'mode': 'invalid',
	'saveAlgorithm': 'invalid',
	'isDeleted': 5
}

body_valid = {
	'id': 3,
	'measureId': 1,
	'saveTimerId': 1,
	'key': 'abc123',
	'name': 'Test',
	'mode': 'readonly_flag',
	'saveAlgorithm': 'off',
	'isDeleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_di_type(service_client):
	"""Di type endpoint"""

	"""Get without param"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""Get with invalid param"""
	url = endpoint + '?id=abc'
	res = await service_client.get(url)
	assert res.status == 400

	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'diType1' in res.content

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	data = body_invalid.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 400

	"""Post with valid body"""
	data = body_valid.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	data = body_valid.copy()
	data['name'] = 'Another Name'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid param"""
	url = endpoint + '?id=abc'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid param"""
	url = endpoint + '?id=3'
	res = await service_client.delete(url)
	assert res.status == 200