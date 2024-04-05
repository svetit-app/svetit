import pytest

endpoint = '/project/save-timer/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_save_timer_list(service_client):
	"""Discovery request to save_timer list endpoint"""

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