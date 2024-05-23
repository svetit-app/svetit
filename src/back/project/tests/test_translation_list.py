import pytest

endpoint = '/project/translation/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_translation_list(service_client):
	"""Translation list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5&projectId=11111111-1111-1111-1111-111111111111'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ru' in res.content
	assert b'"total":1' in res.content