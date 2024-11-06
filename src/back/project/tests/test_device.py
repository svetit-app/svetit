import pytest

endpoint = '/project/device'

body = {
	'id': 3,
	'projectId': '11111111-1111-1111-1111-111111111111',
	'pluginId': 1,
	'name': 'Device 3',
	'checkIntervalMsec': 1000
}


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_device(service_client):
	"""Device endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'Device 1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['name'] = 'Another name for Device 2'
	res = await service_client.patch(endpoint, json=body)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 204
