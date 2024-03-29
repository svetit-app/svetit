import pytest


async def test_save_timer_list(service_client):
	"""Discovery request to save_timer list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/save-timer/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/save-timer/list' + '?start=-1&limit=abc')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/save-timer/list' + '?start=0&limit=5')
	assert res.status == 200