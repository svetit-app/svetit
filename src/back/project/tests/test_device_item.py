import pytest

endpoint = '/project/device-item'

body = {
	'id': 3,
	'deviceId': 1,
	'typeId': 1,
	'name': 'Test',
	'isDeleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_device_item(service_client):
	"""Device item endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'Device Item 1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['name'] = 'Another name'
	res = await service_client.patch(endpoint, json=body)
	assert res.status == 200

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 200