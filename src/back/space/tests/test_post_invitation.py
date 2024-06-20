import pytest

url = '/space/invitation'
json = {
	'spaceId': '11111111-1111-1111-1111-111111111111',
	'userId': '87d16a1d-18b1-4aaa-8b0f-f61915974c66',
	'role': 'user',
	'creatorId': '12d16a1d-18b1-4aaa-8b0f-f61915974c66',
	'createdAt': 0
}
h = {'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'}


# run by make test-space or make run-space
@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_post_invitation_no_auth(service_client):
	"""No authorization X-User header"""
	res = await service_client.post(
		url,
		json=json
	)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_post_invitation_wrong_params(service_client):
	"""Wrong params"""
	# no spaceId
	data = json.copy()
	del data['spaceId']
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400

	# no userId
	data = json.copy()
	del data['userId']
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400

	# no role
	data = json.copy()
	del data['role']
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400

	# no creatorId
	data = json.copy()
	del data['creatorId']
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_post_invitation_invalid_params(service_client):
	"""Wrong params"""
	# empty spaceId
	data = json.copy()
	data['spaceId'] = ''
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400

	# spaceId is not ad valid uuid
	data['spaceId'] = '123'
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400

	# wrong role
	data = json.copy()
	data['role'] = 'administrator'
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400

	# empty userId
	data = json.copy()
	del data['userId']
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400

	# empty creatorId
	data = json.copy()
	data['userId'] = '12d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['creatorId'] = ''
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_post_invitation_valid(service_client):
	"""Valid login of invitation creation"""
	# I want to join to space where requestAllowed=true
	data = json.copy()
	data['userId'] = '20d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['creatorId'] = '20d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 201

	# We want to join some user to space where X-User is admin
	data = json.copy()
	data['userId'] = '88d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['creatorId'] = '01d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.post(
		url, headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json=data)
	assert res.status == 201


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_post_invitation_invalid(service_client):
	"""Valid login of invitation creation"""
	# Unknown space
	data = json.copy()
	data['spaceId'] = '00000000-1111-1111-1111-111111111111'
	data['userId'] = '20d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['creatorId'] = '20d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.post(url, headers=h, json=data)
	assert res.status == 400

	# user is already in space, can't join myself
	data = json.copy()
	data['userId'] = '03d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['creatorId'] = '03d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.post(
		url, headers={'X-User': data['userId']}, json=data)
	assert res.status == 400

	# creator=user, creator is not in space, but requsetSallowed=false
	data['spaceId'] = '12000000-1200-1200-1200-120000000000'
	data['userId'] = '03d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['creatorId'] = '03d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.post(
		url, headers={'X-User': data['userId']}, json=data)
	assert res.status == 400

	# creator<>user, creator is in space, but not an admin
	data = json.copy()
	data['userId'] = '14d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['creatorId'] = '03d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.post(
		url, headers={'X-User': data['creatorId']}, json=data)
	assert res.status == 400
