import pytest

endpoint = '/project/control-circuit'

body = {
	'id': 3,
	'typeId': 1,
	'sectionId': 1,
	'name': 'Control Circuit 3'
}


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_control_circuit(service_client):
	"""Control circuit endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'Control Circuit 1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['name'] = 'Another name for Control Circuit 2'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 204
