import pytest

endpoint = '/project'

async def test_project(service_client):
	"""Project endpoint"""
	"""Without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""By id with invalid uuid"""
	url = endpoint + '?id=123'
	res = await service_client.get(url)
	assert res.status == 400

	"""By id with valid uuid"""
	url = endpoint + '?id=11111111-1111-1111-1111-111111111111'
	res = await service_client.get(url)
	assert res.status == 200

	"""By key with empty key"""
	url = endpoint + '?key='
	res = await service_client.get(url)
	assert res.status == 400

	"""By key with valid key"""
	url = endpoint + '?key=project123'
	res = await service_client.get(url)
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'space_id': 'abc123',
		'key': 123,
		'name': True,
		'description': '',
		'sync': 'invalid_sync'
	}
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'space_id': '33333333-3333-3333-3333-333333333333',
		'key': 'test',
		'name': 'Test project',
		'description': 'Text',
		'sync': 'project_to_node'
	}
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'abc',
		'space_id': 'abc123',
		'key': 123,
		'name': True,
		'description': '',
		'sync': 'invalid_sync'
	}
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': '11111111-1111-1111-1111-111111111111',
		'space_id': '33333333-3333-3333-3333-333333333333',
		'key': 'test',
		'name': 'Test project',
		'description': 'Text',
		'sync': 'project_to_node'
	}
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid param (invalid uuid)"""
	url = endpoint + '?id=123'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid param"""
	url = endpoint + '?id=11111111-1111-1111-1111-111111111111'
	res = await service_client.delete(url)
	assert res.status == 200