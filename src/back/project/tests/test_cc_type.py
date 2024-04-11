import pytest

endpoint = '/project/cc-type'

body = {
	'id': 3,
	'projectId': '11111111-1111-1111-1111-111111111111',
	'key': 'ccType3',
	'name': 'CC Type 3',
	'description': 'Description for CC Type 3'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_type(service_client):
	"""Cc type endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ccType1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['description'] = 'Another description for CC Type 2'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 200