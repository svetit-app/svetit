import pytest


async def test_control_circuit(service_client):
	"""Control circuit endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/control-circuit')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/control-circuit' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/control-circuit' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/control-circuit')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'type_id': 'abc',
		'section_id': 'xyz',
		'name': -1,
		'is_deleted': 123
	}
	res = await service_client.post('/project/control-circuit', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'type_id': 1,
		'section_id': 1,
		'name': 'Test control curcuit',
		'is_deleted': False
	}
	res = await service_client.post('/project/control-circuit', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/control-circuit')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'def',
		'type_id': 'abc',
		'section_id': 'xyz',
		'name': -1,
		'is_deleted': 123
	}
	res = await service_client.patch('/project/control-circuit', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'type_id': 1,
		'section_id': 1,
		'name': 'Test control curcuit',
		'is_deleted': False
	}
	res = await service_client.patch('/project/control-circuit', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/control-circuit')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/control-circuit' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/control-circuit' + '?id=1')
	assert res.status == 200