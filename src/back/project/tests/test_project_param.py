import pytest

endpoint = '/project/project-param'

body_invalid = {
	'projectId': 'abc',
	'paramId': -1,
	'isDeleted': 'test'
}

body_valid = {
	'projectId': '11111111-1111-1111-1111-111111111111',
	'paramId': 3,
	'isDeleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_project_param(service_client):
	"""Project param endpoint"""
	# """Get without params"""
	# res = await service_client.get(endpoint)
	# assert res.status == 400

	# """Get with invalid params"""
	# url = endpoint + '?projectId=abc&paramId=xyz'
	# res = await service_client.get(url)
	# assert res.status == 400

	"""Get with valid params"""
	url = endpoint + '?projectId=11111111-1111-1111-1111-111111111111&paramId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'projectId' in res.content

	# """Post without body"""
	# res = await service_client.post(endpoint)
	# assert res.status == 400

	# """Post with invalid body"""
	# res = await service_client.post(endpoint, json=body_invalid)
	# assert res.status == 400

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 201

	# """Patch without body"""
	# res = await service_client.patch(endpoint)
	# assert res.status == 400

	# """Patch with invalid body"""
	# res = await service_client.patch(endpoint, json=body_invalid)
	# assert res.status == 400

	"""Patch with valid body"""
	data = body_valid.copy()
	data['isDeleted'] = True
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	# """Delete without params"""
	# res = await service_client.delete(endpoint)
	# assert res.status == 400

	# """Delete with invalid params"""
	# url = endpoint + '?projectId=abc&paramId=xyz'
	# res = await service_client.delete(url)
	# assert res.status == 400

	"""Delete with valid params"""
	url = endpoint + '?projectId=11111111-1111-1111-1111-111111111111&paramId=1'
	res = await service_client.delete(url)
	assert res.status == 200