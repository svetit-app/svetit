import pytest


async def test_control_circuit_list(service_client):
	"""Control circuit list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/control-circuit/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/control-circuit/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/control-circuit/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200
