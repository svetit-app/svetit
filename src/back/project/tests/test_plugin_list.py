import pytest


async def test_plugin_list(service_client):
	"""Plugin list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/plugin/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/plugin/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/plugin/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200