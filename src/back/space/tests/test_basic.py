import pytest

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_no_auth(service_client):
    response = await service_client.post(
        '/space', json={'id':'', 'name': 'test12345678901', 'key': '67123', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 401

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_valid(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': '67123', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 201

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_no_name(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'key': '67123', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 500

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_no_key(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 500

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_no_requestsAllowed(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': '67123', 'createdAt':0}
    )
    assert response.status == 500

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_key_is_uuid_but_not_as_x_user(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': '00000000-a69a-4aec-b9c0-c80171d3ed86', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_reserved_key_1(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': 'u', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_reserved_key_2(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': 'auth', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_reserved_key_3(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': 'settings', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_reserved_key_4(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': 'main', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_reserved_key_5(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': 'api', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_regex_invalid_key_1(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': '-123d', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_regex_invalid_key_2(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': 'Б123d', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_regex_invalid_key_3(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': '', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_invalid_key_too_long(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': '1234567890123456789012345678901234567', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 500

# need to understand is response status 500 needed
@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_id_not_empty(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'d2f9924d-a69a-4aec-b9c0-c80171d3ed86', 'name': 'test12345678901', 'key': '1234567890', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 500

# need to understand is response status 400 needed
@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_name_is_empty(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': '', 'key': '1234567890', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 400

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_requestsAllowed_is_not_bool(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': '', 'key': '1234567890', 'requestsAllowed' : "False", 'createdAt':0}
    )
    assert response.status == 500

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_create_space_duplicate_key(service_client):
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': '67123', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 201
    response = await service_client.post(
        '/space', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={'id':'', 'name': 'test12345678901', 'key': '67123', 'requestsAllowed' : False, 'createdAt':0}
    )
    assert response.status == 409