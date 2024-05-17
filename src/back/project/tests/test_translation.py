import pytest

h = {'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'}
endpoint = '/project/translation'

body = {
	'id': 2,
	'projectId': '11111111-1111-1111-1111-111111111111',
	'lang': 'ru',
	'key': 'abc123',
	'value': 'translated'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_translation(service_client):
	"""Translation endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url, headers=h)
	assert res.status == 200
	assert b'ru' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data, headers=h)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['lang'] = 'en'
	res = await service_client.patch(endpoint, json=data, headers=h)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url, headers=h)
	assert res.status == 200
