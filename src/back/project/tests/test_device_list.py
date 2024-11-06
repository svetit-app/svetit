import pytest

endpoint = '/project/device/list'


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_device_list(service_client):
	"""Device list endpoint"""
	"""With valid params"""
	url = endpoint
	url += '?start=0&limit=5&projectId=11111111-1111-1111-1111-111111111111'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content
