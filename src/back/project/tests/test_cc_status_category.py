import pytest

endpoint = '/project/cc-status-category'

body_invalid = {
	'id': 'abc',
	'projectId': 123,
	'key': 456,
	'name': 789,
	'color': 111,
	'isDeleted': 'test'
}

body_valid = {
	'id': 2,
	'projectId': '11111111-1111-1111-1111-111111111111',
	'key': 'abc123',
	'name': 'Test',
	'color': 'white',
	'isDeleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_status_category(service_client):
	"""Cc status category endpoint"""

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
	assert b'ccStatusCategory1' in res.content

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	data = body_invalid.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 400

	"""Post with valid body"""
	data = body_valid.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	data = body_valid.copy()
	data['name'] = 'Another name'
	res = await service_client.patch(endpoint, json=body_valid)
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