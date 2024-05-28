import pytest

endpoint = '/project/measure'

body = {
	'id': 3,
	'projectId': '11111111-1111-1111-1111-111111111111',
	'name': 'M3'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_measure(service_client):
	"""Measure endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'M1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['name'] = 'New M2'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 200