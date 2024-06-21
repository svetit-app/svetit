import pytest

endpoint = '/project'

body = {
	'id': '22222222-2222-2222-2222-222222222222',
	'spaceId': '22222222-2222-2222-2222-222222222222',
	'key': 'project2',
	'name': 'Project 2',
	'description': 'Description of Project 2',
	'changedAt': 1,
	'sync': 'projectToNode'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_project(service_client):
	"""Project endpoint"""
	"""By id with valid uuid"""
	url = endpoint + '?id=11111111-1111-1111-1111-111111111111'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'Project 1' in res.content

	"""By key with valid key"""
	url = endpoint + '?key=project1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'11111111-1111-1111-1111-111111111111' in res.content

	"""Post with valid body"""
	data = body.copy()
	del data['id']
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['description'] = 'Another description of Project 2'
	data['id'] = '11111111-1111-1111-1111-111111111111'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=11111111-1111-1111-1111-111111111111'
	res = await service_client.delete(url)
	assert res.status == 204
