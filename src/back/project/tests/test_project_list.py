import pytest

endpoint = '/project/list'

id = b'11111111-1111-1111-1111-111111111111'


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_project_list(service_client):
	"""Project list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5'
	res = await service_client.get(url)
	assert res.status == 200
	assert id in res.content
	assert b'"total":1' in res.content
