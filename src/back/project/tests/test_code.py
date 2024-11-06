import pytest

endpoint = '/project/code'

body = {
	'id': 2,
	'projectId': '11111111-1111-1111-1111-111111111111',
	'repositoryId': '11111111-1111-1111-1111-111111111111',
	'commitHash': '4572def0053fcba64e4becb1800a1d160502e99f'
}


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_code(service_client):
	"""Code endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ff68dd7233213841e710a492c83cba8fe2380a88' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['commitHash'] = '123'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url)
	assert res.status == 204
