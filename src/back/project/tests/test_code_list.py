import pytest

endpoint = '/project/code/list'


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_code_list(service_client):
	"""Code list endpoint"""
	"""With valid params"""
	url = endpoint + (
		'?start=0&limit=5&projectId=11111111-1111-1111-1111-111111111111'
	)
	res = await service_client.get(url)
	assert res.status == 200
	assert b'ff68dd7233213841e710a492c83cba8fe2380a88' in res.content
	assert b'"total":1' in res.content
