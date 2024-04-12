import pytest

endpoint = '/project/details'

project_uuids = [
	'11111111-1111-1111-1111-111111111111',
	'22222222-2222-2222-2222-222222222222'
]

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_details(service_client):
	"""Details endpoint"""
	"""Valid uuids"""
	res = await service_client.post(endpoint, json=project_uuids)
	assert res.status == 200