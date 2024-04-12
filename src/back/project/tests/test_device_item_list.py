import pytest

endpoint = '/project/device-item/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_device_item_list(service_client):
	"""Device item list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content