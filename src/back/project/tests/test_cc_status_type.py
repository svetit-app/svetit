import pytest

endpoint = 'project/cc-status-type'

body = {
	'id': 3,
	'ccTypeId': 1,
	'categoryId': 1,
	'key':'abc123',
	'text': 'Test',
	'inform': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_status_type(service_client):
	"""Cc status type endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ccStatusType1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = ''
	res = await service_client.post(endpoint, json=data)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['text'] = 'Another text'
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.delete(url)
	assert res.status == 200