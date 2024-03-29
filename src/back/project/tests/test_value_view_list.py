import pytest


async def test_value_view_list(service_client):
	"""Value view list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/value-view/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/value-view/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/value-view/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200