import pytest


async def test_translation_list(service_client):
	"""Translation list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/translation/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/translation/list' + '?start=-1&limit=abc')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/translation/list' + '?start=0&limit=5')
	assert res.status == 200