import pytest

endpoint = '/project/device-item'

body_invalid = {
	'id': 'invalid',
	'deviceId': 'abc',
	'typeId': 'xyz',
	'name': 123,
	'isDeleted': 'test'
}

body_valid = {
	'id': 3,
	'deviceId': 1,
	'typeId': 1,
	'name': 'Test',
	'isDeleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_device_item(service_client):
	"""Device item endpoint"""

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
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	data = body_valid.copy()
	data['name'] = 'Another name'
	res = await service_client.patch(endpoint, json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid param"""
	url = endpoint + '?id=abc'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 200