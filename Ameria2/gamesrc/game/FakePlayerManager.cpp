#include "stdafx.h"

#ifdef __FAKE_PLAYER__
#include "FakePlayerManager.h"
#include "char.h"
#include "char_manager.h"
#include "sectree.h"
#include "sectree_manager.h"
#include "item.h"
#include "item_manager.h"
#include "desc.h"
#include "desc_manager.h"
#include "desc_client.h"
#include "buffer_manager.h"
#include "db.h"
#include <boost/lexical_cast.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFakePlayer
CFakePlayer::CFakePlayer(TFakePlayerLoginData* fakePlayerLoginData) 
{
	Login(fakePlayerLoginData);
}

CFakePlayer::~CFakePlayer()
{
	if (m_pFakePlayer)
		M2_DESTROY_CHARACTER(m_pFakePlayer);
}

void CFakePlayer::Login(TFakePlayerLoginData* fakePlayerLoginData)
{
	long lMapIndex = fakePlayerLoginData->lMapIndex;
	long lX = fakePlayerLoginData->lX; 
	long lY = fakePlayerLoginData->lY;
	
	if (LPSECTREE sectree = SECTREE_MANAGER::instance().Get(lMapIndex, lX, lY); !sectree)
	{
		CFakePlayerManager::Instance().FakePlayerLogout(fakePlayerLoginData->szName);	
		return;
	}
	
	LPCHARACTER pFakePlayer = CHARACTER_MANAGER::instance().CreateCharacter("Reinhardt"); 
	if (nullptr == pFakePlayer)
	{
		CFakePlayerManager::Instance().FakePlayerLogout(fakePlayerLoginData->szName);	
		return;
	}
	
	pFakePlayer->SetCharType(ECharType::CHAR_TYPE_PC);
	pFakePlayer->SetRotation(fakePlayerLoginData->iRotation);
	pFakePlayer->SetXYZ(lX, lY, 0);
	pFakePlayer->SetMapIndex(lMapIndex);
	pFakePlayer->SetName(fakePlayerLoginData->szName);
	pFakePlayer->SetFakePlayer(true);
	m_pFakePlayer = pFakePlayer;
	
	if (!LoadFakePlayer())
	{
		sys_err("<FakePlayer> cant Load FakePlayer %s.", fakePlayerLoginData->szName);
		CFakePlayerManager::Instance().FakePlayerLogout(fakePlayerLoginData->szName);
	}
	
	if (!pFakePlayer->Show(lMapIndex, lX, lY, 0))
		CFakePlayerManager::Instance().FakePlayerLogout(fakePlayerLoginData->szName);	
	
	// pFakePlayer->CheckPet(); /// mal weg machen zum test
	// pFakePlayer->CheckMount();	
	pFakePlayer->ReviveInvisible(5);
}

bool CFakePlayer::LoadFakePlayer()
{
	char szQuery[1024] = {};
	snprintf(szQuery, sizeof(szQuery), "SELECT "
		"race+0, "
		"level, "
#ifdef ENABLE_YOHARA_SYSTEM
		"conqueror_level, "
#endif
		"empire+0, "
		"st, "
		"ht, "
		"dx, "
		"iq, "
		"alignment, "
		"block_eq, "
		"guild_id, "
		"copy_eq_of, "
		"language"
		" FROM fake_player WHERE name = '%s'", m_pFakePlayer->GetName());
	
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery)); 
    if (!pMsg->Get()->uiNumRows)
		return false;
	
	size_t col = 0; 
    MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	std::string stCopyName = "";
	
	// basics
	m_pFakePlayer->SetRace(boost::lexical_cast<uint32_t>(row[col++]) - 1);
	m_pFakePlayer->SetLevel(boost::lexical_cast<uint32_t>(row[col++]));
#ifdef ENABLE_YOHARA_SYSTEM
	m_pFakePlayer->SetConquerorLevel(boost::lexical_cast<uint32_t>(row[col++]));
