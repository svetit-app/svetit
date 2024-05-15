import pytest

h = {'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'}
endpoint = '/project/param-type'

body = {
	'id': 7,
	'parentId': None,
	'key': 'paramType7',
	'name': 'Param Type 7',
	'description': 'Description of Param Type 7',
	'valueType': 'int'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_param_type(service_client):
	"""Param type endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url, headers=h)
	assert res.status == 200
	assert b'paramType1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data, headers=h)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['description'] = 'Another description for Param Type 7'
	res = await service_client.patch(endpoint, json=data, headers=h)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=7'
	res = await service_client.delete(url, headers=h)
	assert res.status == 200