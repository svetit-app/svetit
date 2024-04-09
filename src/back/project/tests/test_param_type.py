import pytest

endpoint = '/project/param-type'

body_invalid = {
	'id': -1,
	'parentId': 'abc',
	'key': 123,
	'name': True,
	'description': -1,
	'valueType': 'unknown',
	'isDeleted': 123
}

body_valid = {
	'id': 7,
	'parentId': None,
	'key': 'paramType7',
	'name': 'Param Type 7',
	'description': 'Description of Param Type 7',
	'valueType': 'int',
	'isDeleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_param_type(service_client):
	"""Param type endpoint"""

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
	data['description'] = 'Another description for Param Type 7'
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
	url = endpoint + '?id=7'
	res = await service_client.delete(url)
	assert res.status == 200