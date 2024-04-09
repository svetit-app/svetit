import pytest

endpoint = '/project'

body_invalid = {
	'id': 'abc',
	'space_id': 'abc123',
	'key': 123,
	'name': True,
	'description': '',
	'sync': 'invalid_sync'
}

body_valid = {
	'id': '22222222-2222-2222-2222-222222222222',
	'spaceId': '22222222-2222-2222-2222-222222222222',
	'key': 'project2',
	'name': 'Project 2',
	'description': 'Description of Project 2',
	'changedAt': 1,
	'sync': 'project_to_node'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_project(service_client):
	"""Project endpoint"""
	# """Without params"""
	# res = await service_client.get(endpoint)
	# assert res.status == 400

	# """By id with invalid uuid"""
	# url = endpoint + '?id=123'
	# res = await service_client.get(url)
	# assert res.status == 400

	"""By id with valid uuid"""
	url = endpoint + '?id=11111111-1111-1111-1111-111111111111'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'Project 1' in res.content

	# """By key with empty key"""
	# url = endpoint + '?key='
	# res = await service_client.get(url)
	# assert res.status == 400

	"""By key with valid key"""
	url = endpoint + '?key=project1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'11111111-1111-1111-1111-111111111111' in res.content

	# """Post without body"""
	# res = await service_client.post(endpoint)
	# assert res.status == 400

	# """Post with invalid body"""
	# data = body_invalid.copy()
	# data['id'] = ''
	# res = await service_client.post(endpoint, json=data)
	# assert res.status == 400

	"""Post with valid body"""
	data = body_valid.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	# """Patch without body"""
	# res = await service_client.patch(endpoint)
	# assert res.status == 400

	# """Patch with invalid body"""
	# res = await service_client.patch(endpoint, json=body_invalid)
	# assert res.status == 400

	"""Patch with valid body"""
	data = body_valid.copy()
	data['description'] = 'Another description of Project 2'
	data['id'] = '11111111-1111-1111-1111-111111111111'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	# """Delete without param"""
	# res = await service_client.delete(endpoint)
	# assert res.status == 400

	# """Delete with invalid param (invalid uuid)"""
	# url = endpoint + '?id=123'
	# res = await service_client.delete(url)
	# assert res.status == 400

	"""Delete with valid param"""
	url = endpoint + '?id=11111111-1111-1111-1111-111111111111'
	res = await service_client.delete(url)
	assert res.status == 200