import pytest

endpoint = '/project/translation/list'

async def test_translation_list(service_client):
	"""Translation list endpoint"""

	"""Without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""With invalid params"""
	url = endpoint + '?start=-1&limit=abc'
	res = await service_client.get(url)
	assert res.status == 400

	"""With valid params"""
	url = endpoint + '?start=0&limit=5'
	res = await service_client.get(url)
	assert res.status == 200