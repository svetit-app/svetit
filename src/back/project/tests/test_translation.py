import pytest


async def test_translation(service_client):
	"""Translation endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/translation')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/translation' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/translation' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/translation')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'lang': 123,
		'key': 456,
		'value': 789
	}
	res = await service_client.post('/project/translation', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'lang': 'ru',
		'key': 'abc123',
		'value': 'translated'
	}
	res = await service_client.post('/project/translation', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/translation')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'xyz',
		'project_id': 'abc',
		'lang': 123,
		'key': 456,
		'value': 789
	}
	res = await service_client.patch('/project/translation', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'lang': 'ru',
		'key': 'abc123',
		'value': 'translated'
	}
	res = await service_client.patch('/project/translation', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/translation')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/translation' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/translation' + '?id=1')
	assert res.status == 200
