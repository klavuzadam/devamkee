#include "stdafx.h"
#ifdef __FAKE_PLAYER__
#include "FakePlayerManager.h"
#include "ClientManager.h"

void CFakePlayerManager::Initialize()
{
	m_mapFakePlayer.clear();
}

void CFakePlayerManager::GDFakePlayer(CPeer* peer, uint32_t dwHandle, const char* data)
{
	TPacketGDFakePlayer* p = (TPacketGDFakePlayer*)data;
	data += sizeof(TPacketGDFakePlayer);

	switch (p->bSubheader)
	{
		case EFakePlayerGDSubheaders::FAKE_PLAYER_SUBHEADER_GD_LOGIN:
			{
				FakePlayerLogin(peer, dwHandle, (TFakePlayerLoginData*)data);
				return;
			}
		
		case EFakePlayerGDSubheaders::FAKE_PLAYER_SUBHEADER_GD_LOGOUT:
			{
				FakePlayerLogout(peer, dwHandle, (TFakePlayerLogoutData*)data);
				return;
			}
			
		default:
			sys_err("CFakePlayerManager::GDFakePlayer: Unknown subheader %d", p->bSubheader);
			break;
	}
}

void CFakePlayerManager::FakePlayerLogin(CPeer* peer, uint32_t dwHandle, TFakePlayerLoginData* fakePlayerLoginData)
{
	const auto it = m_mapFakePlayer.find(fakePlayerLoginData->szName);
	if (it != m_mapFakePlayer.end())
	{
		TPacketDGFakePlayer packet;
		packet.bSubheader = EFakePlayerDGSubheaders::FAKE_PLAYER_SUBHEADER_DG_ALREADY_EXISTS;
		peer->EncodeHeader(HEADER_DG_FAKE_PLAYER, dwHandle, sizeof(packet));
		peer->Encode(&packet, sizeof(packet));
		return;
	}
	
	m_mapFakePlayer.insert(TFakePlayerMap::value_type(fakePlayerLoginData->szName, fakePlayerLoginData->lMapIndex));
	
	TPacketDGFakePlayer packet;
	packet.bSubheader = EFakePlayerDGSubheaders::FAKE_PLAYER_SUBHEADER_DG_LOGIN;
	
	peer->EncodeHeader(HEADER_DG_FAKE_PLAYER, 0, sizeof(packet) + sizeof(TFakePlayerLoginData));
	peer->Encode(&packet, sizeof(packet));
	peer->Encode(fakePlayerLoginData, sizeof(TFakePlayerLoginData));
}

void CFakePlayerManager::FakePlayerLogout(CPeer* peer, uint32_t dwHandle, TFakePlayerLogoutData* fakePlayerLogoutData)
{
	const auto it = m_mapFakePlayer.find(fakePlayerLogoutData->szName);
	if (it == m_mapFakePlayer.end())
	{
		TPacketDGFakePlayer packet;
		packet.bSubheader = EFakePlayerDGSubheaders::FAKE_PLAYER_SUBHEADER_DG_DONT_EXISTS;
		peer->EncodeHeader(HEADER_DG_FAKE_PLAYER, dwHandle, sizeof(packet));
		peer->Encode(&packet, sizeof(packet));
		return;
	}
	
	TPacketDGFakePlayer packet;
	packet.bSubheader = EFakePlayerDGSubheaders::FAKE_PLAYER_SUBHEADER_DG_LOGOUT;

	peer->EncodeHeader(HEADER_DG_FAKE_PLAYER, 0, sizeof(packet) + sizeof(TFakePlayerLogoutData));
	peer->Encode(&packet, sizeof(packet));
	peer->Encode(fakePlayerLogoutData, sizeof(TFakePlayerLogoutData));
	
	m_mapFakePlayer.erase(it);
}
#endif
