import pytest

h = {'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'}
json = {
	'id': '',
	'name': 'test12345678901',
	'key': '67123',
	'requestsAllowed': False,
	'createdAt': 0
}


# run by make test-space or make run-space
async def test_create_space_no_auth(service_client):
	"""No authorization X-User header"""
	res = await service_client.post('/space', json=json)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_valid(service_client):
	"""Valid space creation"""
	res = await service_client.post('/space', json=json, headers=h)
	assert res.status == 201


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_no_name(service_client):
	"""No name param in request json"""
	data = json.copy()
	del data['name']
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_no_key(service_client):
	"""No key param in request json"""
	data = json.copy()
	del data['key']
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_no_requests_allowed(service_client):
	"""No requestsAllowed param in request json"""
	data = json.copy()
	del data['requestsAllowed']
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_key_is_uuid_but_not_as_x_user(service_client):
	"""key is uuid but not equals the caller uuid"""
	data = json.copy()
	data['key'] = '00000000-a69a-4aec-b9c0-c80171d3ed86'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_reserved_key_1(service_client):
	"""using reserved keyword key 1"""
	data = json.copy()
	data['key'] = 'u'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_reserved_key_2(service_client):
	"""using reserved keyword key 2"""
	data = json.copy()
	data['key'] = 'auth'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_reserved_key_3(service_client):
	"""using reserved keyword key 3"""
	data = json.copy()
	data['key'] = 'settings'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_reserved_key_4(service_client):
	"""using reserved keyword key 4"""
	data = json.copy()
	data['key'] = 'main'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_reserved_key_5(service_client):
	"""using reserved keyword key 5"""
	data = json.copy()
	data['key'] = 'api'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_regex_invalid_key_1(service_client):
	"""using invalid by regex key 1"""
	data = json.copy()
	data['key'] = '-123d'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_regex_invalid_key_2(service_client):
	"""using invalid by regex key 2"""
	data = json.copy()
	data['key'] = 'Б123d'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_regex_invalid_key_3(service_client):
	"""using invalid by regex key 3"""
	data = json.copy()
	data['key'] = ''
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_invalid_key_too_long(service_client):
	"""using too long key"""
	data = json.copy()
	data['key'] = '1234567890123456789012345678901234567'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 500


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_name_is_empty(service_client):
	"""using empty name"""
	data = json.copy()
	data['name'] = None
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_requests_allowed_is_not_bool(service_client):
	"""requestsAllowed is not bool in request json"""
	data = json.copy()
	data['requestsAllowed'] = 'False'
	res = await service_client.post('/space', json=data, headers=h)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_create_space_duplicate_key(service_client):
	"""using duplicate key"""
	# creating new space
	res = await service_client.post('/space', json=json, headers=h)
	assert res.status == 201

	# then trying to create one more with same key
	res = await service_client.post('/space', json=json, headers=h)
	assert res.status == 409
