import pytest

endpoint = '/project/plugin'

body_invalid = {
	'id': 'xyz',
	'project_id': 'abc',
	'name': 123,
	'description': False,
	'key': 0,
	'is_deleted': 5
}

body_valid = {
	'id': 2,
	'project_id': '11111111-1111-1111-1111-111111111111',
	'name': 'Plugin 2',
	'description': 'Description for Plugin 2',
	'key': 'plugin2',
	'is_deleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_plugin(service_client):
	"""Plugin endpoint"""

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
	del data['id']
	res = await service_client.post(endpoint, json=data)
	assert res.status == 400

	"""Post with valid body"""
	data = body_valid.copy()
	del data['id']
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
	data['description'] = 'Another description for Plugin 2'
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
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 200