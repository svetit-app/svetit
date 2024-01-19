import pytest


# run by make test-space or make run-space
@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_get_invitations_no_auth(service_client):
    """No authorization X-User header"""
    response = await service_client.get(
        '/space/invitation?start=0&limit=10',
    )
    assert response.status == 401


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_get_invitations_wrong_params(service_client):
    """Wrong input params"""
	# no start
    response = await service_client.get(
        '/space/invitation?limit=10',
        headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
    )
    assert response.status == 400

    # no limit
    response = await service_client.get(
        '/space/invitation?start=0',
        headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
    )
    assert response.status == 400

    # no start & no limit
    response = await service_client.get(
        '/space/invitation',
        headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
    )
    assert response.status == 400

    # params are not integers
    response = await service_client.get(
        '/space/invitation?start=a&limit=b',
        headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
    )
    assert response.status == 400

    # params are negative integers
    response = await service_client.get(
        '/space/invitation?start=-1&limit=2',
        headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_get_invitations_valid_without_spaceid(service_client):
    """Valid get invitations without spaceId"""
    # getting invitations with userId=X-User (but X-User is not an admin anywhere)
    response = await service_client.get(
        '/space/invitation?start=0&limit=10',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 200
    assert b'8ad16a1d-18b1-4aaa-8b0f-f61915974c66' in response.content
    assert b'"total":4' in response.content

    # getting invitations for spaces where X-User is admin
    response = await service_client.get(
        '/space/invitation?start=0&limit=10',
        headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 200
    assert b'8ad16a1d-18b1-4aaa-8b0f-f61915974c66' in response.content
    assert b'"total":5' in response.content

    # getting empty list when X-User is admin in space, but no invitations exist
    response = await service_client.get(
        '/space/invitation?start=0&limit=10',
        headers={'X-User': '20d16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 200
    assert b'"list":[]' in response.content
    assert b'"total":0' in response.content

	# getting empty list when X-User has no invitations anywhere and not an admin anywhere
    response = await service_client.get(
        '/space/invitation?start=0&limit=10',
        headers={'X-User': '21d16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 200
    assert b'"list":[]' in response.content
    assert b'"total":0' in response.content

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_get_invitations_with_spaceid(service_client):
    """Get invitations with spaceId"""
    # empty spaceId
    response = await service_client.get(
        '/space/invitation?start=0&limit=10&spaceId=',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 400

    # spaceId is not an uuid
    response = await service_client.get(
        '/space/invitation?start=0&limit=10&spaceId=123',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 400

    # spaceId is uuid but no such space exists
    response = await service_client.get(
        '/space/invitation?start=0&limit=10&spaceId=x0000000-x000-x000-x000-x00000000000',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 400

    # X-User is not an admin in such space (which exists)
    response = await service_client.get(
        '/space/invitation?start=0&limit=10&spaceId=11000000-1100-1100-1100-110000000000',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 403

    # X-User is an admin in space with spaceId, but no invitations for such space
    response = await service_client.get(
        '/space/invitation?start=0&limit=10&spaceId=10000000-1000-1000-1000-100000000000',
        headers={'X-User': '20d16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 200
    assert b'"list":[]' in response.content
    assert b'"total":0' in response.content

    # X-User is ad admin in space with such spaceId and invitations exist
    response = await service_client.get(
        '/space/invitation?start=0&limit=10&spaceId=11111111-1111-1111-1111-111111111111',
        headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 200
    assert b'8ad16a1d-18b1-4aaa-8b0f-f61915974c66' in response.content
    assert b'"total":5' in response.content
