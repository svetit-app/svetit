import pytest

endpoint = '/project/details'

# run by make test-project or make run-project
async def test_details(service_client):
	"""Details endpoint"""
	"""Without params in body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Invalid uuids"""
	project_uuids_invalid = [
		'abc123',
		123
	]
	res = await service_client.post(endpoint, json=project_uuids_invalid)
	assert res.status == 400

	"""Valid uuids"""
	project_uuids = [
		'11111111-1111-1111-1111-111111111111',
		'22222222-2222-2222-2222-222222222222'
	]
	res = await service_client.post(endpoint, json=project_uuids)
	assert res.status == 200