#endif
	m_pFakePlayer->SetEmpire(boost::lexical_cast<uint32_t>(row[col++]));
	m_pFakePlayer->SetPoint(POINT_ST, boost::lexical_cast<uint32_t>(row[col++]));
	m_pFakePlayer->SetPoint(POINT_HT, boost::lexical_cast<uint32_t>(row[col++]));
	m_pFakePlayer->SetPoint(POINT_DX, boost::lexical_cast<uint32_t>(row[col++]));
	m_pFakePlayer->SetPoint(POINT_IQ, boost::lexical_cast<uint32_t>(row[col++]));
	m_pFakePlayer->UpdateAlignment(boost::lexical_cast<uint32_t>(row[col++]) * 10);
	m_pFakePlayer->SetFakePlayerBlockEQ(boost::lexical_cast<uint32_t>(row[col++]));
	m_pFakePlayer->SetFakePlayerGuildID(boost::lexical_cast<uint32_t>(row[col++]));
	stCopyName = row[col++];
	// m_pFakePlayer->SetCharacterLanguage(boost::lexical_cast<uint32_t>(row[col++]));
	m_pFakePlayer->SetLanguage(boost::lexical_cast<uint32_t>(row[col++]));
	
	// constants
	m_pFakePlayer->SetPoint(POINT_MOV_SPEED, EFakePlayer::FAKE_PLAYER_MOV_SPEED);
	m_pFakePlayer->ComputePoints();
	m_pFakePlayer->PointChange(POINT_HP, m_pFakePlayer->GetMaxHP() - m_pFakePlayer->GetHP());
	m_pFakePlayer->StartRecoveryEvent();
	m_pFakePlayer->FakePlayerCreatePetSystem();
	m_pFakePlayer->FakePlayerCreateMountSystem();
	// m_pFakePlayer->SetAway(true);
	
	// items
	if ("" == stCopyName)
		LoadFakePlayerItem(m_pFakePlayer->GetName());
	else
		LoadFakePlayerItem(stCopyName.c_str());	
	return true;
}

