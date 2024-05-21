import pytest

endpoint = '/project/value-view'

body = {
	'id': 3,
	'diTypeId': 1,
	'value': 'Test Value',
	'view': 'Test View',
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_value_view(service_client):
	"""Value view endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'Value1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['value'] = 'Another value'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 200