import pytest

endpoint = '/project/save-timer'

body = {
	'id': 2,
	'projectId': '11111111-1111-1111-1111-111111111111',
	'intervalMsec': 2000
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_save_timer(service_client):
	"""Save timer endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'1000' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['intervalMsec'] = 500
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 204