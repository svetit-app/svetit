import pytest

endpoint = '/project/section'

body = {
	'id': 3,
	'projectId': '11111111-1111-1111-1111-111111111111',
	'name': 'Section 3'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_section(service_client):
	"""Section endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'Section 1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['name'] = 'New name for Section 2'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 200