void CFakePlayer::LoadFakePlayerItem(const char* szName)
{
	char szQuery[1024];
	snprintf(szQuery, sizeof(szQuery), 
		// "SELECT wear_pos+0, vnum"
		// ", socket0, socket1, socket2, socket3, socket4, socket5, socket6"
		// ", attrtype0, attrvalue0"
		// ", attrtype1, attrvalue1"
		// ", attrtype2, attrvalue2"
		// ", attrtype3, attrvalue3"
		// ", attrtype4, attrvalue4"
		// ", attrtype5, attrvalue5"
		// ", attrtype6, attrvalue6"
		// " FROM fake_player_item WHERE name = '%s'", szName);
	
	
		"SELECT wear_pos+0, vnum"
		",socket0,socket1,socket2,socket3,socket4,socket5,socket6,socket7,attrtype0,attrvalue0,attrtype1,attrvalue1,attrtype2,attrvalue2,attrtype3,attrvalue3,attrtype4,attrvalue4,attrtype5,attrvalue5,attrtype6,attrvalue6,attrtype7,attrvalue7,attrtype8,attrvalue8,attrtype9,attrvalue9,attrtype10,attrvalue10,attrtype11,attrvalue11,attrtype12,attrvalue12,attrtype13,attrvalue13,attrtype14,attrvalue14"
// #ifdef ENABLE_YOHARA_SYSTEM
		// ", randomtype0, randomvalue0, randomrow0"
		// ", randomtype1, randomvalue1, randomrow1"
		// ", randomtype2, randomvalue2, randomrow2"
// #endif
// #ifdef ENABLE_NEW_NAME_ITEM
		// ",name "
// #endif
		" FROM fake_player_item WHERE name = '%s'", szName);

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery)); 
    if (!pMsg->Get()->uiNumRows)
		return;
	
	MYSQL_ROW row;
	size_t col = 0; 
	TFakePlayerItemData itemData;
	for (size_t i = 0; i < mysql_num_rows(pMsg->Get()->pSQLResult); ++i)
	{
		col = 0;
		memset(&itemData, 0, sizeof(itemData));
		row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		
		itemData.bWearPos = boost::lexical_cast<uint32_t>(row[col++]) - 1;
		itemData.dwVnum = boost::lexical_cast<uint32_t>(row[col++]);
		
		for (size_t i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			itemData.alSockets[i] = boost::lexical_cast<uint32_t>(row[col++]);
		
		for (size_t i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			itemData.aAttr[i].bType = boost::lexical_cast<uint32_t>(row[col++]);
			itemData.aAttr[i].sValue = boost::lexical_cast<uint32_t>(row[col++]);
		}
		
		LPITEM item = ITEM_MANAGER::instance().CreateItem(itemData.dwVnum);
		if (!item)
		{
			sys_err("<FakePlayer> cannot create item by vnum %u (name %s)", itemData.dwVnum, szName);
			continue;
		}

		item->SetSkipSave(true);
		item->SetSockets(itemData.alSockets);
		item->SetAttributes(itemData.aAttr);

		if (item->CheckItemUseLevel(m_pFakePlayer->GetLevel()
#ifdef ENABLE_YOHARA_SYSTEM
, m_pFakePlayer->GetConquerorLevel()
#endif
) && item->EquipTo(m_pFakePlayer, itemData.bWearPos)
		)
		{
			item->StopExpireEvents();
		}
		else
		{
			M2_DELETE(item);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFakePlayerManager
void CFakePlayerManager::Initialize()
{
	m_mapFakePlayer.clear();
}

void CFakePlayerManager::Destroy()
{
	Initialize();
}

void CFakePlayerManager::DGFakePlayer(uint32_t dwHandle, const char* c_pData)
{
	TPacketDGFakePlayer* p = (TPacketDGFakePlayer*)c_pData;
	c_pData += sizeof(TPacketDGFakePlayer);

	switch (p->bSubheader)
	{	
		case EFakePlayerDGSubheaders::FAKE_PLAYER_SUBHEADER_DG_LOGIN:
			{	
				SpawnFakePlayer((TFakePlayerLoginData*)c_pData);
				return;
			}
			
		case EFakePlayerDGSubheaders::FAKE_PLAYER_SUBHEADER_DG_LOGOUT:
			{	
				DestroyFakePlayer((TFakePlayerLogoutData*)c_pData);
				return;
			}	
			
		case EFakePlayerDGSubheaders::FAKE_PLAYER_SUBHEADER_DG_ALREADY_EXISTS:
			{
				if (LPDESC desc = DESC_MANAGER::instance().FindByHandle(dwHandle); desc)
				{
					if (LPCHARACTER ch = desc->GetCharacter(); ch)
						ch->ChatPacket(CHAT_TYPE_INFO, "<FakePlayer> This FakePlayer already exists.");
				}
				
				return;
			}

		case EFakePlayerDGSubheaders::FAKE_PLAYER_SUBHEADER_DG_DONT_EXISTS:
			{
				if (LPDESC desc = DESC_MANAGER::instance().FindByHandle(dwHandle); desc)
				{
					if (LPCHARACTER ch = desc->GetCharacter(); ch)
						ch->ChatPacket(CHAT_TYPE_INFO, "<FakePlayer> This FakePlayer dont exists.");
				}
				
				return;
			}
			
		default:
			sys_err("CFakePlayerManager::DGFakePlayer: Unknown subheader %d", p->bSubheader);
			break;
	}
}

void CFakePlayerManager::FakePlayerLogin(LPCHARACTER ch, TFakePlayerLoginData* fakePlayerLoginData)
{
	TPacketGDFakePlayer packet;
	packet.bSubheader = EFakePlayerGDSubheaders::FAKE_PLAYER_SUBHEADER_GD_LOGIN;

	TEMP_BUFFER buf;
	buf.write(&packet, sizeof(packet));
	buf.write(fakePlayerLoginData, sizeof(TFakePlayerLoginData));
	db_clientdesc->DBPacket(HEADER_GD_FAKE_PLAYER, ch->GetDesc()->GetHandle(), buf.read_peek(), buf.size());	
}

void CFakePlayerManager::FakePlayerLogout(const char* szName, LPCHARACTER ch)
{
	TPacketGDFakePlayer packet;
	packet.bSubheader = EFakePlayerGDSubheaders::FAKE_PLAYER_SUBHEADER_GD_LOGOUT;
	
	TFakePlayerLogoutData fakePlayerLogoutData;
	strlcpy(fakePlayerLogoutData.szName, szName, sizeof(fakePlayerLogoutData.szName));
	
	TEMP_BUFFER buf;
	buf.write(&packet, sizeof(packet));
	buf.write(&fakePlayerLogoutData, sizeof(fakePlayerLogoutData));
	if (ch)
		db_clientdesc->DBPacket(HEADER_GD_FAKE_PLAYER, ch->GetDesc()->GetHandle(), buf.read_peek(), buf.size());	
	else
		db_clientdesc->DBPacket(HEADER_GD_FAKE_PLAYER, 0, buf.read_peek(), buf.size());	
}

void CFakePlayerManager::SpawnFakePlayer(TFakePlayerLoginData* fakePlayerLoginData)
{
	std::unique_ptr<CFakePlayer> pFakePlayer = std::make_unique<CFakePlayer>(fakePlayerLoginData);	
	m_mapFakePlayer.insert(TFakePlayerMap::value_type(fakePlayerLoginData->szName, std::move(pFakePlayer)));
}

void CFakePlayerManager::DestroyFakePlayer(TFakePlayerLogoutData* fakePlayerLogoutData)
{
	const auto it = m_mapFakePlayer.find(fakePlayerLogoutData->szName);
	if (it == m_mapFakePlayer.end())
		return;
	
	m_mapFakePlayer.erase(it);
}

LPCHARACTER CFakePlayerManager::FindFakePlayer(std::string stName)
{
	const auto it = m_mapFakePlayer.find(stName);
	if (it == m_mapFakePlayer.end())
		return nullptr;
	
	return it->second->GetFakePlayer();
}
#endif