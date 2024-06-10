import pytest

endpoint = '/space/invitation'
url = endpoint + '?start=0&limit=10'

user = b'8ad16a1d-18b1-4aaa-8b0f-f61915974c66'
hUser = {'X-User': user.decode()}


# run by make test-space or make run-space
@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_get_invitations_no_auth(service_client):
	"""No authorization X-User header"""
	res = await service_client.get(url)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_get_invitations_wrong_params(service_client):
	"""Wrong input params"""
	h = {'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'}

	# no start
	res = await service_client.get(endpoint + '?limit=10', headers=h)
	assert res.status == 400

	# no limit
	res = await service_client.get(endpoint + '?start=0', headers=h)
	assert res.status == 400

	# no start & no limit
	res = await service_client.get(endpoint, headers=h)
	assert res.status == 400

	# params are not integers
	res = await service_client.get(endpoint + '?start=a&limit=b', headers=h)
	assert res.status == 400

	# params are negative integers
	res = await service_client.get(endpoint + '?start=-1&limit=2', headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_get_invitations_valid_without_spaceid(service_client):
	"""Valid get invitations without spaceId"""
	# getting invitations with userId=X-User (but X-User is not admin)
	res = await service_client.get(url, headers=hUser)
	assert res.status == 200
	assert user in res.content
	assert b'"total":4' in res.content

	# getting invitations for spaces where X-User is admin
	res = await service_client.get(
		url, headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'},
	)
	assert res.status == 200
	assert user in res.content
	assert b'"total":5' in res.content

	# getting empty list when X-User is admin in space, but no inv exist
	res = await service_client.get(
		url, headers={'X-User': '20d16a1d-18b1-4aaa-8b0f-f61915974c66'},
	)
	assert res.status == 200
	assert b'"list":[]' in res.content
	assert b'"total":0' in res.content

	# getting empty list when X-User has no invitations and not an admin
	res = await service_client.get(
		url, headers={'X-User': '21d16a1d-18b1-4aaa-8b0f-f61915974c66'},
	)
	assert res.status == 200
	assert b'"list":[]' in res.content
	assert b'"total":0' in res.content


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_get_invitations_with_spaceid(service_client):
	"""Get invitations with spaceId"""
	# empty spaceId
	res = await service_client.get(url + '&spaceId=', headers=hUser)
	assert res.status == 400

	# spaceId is not an uuid
	res = await service_client.get(url + '&spaceId=123', headers=hUser)
	assert res.status == 400

	# spaceId is uuid but no such space exists
	res = await service_client.get(
		url + '&spaceId=x0000000-x000-x000-x000-x00000000000',
		headers=hUser,
	)
	assert res.status == 400

	# X-User is not an admin in such space (which exists)
	res = await service_client.get(
		url + '&spaceId=11000000-1100-1100-1100-110000000000',
		headers=hUser,
	)
	assert res.status == 403

	# X-User is admin, but no invitations for such space
	res = await service_client.get(
		url + '&spaceId=10000000-1000-1000-1000-100000000000',
		headers={'X-User': '20d16a1d-18b1-4aaa-8b0f-f61915974c66'},
	)
	assert res.status == 200
	assert b'"list":[]' in res.content
	assert b'"total":0' in res.content

	# X-User is ad admin in space with such spaceId and invitations exist
	res = await service_client.get(
		url + '&spaceId=11111111-1111-1111-1111-111111111111',
		headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'},
	)
	assert res.status == 200
	assert user in res.content
	assert b'"total":5' in res.content
