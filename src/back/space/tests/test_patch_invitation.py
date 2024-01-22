import pytest


# run by make test-space or make run-space
@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_no_auth(service_client):
    """No authorization X-User header"""
    response = await service_client.patch(
        '/space/invitation?id=1',
    )
    assert response.status == 401


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_wrong_param(service_client):
    """Wrong input param"""
    # no id param
    response = await service_client.patch(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 400

    # empty id param
    response = await service_client.patch(
        '/space/invitation?id=',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 400

    # not int id param
    response = await service_client.patch(
        '/space/invitation?id=abc',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 400

    # negative int id param
    response = await service_client.patch(
        '/space/invitation?id=-1',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_not_found_id(service_client):
    """Not found invitation with such id"""
    response = await service_client.patch(
        '/space/invitation?id=20',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 404


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_wrong_role(service_client):
    """Invitation with wrong role"""
    response = await service_client.patch(
        '/space/invitation?id=19',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_invalid_logic_1(service_client):
    """Wants to join, but X-User can't accept himself"""
    response = await service_client.patch(
        '/space/invitation?id=10',
        headers={'X-User': '15000000-0000-0000-0000-000000000000'},
    )
    assert response.status == 403


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_invalid_logic_2(service_client):
    """Wants to join, but X-User is not an admin"""
    response = await service_client.patch(
        '/space/invitation?id=10',
        headers={'X-User': '03d16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 403


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_invalid_logic_3(service_client):
    """We invited/I was invited, but X-User=inv.userId needed"""
    response = await service_client.patch(
        '/space/invitation?id=1',
        headers={'X-User': '03d16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 403


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_valid_1(service_client):
    """I want to join and X-User<>inv.userId and X-User is admin"""
    response = await service_client.patch(
        '/space/invitation?id=8',
        headers={'X-User': '88d16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 200


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_invitation_valid_2(service_client):
    """I was invited and X-User=inv.userId"""
    response = await service_client.patch(
        '/space/invitation?id=1',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
    )
    assert response.status == 200
