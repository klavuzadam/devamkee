#include "stdafx.h"
#include "constants.h"
#include "config.h"
#include "utils.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "protocol.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "cmd.h"
#include "shop.h"
#include "shop_manager.h"
#include "safebox.h"
#include "regen.h"
#include "battle.h"
#include "exchange.h"
#include "questmanager.h"
#include "profiler.h"
#include "messenger_manager.h"
#include "party.h"
#include "p2p.h"
#include "affect.h"
#include "guild.h"
#include "guild_manager.h"
#include "log.h"
#include "banword.h"
#include "empire_text_convert.h"
#include "unique_item.h"
#include "building.h"
#include "locale_service.h"
#include "gm.h"
#include "spam.h"
#include "ani.h"
#include "motion.h"
#include "OXEvent.h"
#include "locale_service.h"
#include "DragonSoul.h"

#include "belt_inventory_helper.h" // @fixme119
#include "../common/CommonDefines.h"
#include "input.h"
#include "battlepass.h"
// #define ENABLE_CHAT_COLOR_SYSTEM
#define __DISABLE_LOW_LEVEL_COMM__ // block level 30 below chat and whisper
#define ENABLE_CHAT_LOGGING
#define ENABLE_CHAT_SPAMLIMIT
#define ENABLE_WHISPER_CHAT_SPAMLIMIT
#ifdef ENABLE_SWITCHBOT_WORLDARD
#include "switchbonus.h"
#endif
#ifdef ENABLE_CHAT_LOGGING
static char	__escape_string[1024];
static char	__escape_string2[1024];
#endif
#if defined(WJ_COMBAT_ZONE)
	#include "combat_zone.h"
#endif
#ifdef __FAKE_PLAYER__
	#include "FakePlayerManager.h"
#endif
#ifdef ENABLE_QUEEN_NETHIS
#	include "SnakeLair.h"
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
#	include "minigame_manager.h"
#endif
#if defined(ENABLE_FISHING_GAME)
#include "fishing.h"
#endif

#ifdef ENABLE_SWITCHBOT
#include "new_switchbot.h"
#endif

#define QC_EMPIRE_RED		"a"
#define QC_EMPIRE_YELLOW	"b"
#define QC_EMPIRE_BLUE		"c"
#define QC_EMPTY_DATA		""

static int __deposit_limit()
{
	return (1000*10000); // 1Ãµ¸¸
}
#ifdef __NEWPET_SYSTEM__
#include "New_PetSystem.h"
#endif

#ifdef ENABLE_DUNGEON_INFO
#include "dungeon_info.h"
#endif

#ifdef ENABLE_DROP_ITEM_WORLDARD
#include "drop_item.h"
#endif


#ifdef __SEND_TARGET_INFO__
void CInputMain::TargetInfoLoad(LPCHARACTER ch, const char* c_pData)
{
	const auto* p = reinterpret_cast<const TPacketCGTargetInfoLoad*>(c_pData);
	if (!p || p->vid == 0)
	{
		return;
	}

	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	LPCHARACTER mobTarget = CHARACTER_MANAGER::instance().Find(p->vid);
	if (!mobTarget || mobTarget->IsPC())
	{
		return;
	}

	if (!mobTarget->IsMonster() && !mobTarget->IsStone())
	{
		return;
	}

	std::vector<LPITEM> vecDrop;
	if (ITEM_MANAGER::instance().CreateDropItemVector(mobTarget, ch, vecDrop))
	{
		
	}

	std::vector<STargetItemInfo> vecRes;

	if (!vecDrop.empty())
	{
		STargetItemInfo info = {};

		for (const auto& item : vecDrop)
		{
			info.vnum = item->GetVnum();
			info.count = item->GetCount();

			vecRes.emplace_back(info);
		}
	}

	TPacketGCTargetInfo pGC = {};
	pGC.header = HEADER_GC_TARGET_INFO;
	pGC.count = vecRes.size();
	pGC.size = sizeof(pGC) + (sizeof(STargetItemInfo) * pGC.count);
	pGC.vid = p->vid;
	pGC.race = mobTarget->GetRaceNum();

	TEMP_BUFFER buff;
	buff.write(&pGC, sizeof(pGC));

	if (pGC.count > 0)
	{
		buff.write(vecRes.data(), pGC.count * sizeof(vecRes[0]));
	}

	d->Packet(buff.read_peek(), buff.size());
}
#endif

void SendBlockChatInfo(LPCHARACTER ch, int sec)
{
	if (sec <= 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "739");
		return;
	}

	long hour = sec / 3600;
	sec -= hour * 3600;

	long min = (sec / 60);
	sec -= min * 60;

	char buf[128+1];

	if (hour > 0 && min > 0)
		snprintf(buf, sizeof(buf), "740 %ld %ld %d", hour, min, sec);
	else if (hour > 0 && min == 0)
		snprintf(buf, sizeof(buf), "741 %ld %d", hour, sec);
	else if (hour == 0 && min > 0)
		snprintf(buf, sizeof(buf), "742 %ld %d", min, sec);
	else
		snprintf(buf, sizeof(buf), "743 %d", sec);

	ch->ChatPacket(CHAT_TYPE_INFO, buf);
}

EVENTINFO(spam_event_info)
{
	char host[MAX_HOST_LENGTH+1];

	spam_event_info()
	{
		::memset( host, 0, MAX_HOST_LENGTH+1 );
	}
};

typedef boost::unordered_map<std::string, std::pair<unsigned int, LPEVENT> > spam_score_of_ip_t;
spam_score_of_ip_t spam_score_of_ip;

EVENTFUNC(block_chat_by_ip_event)
{
	spam_event_info* info = dynamic_cast<spam_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "block_chat_by_ip_event> <Factor> Null pointer" );
		return 0;
	}

	const char * host = info->host;

	spam_score_of_ip_t::iterator it = spam_score_of_ip.find(host);

	if (it != spam_score_of_ip.end())
	{
		it->second.first = 0;
		it->second.second = NULL;
	}

	return 0;
}

bool SpamBlockCheck(LPCHARACTER ch, const char* const buf, const size_t buflen)
{
	if (ch->GetLevel() < g_iSpamBlockMaxLevel)
	{
		spam_score_of_ip_t::iterator it = spam_score_of_ip.find(ch->GetDesc()->GetHostName());

		if (it == spam_score_of_ip.end())
		{
			spam_score_of_ip.insert(std::make_pair(ch->GetDesc()->GetHostName(), std::make_pair(0, (LPEVENT) NULL)));
			it = spam_score_of_ip.find(ch->GetDesc()->GetHostName());
		}

		if (it->second.second)
		{
			SendBlockChatInfo(ch, event_time(it->second.second) / passes_per_sec);
			return true;
		}

		unsigned int score;
		const char * word = SpamManager::instance().GetSpamScore(buf, buflen, score);

		it->second.first += score;

		if (word)
			sys_log(0, "SPAM_SCORE: %s text: %s score: %u total: %u word: %s", ch->GetName(), buf, score, it->second.first, word);

		if (it->second.first >= g_uiSpamBlockScore)
		{
			spam_event_info* info = AllocEventInfo<spam_event_info>();
			strlcpy(info->host, ch->GetDesc()->GetHostName(), sizeof(info->host));

			it->second.second = event_create(block_chat_by_ip_event, info, PASSES_PER_SEC(g_uiSpamBlockDuration));
			sys_log(0, "SPAM_IP: %s for %u seconds", info->host, g_uiSpamBlockDuration);

			LogManager::instance().CharLog(ch, 0, "SPAM", word);

			SendBlockChatInfo(ch, event_time(it->second.second) / passes_per_sec);

			return true;
		}
	}

	return false;
}

enum
{
	TEXT_TAG_PLAIN,
	TEXT_TAG_TAG, // ||
	TEXT_TAG_COLOR, // |cffffffff
	TEXT_TAG_HYPERLINK_START, // |H
	TEXT_TAG_HYPERLINK_END, // |h ex) |Hitem:1234:1:1:1|h
	TEXT_TAG_RESTORE_COLOR,
};

int GetTextTag(const char * src, int maxLen, int & tagLen, std::string & extraInfo)
{
	tagLen = 1;

	if (maxLen < 2 || *src != '|')
		return TEXT_TAG_PLAIN;

	const char * cur = ++src;

	if (*cur == '|') // ||´Â |·Î Ç¥½ÃÇÑ´Ù.
	{
		tagLen = 2;
		return TEXT_TAG_TAG;
	}
	else if (*cur == 'c') // color |cffffffffblahblah|r
	{
		tagLen = 2;
		return TEXT_TAG_COLOR;
	}
	else if (*cur == 'H') // hyperlink |Hitem:10000:0:0:0:0|h[ÀÌ¸§]|h
	{
		tagLen = 2;
		return TEXT_TAG_HYPERLINK_START;
	}
	else if (*cur == 'h') // end of hyperlink
	{
		tagLen = 2;
		return TEXT_TAG_HYPERLINK_END;
	}

	return TEXT_TAG_PLAIN;
}

void GetTextTagInfo(const char * src, int src_len, int & hyperlinks, bool & colored)
{
	colored = false;
	hyperlinks = 0;

	int len;
	std::string extraInfo;

	for (int i = 0; i < src_len;)
	{
		int tag = GetTextTag(&src[i], src_len - i, len, extraInfo);

		if (tag == TEXT_TAG_HYPERLINK_START)
			++hyperlinks;

		if (tag == TEXT_TAG_COLOR)
			colored = true;

		i += len;
	}
}

int ProcessTextTag(LPCHARACTER ch, const char * c_pszText, size_t len)
{
	return 0;
	//°³ÀÎ»óÁ¡Áß¿¡ ±Ý°­°æÀ» »ç¿ëÇÒ °æ¿ì
	//0 : Á¤»óÀûÀ¸·Î »ç¿ë
	//1 : ±Ý°­°æ ºÎÁ·
	//2 : ±Ý°­°æÀÌ ÀÖÀ¸³ª, °³ÀÎ»óÁ¡¿¡¼­ »ç¿ëÁß
	//3 : ±âÅ¸
	//4 : ¿¡·¯
	int hyperlinks;
	bool colored;

	GetTextTagInfo(c_pszText, len, hyperlinks, colored);

	if (colored == true && hyperlinks == 0)
		return 4;

#ifdef ENABLE_NEWSTUFF
	if (g_bDisablePrismNeed)
		return 0;
#endif
	int nPrismCount = ch->CountSpecifyItem(ITEM_PRISM);

	if (nPrismCount < hyperlinks)
		return 1;

	ch->RemoveSpecifyItem(ITEM_PRISM, hyperlinks);
	return 0;

}

int CInputMain::Whisper(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	const TPacketCGWhisper* pinfo = reinterpret_cast<const TPacketCGWhisper*>(data);

	if (uiBytes < pinfo->wSize)
		return -1;

	int iExtraLen = pinfo->wSize - sizeof(TPacketCGWhisper);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->wSize, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}

#ifdef __DISABLE_LOW_LEVEL_COMM__
	if (ch->GetLevel() < 30 && !ch->IsGM())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Cannot whisper below level 30."));
		return iExtraLen;
	}
#endif

#ifdef __NEW_BANWORD__
	char bufBanword[CHAT_MAX_LEN + 1];
	strlcpy(bufBanword, data + sizeof(TPacketCGWhisper), MIN(iExtraLen + 1, sizeof(bufBanword)));
	if (CHARACTER_MANAGER::Instance().IsBanword(ch, bufBanword))
	{
		if(ch->GetProtectTime("hwid_ban"))
		{
			ch->GetDesc()->SetPhase(PHASE_CLOSE);
			return -1;
		}
		return iExtraLen;
	}
#endif

#ifdef ENABLE_WHISPER_CHAT_SPAMLIMIT
	if (ch->IncreaseChatCounter() >= 10)
	{
		ch->GetDesc()->DelayedDisconnect(0);
		return (iExtraLen);
	}
#endif

	if (ch->GetLevel() < 1 && !ch->IsGM())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Tu nivel no es suficiente para realizar esta acci?, necesitas ser level 1.");
		return (iExtraLen);
	}



	if (ch->FindAffect(AFFECT_BLOCK_CHAT))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "739");
		return (iExtraLen);
	}
#if defined(WJ_COMBAT_ZONE) && defined(WJ_COMBAT_ZONE_HIDE_INFO_USER)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()) && (ch->GetGMLevel() < GM_IMPLEMENTOR))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "744");
		return (iExtraLen);
	}
#endif
	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindPC(pinfo->szNameTo);

	if (pkChr == ch)
		return (iExtraLen);

#if defined(__BL_BATTLE_ROYALE__)
	if (ch && ch->IsBattleRoyale())
		return (iExtraLen);

	if (pkChr && pkChr->IsBattleRoyale())
		return (iExtraLen);
#endif

	LPDESC pkDesc = NULL;

	BYTE bOpponentEmpire = 0;

	if (test_server)
	{
		if (!pkChr)
			sys_log(0, "Whisper to %s(%s) from %s", "Null", pinfo->szNameTo, ch->GetName());
		else
			sys_log(0, "Whisper to %s(%s) from %s", pkChr->GetName(), pinfo->szNameTo, ch->GetName());
	}

	if (ch->IsBlockMode(BLOCK_WHISPER))
	{
		if (ch->GetDesc())
		{
			TPacketGCWhisper pack;
			pack.bHeader = HEADER_GC_WHISPER;
			pack.bType = WHISPER_TYPE_SENDER_BLOCKED;
			pack.wSize = sizeof(TPacketGCWhisper);
			strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
			ch->GetDesc()->Packet(&pack, sizeof(pack));
		}
		return iExtraLen;
	}

	if (!pkChr)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(pinfo->szNameTo);

		if (pkCCI)
		{
			pkDesc = pkCCI->pkDesc;
			pkDesc->SetRelay(pinfo->szNameTo);
			bOpponentEmpire = pkCCI->bEmpire;

			if (test_server)
				sys_log(0, "Whisper to %s from %s (Channel %d Mapindex %d)", "Null", ch->GetName(), pkCCI->bChannel, pkCCI->lMapIndex);
#ifdef ENABLE_BATTLE_PASS
			CHARACTER_MANAGER::Instance().DoMission(ch, MISSION_WHISPER, 1, 0);
#endif
#ifdef __NEW_BANWORD__
			CHARACTER_MANAGER::Instance().LoadBanword();
#endif
		}
	}
	else
	{
		pkDesc = pkChr->GetDesc();
		bOpponentEmpire = pkChr->GetEmpire();
	}

	if (!pkDesc)
	{
#ifdef __FAKE_PLAYER__
		if (LPCHARACTER pFakePlayer = CFakePlayerManager::instance().FindFakePlayer(pinfo->szNameTo); pFakePlayer)
			return iExtraLen;
#endif	
		if (ch->GetDesc())
		{
			TPacketGCWhisper pack;

			pack.bHeader = HEADER_GC_WHISPER;
			pack.bType = WHISPER_TYPE_NOT_EXIST;
			pack.wSize = sizeof(TPacketGCWhisper);
			strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
			ch->GetDesc()->Packet(&pack, sizeof(TPacketGCWhisper));
			sys_log(0, "WHISPER: no player");
		}
	}
	else
	{
		if (ch->IsBlockMode(BLOCK_WHISPER))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;
				pack.bHeader = HEADER_GC_WHISPER;
				pack.bType = WHISPER_TYPE_SENDER_BLOCKED;
				pack.wSize = sizeof(TPacketGCWhisper);
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
		}
		else if (pkChr && pkChr->IsBlockMode(BLOCK_WHISPER))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;
				pack.bHeader = HEADER_GC_WHISPER;
				pack.bType = WHISPER_TYPE_TARGET_BLOCKED;
				pack.wSize = sizeof(TPacketGCWhisper);
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
		}
#ifdef ENABLE_MESSENGER_BLOCK
		else if (pkDesc && MessengerManager::instance().IsBlocked(ch->GetName(), pinfo->szNameTo))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;

				char msg[CHAT_MAX_LEN + 1];
				snprintf(msg, sizeof(msg), "Unlock %s to continue.", pinfo->szNameTo);
				int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + len;
				pack.bType = WHISPER_TYPE_SYSTEM;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));

				TEMP_BUFFER buf;

				buf.write(&pack, sizeof(TPacketGCWhisper));
				buf.write(msg, len);
				ch->GetDesc()->Packet(buf.read_peek(), buf.size());
			}
		}
		else if (pkDesc && MessengerManager::instance().IsBlocked(pinfo->szNameTo, ch->GetName()))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;

				char msg[CHAT_MAX_LEN + 1];
				snprintf(msg, sizeof(msg), "%s has blocked you.", pinfo->szNameTo);
				int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + len;
				pack.bType = WHISPER_TYPE_SYSTEM;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));

				TEMP_BUFFER buf;

				buf.write(&pack, sizeof(TPacketGCWhisper));
				buf.write(msg, len);
				ch->GetDesc()->Packet(buf.read_peek(), buf.size());
			}
		}
#endif

		else
		{
			BYTE bType = WHISPER_TYPE_NORMAL;

			char buf[CHAT_MAX_LEN + 1];
			strlcpy(buf, data + sizeof(TPacketCGWhisper), MIN(iExtraLen + 1, sizeof(buf)));
			const size_t buflen = strlen(buf);

			if (true == SpamBlockCheck(ch, buf, buflen))
			{
				if (!pkChr)
				{
					CCI * pkCCI = P2P_MANAGER::instance().Find(pinfo->szNameTo);

					if (pkCCI)
					{
						pkDesc->SetRelay("");
					}
				}
				return iExtraLen;
			}

			CBanwordManager::instance().ConvertString(buf, buflen);

			if (g_bEmpireWhisper)
				if (!ch->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE))
					if (!(pkChr && pkChr->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE)))
						if (bOpponentEmpire != ch->GetEmpire() && ch->GetEmpire() && bOpponentEmpire // ¼­·Î Á¦±¹ÀÌ ´Ù¸£¸é¼­
								&& ch->GetGMLevel() == GM_PLAYER && gm_get_level(pinfo->szNameTo) == GM_PLAYER) // µÑ´Ù ÀÏ¹Ý ÇÃ·¹ÀÌ¾îÀÌ¸é
							// ÀÌ¸§ ¹Û¿¡ ¸ð¸£´Ï gm_get_level ÇÔ¼ö¸¦ »ç¿ë
						{
							if (!pkChr)
							{
								// ´Ù¸¥ ¼­¹ö¿¡ ÀÖÀ¸´Ï Á¦±¹ Ç¥½Ã¸¸ ÇÑ´Ù. bTypeÀÇ »óÀ§ 4ºñÆ®¸¦ Empire¹øÈ£·Î »ç¿ëÇÑ´Ù.
								bType = ch->GetEmpire() << 4;
							}
							else
							{
								ConvertEmpireText(ch->GetEmpire(), buf, buflen, 10 + 2 * pkChr->GetSkillPower(SKILL_LANGUAGE1 + ch->GetEmpire() - 1)/*º¯È¯È®·ü*/);
							}
						}

			int processReturn = ProcessTextTag(ch, buf, buflen);
			if (0!=processReturn)
			{
				if (ch->GetDesc())
				{
					TItemTable * pTable = ITEM_MANAGER::instance().GetTable(ITEM_PRISM);

					if (pTable)
					{
						char buf[128];
						int len;
						if (3==processReturn) //±³È¯Áß
							len = snprintf(buf, sizeof(buf), "745");
						else
							len = snprintf(buf, sizeof(buf), "746 %s", pTable->szLocaleName);

						if (len < 0 || len >= (int) sizeof(buf))
							len = sizeof(buf) - 1;

						++len;  // \0 ¹®ÀÚ Æ÷ÇÔ

						TPacketGCWhisper pack;

						pack.bHeader = HEADER_GC_WHISPER;
						pack.bType = WHISPER_TYPE_ERROR;
						pack.wSize = sizeof(TPacketGCWhisper) + len;
						strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));

						ch->GetDesc()->BufferedPacket(&pack, sizeof(pack));
						ch->GetDesc()->Packet(buf, len);

						sys_log(0, "WHISPER: not enough %s: char: %s", pTable->szLocaleName, ch->GetName());
					}
				}

				// ¸±·¡ÀÌ »óÅÂÀÏ ¼ö ÀÖÀ¸¹Ç·Î ¸±·¡ÀÌ¸¦ Ç®¾îÁØ´Ù.
				pkDesc->SetRelay("");
				return (iExtraLen);
			}

			if (ch->IsGM())
				bType = (bType & 0xF0) | WHISPER_TYPE_GM;

			if (buflen > 0)
			{
				TPacketGCWhisper pack;

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + buflen;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
				pack.bLanguage = ch->GetCharacterLanguage();
				pack.bEmpire = ch->GetEmpire();
#endif
				pack.bType = bType;
				strlcpy(pack.szNameFrom, ch->GetName(), sizeof(pack.szNameFrom));

				// desc->BufferedPacketÀ» ÇÏÁö ¾Ê°í ¹öÆÛ¿¡ ½á¾ßÇÏ´Â ÀÌÀ¯´Â
				// P2P relayµÇ¾î ÆÐÅ¶ÀÌ Ä¸½¶È­ µÉ ¼ö ÀÖ±â ¶§¹®ÀÌ´Ù.
				TEMP_BUFFER tmpbuf;

				tmpbuf.write(&pack, sizeof(pack));
				tmpbuf.write(buf, buflen);

				pkDesc->Packet(tmpbuf.read_peek(), tmpbuf.size());

#ifdef ENABLE_BATTLE_PASS
				CHARACTER_MANAGER::Instance().DoMission(ch, MISSION_WHISPER, 1, 0);
#endif

				// @warme006
				// sys_log(0, "WHISPER: %s -> %s : %s", ch->GetName(), pinfo->szNameTo, buf);

				LogManager::instance().WhisperLog(ch->GetName(), pinfo->szNameTo, buf);

#ifdef ENABLE_CHAT_LOGGING
				if (ch->IsGM())
				{
					LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), buf, buflen);
					LogManager::instance().EscapeString(__escape_string2, sizeof(__escape_string2), pinfo->szNameTo, sizeof(pack.szNameFrom));
					LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), 0, __escape_string2, "WHISPER", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
				}
#endif
			}
		}
	}
	if(pkDesc)
		pkDesc->SetRelay("");

	return (iExtraLen);
}

struct RawPacketToCharacterFunc
{
	const void * m_buf;
	int	m_buf_len;

	RawPacketToCharacterFunc(const void * buf, int buf_len) : m_buf(buf), m_buf_len(buf_len)
	{
	}

	void operator () (LPCHARACTER c)
	{
		if (!c->GetDesc())
			return;

		c->GetDesc()->Packet(m_buf, m_buf_len);
	}
};

struct FEmpireChatPacket
{
	packet_chat& p;
	const char* orig_msg;
	int orig_len;
	char converted_msg[CHAT_MAX_LEN+1];

	BYTE bEmpire;
	int iMapIndex;
	int namelen;

	FEmpireChatPacket(packet_chat& p, const char* chat_msg, int len, BYTE bEmpire, int iMapIndex, int iNameLen)
		: p(p), orig_msg(chat_msg), orig_len(len), bEmpire(bEmpire), iMapIndex(iMapIndex), namelen(iNameLen)
	{
		memset( converted_msg, 0, sizeof(converted_msg) );
	}

	void operator () (LPDESC d)
	{
		if (!d->GetCharacter())
			return;

		if (d->GetCharacter()->GetMapIndex() != iMapIndex)
			return;

		d->BufferedPacket(&p, sizeof(packet_chat));

		if (d->GetEmpire() == bEmpire ||
			bEmpire == 0 ||
			d->GetCharacter()->GetGMLevel() > GM_PLAYER ||
			d->GetCharacter()->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE))
		{
			d->Packet(orig_msg, orig_len);
		}
		else
		{
			// »ç¶÷¸¶´Ù ½ºÅ³·¹º§ÀÌ ´Ù¸£´Ï ¸Å¹ø ÇØ¾ßÇÕ´Ï´Ù
			size_t len = strlcpy(converted_msg, orig_msg, sizeof(converted_msg));

			if (len >= sizeof(converted_msg))
				len = sizeof(converted_msg) - 1;

			ConvertEmpireText(bEmpire, converted_msg + namelen, len - namelen, 10 + 2 * d->GetCharacter()->GetSkillPower(SKILL_LANGUAGE1 + bEmpire - 1));
			d->Packet(converted_msg, orig_len);
		}
	}
};

struct FYmirChatPacket
{
	packet_chat& packet;
	const char* m_szChat;
	size_t m_lenChat;
	const char* m_szName;

	int m_iMapIndex;
	BYTE m_bEmpire;
	bool m_ring;

	char m_orig_msg[CHAT_MAX_LEN+1];
	int m_len_orig_msg;
	char m_conv_msg[CHAT_MAX_LEN+1];
	int m_len_conv_msg;

	FYmirChatPacket(packet_chat& p, const char* chat, size_t len_chat, const char* name, size_t len_name, int iMapIndex, BYTE empire, bool ring)
		: packet(p),
		m_szChat(chat), m_lenChat(len_chat),
		m_szName(name),
		m_iMapIndex(iMapIndex), m_bEmpire(empire),
		m_ring(ring)
	{
		m_len_orig_msg = snprintf(m_orig_msg, sizeof(m_orig_msg), "%s : %s", m_szName, m_szChat) + 1; // ³Î ¹®ÀÚ Æ÷ÇÔ

		if (m_len_orig_msg < 0 || m_len_orig_msg >= (int) sizeof(m_orig_msg))
			m_len_orig_msg = sizeof(m_orig_msg) - 1;

		m_len_conv_msg = snprintf(m_conv_msg, sizeof(m_conv_msg), "??? : %s", m_szChat) + 1; // ³Î ¹®ÀÚ ¹ÌÆ÷ÇÔ

		if (m_len_conv_msg < 0 || m_len_conv_msg >= (int) sizeof(m_conv_msg))
			m_len_conv_msg = sizeof(m_conv_msg) - 1;

		ConvertEmpireText(m_bEmpire, m_conv_msg + 6, m_len_conv_msg - 6, 10); // 6Àº "??? : "ÀÇ ±æÀÌ
	}

	void operator() (LPDESC d)
	{
		if (!d->GetCharacter())
			return;

		if (d->GetCharacter()->GetMapIndex() != m_iMapIndex)
			return;

		if (m_ring ||
			d->GetEmpire() == m_bEmpire ||
			d->GetCharacter()->GetGMLevel() > GM_PLAYER ||
			d->GetCharacter()->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE))
		{
			packet.size = m_len_orig_msg + sizeof(TPacketGCChat);

			d->BufferedPacket(&packet, sizeof(packet_chat));
			d->Packet(m_orig_msg, m_len_orig_msg);
		}
		else
		{
			packet.size = m_len_conv_msg + sizeof(TPacketGCChat);

			d->BufferedPacket(&packet, sizeof(packet_chat));
			d->Packet(m_conv_msg, m_len_conv_msg);
		}
	}
};
#ifdef __NEWPET_SYSTEM__
void CInputMain::BraveRequestPetName(LPCHARACTER ch, const char* c_pData)
{
	if (!ch->GetDesc()) { return; }
	int vid = ch->GetEggVid();
	if (vid == 0) { return; }

	TPacketCGRequestPetName* p = (TPacketCGRequestPetName*)c_pData;

	if (ch->GetGold() < 100000) {
		ch->ChatPacket(CHAT_TYPE_INFO, "Hai bisogno di 100.000 yang");
	}

	if (ch->CountSpecifyItem(vid) > 0 && check_name(p->petname) != 0) {
		DBManager::instance().SendMoneyLog(MONEY_LOG_QUEST, ch->GetPlayerID(), -100000);
		ch->PointChange(POINT_GOLD, -100000, true);
		ch->RemoveSpecifyItem(vid, 1);
		LPITEM item = ch->AutoGiveItem(vid + 300, 1);
		int tmpslot = number(1, 3);
		int tmpskill[3] = { 0, 0, 0 };
		for (int i = 0; i < 3; ++i)
		{
			if (i > tmpslot - 1)
				tmpskill[i] = -1;
		}
		int tmpdur = number(1, 14) * 24 * 60;
		char szQuery1[1024];
		snprintf(szQuery1, sizeof(szQuery1), "INSERT INTO new_petsystem VALUES(%lu,'%s', 1, 0, 0, 0, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", item->GetID(), p->petname, number(1, 23), number(1, 23), number(1, 23), tmpskill[0], 0, tmpskill[1], 0, tmpskill[2], 0, tmpdur, tmpdur);
		std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
	}
	else {
		ch->ChatPacket(CHAT_TYPE_INFO, "Errore nell'imposztazione del nome.");
	}
}
#endif

int CInputMain::Chat(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	const TPacketCGChat* pinfo = reinterpret_cast<const TPacketCGChat*>(data);

	if (uiBytes < pinfo->size)
		return -1;

	const int iExtraLen = pinfo->size - sizeof(TPacketCGChat);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->size, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}

	char buf[CHAT_MAX_LEN - (CHARACTER_NAME_MAX_LEN + 3) + 1];
	strlcpy(buf, data + sizeof(TPacketCGChat), MIN(iExtraLen + 1, sizeof(buf)));
	const size_t buflen = strlen(buf);

	if (buflen > 1 && *buf == '/')
	{
		interpret_command(ch, buf + 1, buflen - 1);
		return iExtraLen;
	}

#if defined(__BL_BATTLE_ROYALE__)
	if (ch->IsBattleRoyale())
		return iExtraLen;
#endif

#ifdef __DISABLE_LOW_LEVEL_COMM__
	switch (pinfo->type)
	{
		case CHAT_TYPE_TALKING:
		case CHAT_TYPE_SHOUT:
		{
			if (ch->GetLevel() < 30 && !ch->IsGM())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Cannot send chat messages below level 30."));
				return iExtraLen;
			}

		}
		break;
	}
#endif

#ifdef ENABLE_CHAT_SPAMLIMIT
	if (ch->IncreaseChatCounter() >= 30)
	{
		if (ch->GetChatCounter() == 30)
			ch->GetDesc()->DelayedDisconnect(0);
		return iExtraLen;
	}
#else
	if (ch->IncreaseChatCounter() >= 30)
	{
		if (ch->GetChatCounter() == 30)
		{
			sys_log(0, "CHAT_HACK: %s", ch->GetName());
			ch->GetDesc()->DelayedDisconnect(5);
		}

		return iExtraLen;
	}
#endif

	// Ã¤ÆÃ ±ÝÁö Affect Ã³¸®
	const CAffect* pAffect = ch->FindAffect(AFFECT_BLOCK_CHAT);

	if (pAffect != NULL)
	{
		SendBlockChatInfo(ch, pAffect->lDuration);
		return iExtraLen;
	}

#ifdef __NEW_BANWORD__
	if (CHARACTER_MANAGER::Instance().IsBanword(ch, buf))
	{
		if(ch->GetProtectTime("hwid_ban"))
		{
			ch->GetDesc()->SetPhase(PHASE_CLOSE);
			return -1;
		}
		return iExtraLen;
	}
#endif

	if (true == SpamBlockCheck(ch, buf, buflen))
	{
		return iExtraLen;
	}

	if (ch->GetLevel() < 1 && !ch->IsGM())
	{
		if (CHAT_TYPE_SHOUT == pinfo->type || CHAT_TYPE_TALKING == pinfo->type){
			ch->ChatPacket(CHAT_TYPE_INFO, "Tu nivel no es suficiente para realizar esta acci?, necesitas ser level 1.");
			return iExtraLen;
		}
	}


#if defined(WJ_COMBAT_ZONE) && defined(WJ_COMBAT_ZONE_HIDE_INFO_USER)	
	if (CCombatZoneManager::Instance().IsCombatZoneMap(ch->GetMapIndex()) && (ch->GetGMLevel() < GM_IMPLEMENTOR))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "747");
		return iExtraLen;
	}
#endif
	// @fixme133 begin
	CBanwordManager::instance().ConvertString(buf, buflen);

	int processReturn = ProcessTextTag(ch, buf, buflen);
	if (0!=processReturn)
	{
		const TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ITEM_PRISM);

		if (NULL != pTable)
		{
			if (3==processReturn) //±³È¯Áß
				ch->ChatPacket(CHAT_TYPE_INFO, "745", pTable->szLocaleName);
			else
				ch->ChatPacket(CHAT_TYPE_INFO, "746 %s", pTable->szLocaleName);

		}

		return iExtraLen;
	}

	char chatbuf[CHAT_MAX_LEN + 1];
#ifdef ENABLE_CHAT_COLOR_SYSTEM
	int len = snprintf(chatbuf, sizeof(chatbuf), "%s%s : %s", ch->IsGM()?"|cFFffa200|H|h[Staff]|h|r ":"", ch->GetName(), buf);
#else
	int len = snprintf(chatbuf, sizeof(chatbuf), "%s : %s", ch->GetName(), buf);
#endif


#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	if (CHAT_TYPE_SHOUT == pinfo->type)
	{

//#ifdef SHOUT_LOG_EXPANDED
//		char test[CHAT_MAX_LEN + 1];
//		snprintf(test, sizeof(test), buf);
//		LogManager::instance().ShoutLog(g_bChannel, ch->GetEmpire(), ch->GetName(), test);
//#else
		LogManager::instance().ShoutLog(g_bChannel, ch->GetEmpire(), ch->GetName(), chatbuf);
//#endif

		std::string c_pszTempBuf = chatbuf;

		if(ch->IsGM())
			len = snprintf(chatbuf, sizeof(chatbuf), "|lang|%d||empire|%d||cFFffa200|H|h[Staff]|h|r %s", ch->GetCharacterLanguage(), ch->GetEmpire(), c_pszTempBuf.c_str());
		else
			len = snprintf(chatbuf, sizeof(chatbuf), "|lang|%d||empire|%d|%s", ch->GetCharacterLanguage(), ch->GetEmpire(), c_pszTempBuf.c_str());
	}
#endif

	if (len < 0 || len >= (int) sizeof(chatbuf))
		len = sizeof(chatbuf) - 1;

	if (pinfo->type == CHAT_TYPE_SHOUT)
	{
		// const int SHOUT_LIMIT_LEVEL = 15;

		if (ch->GetLevel() < g_iShoutLimitLevel)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "748 %d", g_iShoutLimitLevel);
			return (iExtraLen);
		}

		// if (thecore_heart->pulse - (int) ch->GetLastShoutPulse() < passes_per_sec * g_iShoutLimitTime)
		if (thecore_heart->pulse - (int) ch->GetLastShoutPulse() < passes_per_sec * 5)
			return (iExtraLen);

		ch->SetLastShoutPulse(thecore_heart->pulse);
#ifdef ENABLE_BATTLE_PASS
		CHARACTER_MANAGER::Instance().DoMission(ch, MISSION_SHOUT_CHAT, 1, 0);
#endif
#ifdef ENABLE_BATTLEPASS
		ch->UpdateMissionProgress(MISSION_SEND_SHOUT_MESSAGE, 1);
#endif
		TPacketGGShout p;

		p.bHeader = HEADER_GG_SHOUT;
		p.bEmpire = ch->GetEmpire();
		strlcpy(p.szText, chatbuf, sizeof(p.szText));

		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShout));

		SendShout(chatbuf, ch->GetEmpire());

		return (iExtraLen);
	}

	TPacketGCChat pack_chat;

	pack_chat.header = HEADER_GC_CHAT;
	pack_chat.size = sizeof(TPacketGCChat) + len;
	pack_chat.type = pinfo->type;
	pack_chat.id = ch->GetVID();

	switch (pinfo->type)
	{
		case CHAT_TYPE_TALKING:
			{
				const DESC_MANAGER::DESC_SET & c_ref_set = DESC_MANAGER::instance().GetClientSet();

				if (false)
				{
					std::for_each(c_ref_set.begin(), c_ref_set.end(),
							FYmirChatPacket(pack_chat,
								buf,
								strlen(buf),
								ch->GetName(),
								strlen(ch->GetName()),
								ch->GetMapIndex(),
								ch->GetEmpire(),
								ch->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE)));
				}
				else
				{
					std::for_each(c_ref_set.begin(), c_ref_set.end(),
							FEmpireChatPacket(pack_chat,
								chatbuf,
								len,
								(ch->GetGMLevel() > GM_PLAYER ||
								 ch->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE)) ? 0 : ch->GetEmpire(),
								ch->GetMapIndex(), strlen(ch->GetName())));
#ifdef ENABLE_CHAT_LOGGING
					if (ch->IsGM())
					{
						LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
						LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), 0, "", "NORMAL", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
					}
#endif
				}
			}
			break;

		case CHAT_TYPE_PARTY:
			{
				if (!ch->GetParty())
					ch->ChatPacket(CHAT_TYPE_INFO, "749");
				else
				{
					TEMP_BUFFER tbuf;

					tbuf.write(&pack_chat, sizeof(pack_chat));
					tbuf.write(chatbuf, len);

					RawPacketToCharacterFunc f(tbuf.read_peek(), tbuf.size());
					ch->GetParty()->ForEachOnlineMember(f);
#ifdef ENABLE_CHAT_LOGGING
					if (ch->IsGM())
					{
						LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
						LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), ch->GetParty()->GetLeaderPID(), "", "PARTY", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
					}
#endif
				}
			}
			break;

		case CHAT_TYPE_GUILD:
			{
				if (!ch->GetGuild())
					ch->ChatPacket(CHAT_TYPE_INFO, "750");
				else
				{
					ch->GetGuild()->Chat(chatbuf);
#ifdef ENABLE_CHAT_LOGGING
					if (ch->IsGM())
					{
						LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
						LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), ch->GetGuild()->GetID(), ch->GetGuild()->GetName(), "GUILD", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
					}
#endif
				}
			}
			break;

		default:
			sys_err("Unknown chat type %d", pinfo->type);
			break;
	}

	return (iExtraLen);
}

void CInputMain::ItemUse(LPCHARACTER ch, const char * data)
{
	ch->UseItem(((struct command_item_use *) data)->Cell);
}

void CInputMain::ItemToItem(LPCHARACTER ch, const char * pcData)
{
	TPacketCGItemUseToItem * p = (TPacketCGItemUseToItem *) pcData;
	if (ch)
		ch->UseItem(p->Cell, p->TargetCell);
}

void CInputMain::ItemDrop(LPCHARACTER ch, const char * data)
{
	struct command_item_drop * pinfo = (struct command_item_drop *) data;

	//MONARCH_LIMIT
	//if (ch->IsMonarch())
	//	return;
	//END_MONARCH_LIMIT
	if (!ch)
		return;

	// ¿¤Å©°¡ 0º¸´Ù Å©¸é ¿¤Å©¸¦ ¹ö¸®´Â °Í ÀÌ´Ù.
	if (pinfo->gold > 0)
		ch->DropGold(pinfo->gold);
	else
		ch->DropItem(pinfo->Cell);
}

void CInputMain::ItemDrop2(LPCHARACTER ch, const char * data)
{
	//MONARCH_LIMIT
	//if (ch->IsMonarch())
	//	return;
	//END_MONARCH_LIMIT

	TPacketCGItemDrop2 * pinfo = (TPacketCGItemDrop2 *) data;

	// ¿¤Å©°¡ 0º¸´Ù Å©¸é ¿¤Å©¸¦ ¹ö¸®´Â °Í ÀÌ´Ù.

	if (!ch)
		return;
	if (pinfo->gold > 0)
		ch->DropGold(pinfo->gold);
	else
		ch->DropItem(pinfo->Cell, pinfo->count);
}

#ifdef ENABLE_SELL_ITEM
void CInputMain::ItemSell(LPCHARACTER ch, const char * data)
{
	TPacketCGItemSell * pinfo = (TPacketCGItemSell *) data;
	
	if (!ch)
		return;

	if (pinfo->gold > 0)
		ch->DropGold(pinfo->gold);
	else
		ch->SellItem(pinfo->Cell);
}
#endif

void CInputMain::ItemMove(LPCHARACTER ch, const char * data)
{
	struct command_item_move * pinfo = (struct command_item_move *) data;

	if (ch)
		ch->MoveItem(pinfo->Cell, pinfo->CellTo, pinfo->count);
}

void CInputMain::ItemPickup(LPCHARACTER ch, const char * data)
{
	struct command_item_pickup * pinfo = (struct command_item_pickup*) data;
	if (ch)
#ifdef ENABLE_CHECK_PICKUP_HACK
		if (ch->PickupItem(pinfo->vid))
		{
			DWORD d_difference = get_dword_time() - ch->GetLastPickupTime();
			if (d_difference < ENABLE_CHECK_PICKUP_HACK && d_difference > 3)
			{
				LPDESC d = ch->GetDesc();
				if (d)
				{
					if (d->DelayedDisconnect(3))
					{
						LogManager::instance().HackLog("CShield-ServerSide-ErrorCode: Pickup Hack", ch);
					}
				}
			}
			ch->SetLastPickupTime();
		}
#else
		ch->PickupItem(pinfo->vid);
#endif
}

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
int CInputMain::ItemsPickup(LPCHARACTER ch, const char * c_pData, size_t uiBytes)
{
	TPacketCGItemsPickUp * p = (TPacketCGItemsPickUp *)c_pData;
	const int iExtraLen = p->count * sizeof(DWORD);
	if (uiBytes < sizeof(TPacketCGItemsPickUp) + iExtraLen)
		return -1;
	const DWORD pickupMode = ch->GetPickUPMode();
	if (IS_SET(pickupMode, AUTOMATIC_PICK_UP_ACTIVATE))
	{
		const DWORD *dwVIDS = (DWORD *)(c_pData + sizeof(TPacketCGItemsPickUp));
		for (int i = 0; i < p->count; i++)
		{
			const DWORD dwVID = *(dwVIDS + i);
			LPITEM item = ITEM_MANAGER::instance().FindByVID(dwVID);
			if (!item || !item->GetSectree() || !item->DistanceValid(ch, false))
				continue;
			const bool canGet = ch->CheckItemCanGet(item);
			if(canGet)
				ch->PickupItem(dwVID);
		}
		
		/*
		const DWORD *dwVIDS = (DWORD *)(c_pData + sizeof(TPacketCGItemsPickUp));
		std::map<LPITEM, LPCHARACTER> m_pickupItems; 
		for (int i = 0; i < p->count; i++)
		{
			const DWORD dwVID = *(dwVIDS + i);
			LPITEM item = ITEM_MANAGER::instance().FindByVID(dwVID);
			if (!item || !item->GetSectree() || !item->DistanceValid(ch))
				continue;
			LPCHARACTER itemOwner = ch;
			if(ch->CanGetGroundItem(item, itemOwner))
			{
				const bool canGet = itemOwner->CheckItemCanGet(item);
				if(canGet)
					m_pickupItems.emplace(item, itemOwner);
			}
		}
		for(auto it = m_pickupItems.begin();it != m_pickupItems.end();++it)
		{
			LPITEM item = it->first;
			LPSECTREE sectree = item->GetSectree();
			if(sectree)
			{
				item->SetOwnership(NULL);
				sectree->RemoveEntity(item);
				item->Save();
			}
		}
		for(auto it = m_pickupItems.begin();it != m_pickupItems.end();++it)
			it->first->ViewCleanup();

		for(auto it = m_pickupItems.begin();it != m_pickupItems.end();++it)
		{
			LPITEM item = it->first;
			LPCHARACTER itemOwner = it->second;
			if(item->GetVnum() == 1)
			{
				itemOwner->GiveGold(item->GetCount());
				M2_DESTROY_ITEM(item);
				itemOwner->Save();
			}
			else
			{
				itemOwner->ChatPacket(CHAT_TYPE_ITEM_INFO, "304 %s", item->GetName());
				itemOwner->AutoGiveItem(item);
			}
		}
		*/
	}
	return iExtraLen;
}
#endif
void CInputMain::QuickslotAdd(LPCHARACTER ch, const char * data)
{
	struct command_quickslot_add * pinfo = (struct command_quickslot_add *) data;
	ch->SetQuickslot(pinfo->pos, pinfo->slot);
}

void CInputMain::QuickslotDelete(LPCHARACTER ch, const char * data)
{
	struct command_quickslot_del * pinfo = (struct command_quickslot_del *) data;
	ch->DelQuickslot(pinfo->pos);
}

void CInputMain::QuickslotSwap(LPCHARACTER ch, const char * data)
{
	struct command_quickslot_swap * pinfo = (struct command_quickslot_swap *) data;
	ch->SwapQuickslot(pinfo->pos, pinfo->change_pos);
}

int CInputMain::Messenger(LPCHARACTER ch, const char* c_pData, size_t uiBytes)
{
	TPacketCGMessenger* p = (TPacketCGMessenger*) c_pData;

	if (uiBytes < sizeof(TPacketCGMessenger))
		return -1;

	c_pData += sizeof(TPacketCGMessenger);
	uiBytes -= sizeof(TPacketCGMessenger);

	switch (p->subheader)
	{
		case MESSENGER_SUBHEADER_CG_ADD_BY_VID:
			{
				if (uiBytes < sizeof(TPacketCGMessengerAddByVID))
					return -1;

				TPacketCGMessengerAddByVID * p2 = (TPacketCGMessengerAddByVID *) c_pData;
				LPCHARACTER ch_companion = CHARACTER_MANAGER::instance().Find(p2->vid);

				if (!ch_companion)
					return sizeof(TPacketCGMessengerAddByVID);

				if (ch->IsObserverMode())
					return sizeof(TPacketCGMessengerAddByVID);

				if (ch_companion->IsBlockMode(BLOCK_MESSENGER_INVITE))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "751");
					return sizeof(TPacketCGMessengerAddByVID);
				}

				LPDESC d = ch_companion->GetDesc();

				if (!d)
					return sizeof(TPacketCGMessengerAddByVID);

				if (ch->GetGMLevel() == GM_PLAYER && ch_companion->GetGMLevel() != GM_PLAYER)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "752");
					return sizeof(TPacketCGMessengerAddByVID);
				}
#ifdef ENABLE_MESSENGER_BLOCK
				if (MessengerManager::instance().IsBlocked(ch->GetName(), ch_companion->GetName()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "You need to unblock %s do to that.", ch_companion->GetName());
					return sizeof(TPacketCGMessengerAddByVID);
				}
				if (MessengerManager::instance().IsBlocked(ch_companion->GetName(), ch->GetName()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "%s has blocked you.", ch_companion->GetName());
					return sizeof(TPacketCGMessengerAddByVID);
				}
#endif
				if (ch->GetDesc() == d) // ÀÚ½ÅÀº Ãß°¡ÇÒ ¼ö ¾ø´Ù.
					return sizeof(TPacketCGMessengerAddByVID);

				MessengerManager::instance().RequestToAdd(ch, ch_companion);
				//MessengerManager::instance().AddToList(ch->GetName(), ch_companion->GetName());
			}
			return sizeof(TPacketCGMessengerAddByVID);

		case MESSENGER_SUBHEADER_CG_ADD_BY_NAME:
			{
				if (uiBytes < CHARACTER_NAME_MAX_LEN)
					return -1;

				char name[CHARACTER_NAME_MAX_LEN + 1];
				strlcpy(name, c_pData, sizeof(name));

				if (ch->GetGMLevel() == GM_PLAYER && gm_get_level(name) != GM_PLAYER)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "752");
					return CHARACTER_NAME_MAX_LEN;
				}

				LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(name);

				if (!tch)
					ch->ChatPacket(CHAT_TYPE_INFO, "753 %s", name);
				else
				{
					if (tch == ch) // ÀÚ½ÅÀº Ãß°¡ÇÒ ¼ö ¾ø´Ù.
						return CHARACTER_NAME_MAX_LEN;
						
#ifdef ENABLE_MESSENGER_BLOCK
					if (MessengerManager::instance().IsBlocked(ch->GetName(), tch->GetName()))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "You need to unblock %s do to that.", tch->GetName());
						return CHARACTER_NAME_MAX_LEN;
					}
					if (MessengerManager::instance().IsBlocked(tch->GetName(), ch->GetName()))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "%s has blocked you.", tch->GetName());
						return CHARACTER_NAME_MAX_LEN;
					}
#endif

					if (tch->IsBlockMode(BLOCK_MESSENGER_INVITE) == true)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "751");
					}
					else
					{
						// ¸Þ½ÅÀú°¡ Ä³¸¯ÅÍ´ÜÀ§°¡ µÇ¸é¼­ º¯°æ
						MessengerManager::instance().RequestToAdd(ch, tch);
						//MessengerManager::instance().AddToList(ch->GetName(), tch->GetName());
					}
				}
			}
			return CHARACTER_NAME_MAX_LEN;

		case MESSENGER_SUBHEADER_CG_REMOVE:
			{
				if (uiBytes < CHARACTER_NAME_MAX_LEN)
					return -1;

				char char_name[CHARACTER_NAME_MAX_LEN + 1];
				strlcpy(char_name, c_pData, sizeof(char_name));
				MessengerManager::instance().RemoveFromList(ch->GetName(), char_name);
			}
			return CHARACTER_NAME_MAX_LEN;
#ifdef ENABLE_MESSENGER_BLOCK
		case MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_VID:
		{
			if (uiBytes < sizeof(TPacketCGMessengerAddBlockByVID))
				return -1;

			TPacketCGMessengerAddBlockByVID* p2 = (TPacketCGMessengerAddBlockByVID*)c_pData;
			LPCHARACTER ch_companion = CHARACTER_MANAGER::instance().Find(p2->vid);

			if (!ch_companion)
				return sizeof(TPacketCGMessengerAddBlockByVID);

			if (ch->IsObserverMode())
				return sizeof(TPacketCGMessengerAddBlockByVID);

			if (ch_companion == ch)
				return sizeof(TPacketCGMessengerAddBlockByVID);

			if(!ch_companion->IsPC())
				return sizeof(TPacketCGMessengerAddBlockByVID);

			LPDESC d = ch_companion->GetDesc();

			if (!d)
				return sizeof(TPacketCGMessengerAddByVID);

			LPCHARACTER pkPartner = ch->GetMarryPartner();
			if (pkPartner)
			{
				if (strcmp(ch_companion->GetName(), pkPartner->GetName()) == 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "You can't block your spouse.");
					return sizeof(TPacketCGMessengerAddBlockByVID);
				}
			}

			if (MessengerManager::instance().IsInFriendList(ch->GetName(), ch_companion->GetName()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Remove %s from your friends list to continue.", ch_companion->GetName());
				return sizeof(TPacketCGMessengerAddBlockByVID);
			}

			if (MessengerManager::instance().IsBlocked(ch->GetName(), ch_companion->GetName()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "%s is already blocked.", ch_companion->GetName());
				return sizeof(TPacketCGMessengerAddBlockByVID);
			}

			if (ch->GetGMLevel() == GM_PLAYER && ch_companion->GetGMLevel() != GM_PLAYER && !test_server)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "You can't block this player.");
				return sizeof(TPacketCGMessengerAddByVID);
			}

			if (thecore_pulse() > ch->GetBlockAntiFloodPulse() + PASSES_PER_SEC(1))
			{
				ch->SetBlockAntiFloodCount(0);
				ch->SetBlockAntiFloodPulse(thecore_pulse());
			}

			if (ch->IncreaseBlockAntiFloodCount() >= 5)
			{
				// ch->GetDesc()->SetDisconnectReason("MESSENGER_BLOCK_SPAM");
				ch->GetDesc()->SetPhase(PHASE_CLOSE);
				return sizeof(TPacketCGMessengerAddByVID);
			}

			MessengerManager::instance().AddToBlockList(ch->GetName(), ch_companion->GetName());
		}
		return sizeof(TPacketCGMessengerAddBlockByVID);

		case MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_NAME:
		{
			if (uiBytes < CHARACTER_NAME_MAX_LEN)
				return -1;

			char name[CHARACTER_NAME_MAX_LEN + 1];
			strlcpy(name, c_pData, sizeof(name));

			if (gm_get_level(name) != GM_PLAYER && !test_server)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "You can't block this player.");
				return CHARACTER_NAME_MAX_LEN;
			}

			LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindPC(name);
			if (pkChr == ch)
				return CHARACTER_NAME_MAX_LEN;
						
			LPDESC pkDesc = nullptr;
			if (!pkChr)
			{
				CCI* pkCCI = P2P_MANAGER::instance().Find(name);
				if (pkCCI)
					pkDesc = pkCCI->pkDesc;
			}
			else
				pkDesc = pkChr->GetDesc();

			if (!pkDesc)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "%s is not online.", name);
				return CHARACTER_NAME_MAX_LEN;
			}

			LPCHARACTER pkPartner = ch->GetMarryPartner();
			if (pkPartner)
			{
				if (strcmp(pkPartner->GetName(), name) == 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "You can't block your spouse.");
					return CHARACTER_NAME_MAX_LEN;
				}
			}

			if (MessengerManager::instance().IsInFriendList(ch->GetName(), name))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Remove %s from your friends list to continue.", name);
				return CHARACTER_NAME_MAX_LEN;
			}

			if (MessengerManager::instance().IsBlocked(ch->GetName(), name))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "%s is already blocked.", name);
				return CHARACTER_NAME_MAX_LEN;
			}

			if (thecore_pulse() > ch->GetBlockAntiFloodPulse() + PASSES_PER_SEC(1))
			{
				ch->SetBlockAntiFloodCount(0);
				ch->SetBlockAntiFloodPulse(thecore_pulse());
			}

			if (ch->IncreaseBlockAntiFloodCount() >= 5)
			{
				// ch->GetDesc()->SetDisconnectReason("MESSENGER_BLOCK_SPAM");
				ch->GetDesc()->SetPhase(PHASE_CLOSE);
				return CHARACTER_NAME_MAX_LEN;
			}

			MessengerManager::instance().AddToBlockList(ch->GetName(), name);
		}
		return CHARACTER_NAME_MAX_LEN;

		case MESSENGER_SUBHEADER_CG_BLOCK_REMOVE:
		{
			if (uiBytes < CHARACTER_NAME_MAX_LEN)
				return -1;

			char char_name[CHARACTER_NAME_MAX_LEN + 1];
			strlcpy(char_name, c_pData, sizeof(char_name));

			if (!MessengerManager::instance().IsBlocked(ch->GetName(), char_name))
				return CHARACTER_NAME_MAX_LEN;

			if (thecore_pulse() > ch->GetBlockAntiFloodPulse() + PASSES_PER_SEC(1))
			{
				ch->SetBlockAntiFloodCount(0);
				ch->SetBlockAntiFloodPulse(thecore_pulse());
			}

			if (ch->IncreaseBlockAntiFloodCount() >= 5)
			{
				// ch->GetDesc()->SetDisconnectReason("MESSENGER_BLOCK_SPAM");
				ch->GetDesc()->SetPhase(PHASE_CLOSE);
				return CHARACTER_NAME_MAX_LEN;
			}

			MessengerManager::instance().RemoveFromBlockList(ch->GetName(), char_name);
		}
		return CHARACTER_NAME_MAX_LEN;

		case MESSENGER_SUBHEADER_CG_BLOCK_REMOVE_BY_VID:
		{
			if (uiBytes < sizeof(TPacketCGMessengerRemoveBlockByVID))
				return -1;

			TPacketCGMessengerRemoveBlockByVID* p2 = (TPacketCGMessengerRemoveBlockByVID*)c_pData;
			LPCHARACTER ch_companion = CHARACTER_MANAGER::instance().Find(p2->vid);
			if (!ch_companion)
				return sizeof(TPacketCGMessengerRemoveBlockByVID);

			if (ch->IsObserverMode())
				return sizeof(TPacketCGMessengerRemoveBlockByVID);

			if (ch_companion == ch || !ch_companion->IsPC() || !ch_companion->GetDesc())
				return sizeof(TPacketCGMessengerRemoveBlockByVID);

			if (!MessengerManager::instance().IsBlocked(ch->GetName(), ch_companion->GetName()))
				return sizeof(TPacketCGMessengerRemoveBlockByVID);

			if (thecore_pulse() > ch->GetBlockAntiFloodPulse() + PASSES_PER_SEC(1))
			{
				ch->SetBlockAntiFloodCount(0);
				ch->SetBlockAntiFloodPulse(thecore_pulse());
			}

			if (ch->IncreaseBlockAntiFloodCount() >= 5)
			{
				// ch->GetDesc()->SetDisconnectReason("MESSENGER_BLOCK_SPAM");
				ch->GetDesc()->SetPhase(PHASE_CLOSE);
				return sizeof(TPacketCGMessengerRemoveBlockByVID);
			}

			MessengerManager::instance().RemoveFromBlockList(ch->GetName(), ch_companion->GetName());
		}
		return sizeof(TPacketCGMessengerRemoveBlockByVID);
#endif

		default:
			sys_err("CInputMain::Messenger : Unknown subheader %d : %s", p->subheader, ch->GetName());
			break;
	}

	return 0;
}




int CInputMain::Shop(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	TPacketCGShop * p = (TPacketCGShop *) data;

	if (uiBytes < sizeof(TPacketCGShop))
		return -1;

	if (test_server)
		sys_log(0, "CInputMain::Shop() ==> SubHeader %d", p->subheader);

	const char * c_pData = data + sizeof(TPacketCGShop);
	uiBytes -= sizeof(TPacketCGShop);

	switch (p->subheader)
	{
		case SHOP_SUBHEADER_CG_END:
			sys_log(1, "INPUT: %s SHOP: END", ch->GetName());
			CShopManager::instance().StopShopping(ch);
			return 0;

		case SHOP_SUBHEADER_CG_BUY:
			{
				if (uiBytes < sizeof(BYTE) + sizeof(BYTE))
					return -1;

				BYTE bPos = *(c_pData + 1);
				sys_log(1, "INPUT: %s SHOP: BUY %d", ch->GetName(), bPos);
				CShopManager::instance().Buy(ch, bPos);
				return (sizeof(BYTE) + sizeof(BYTE));
			}

		case SHOP_SUBHEADER_CG_SELL:
			{
				if (uiBytes < sizeof(WORD))
					return -1;

				WORD pos = *c_pData;

				sys_log(0, "INPUT: %s SHOP: SELL", ch->GetName());
				#ifndef ENABLE_SPECIAL_STORAGE
				CShopManager::instance().Sell(ch, pos);
				#endif
				return sizeof(WORD);
			}


#ifdef ENABLE_SPECIAL_STORAGE
		case SHOP_SUBHEADER_CG_SELL2:
			{
				if (uiBytes < sizeof(BYTE) + sizeof(WORD) + sizeof(DWORD))
					return -1;

				const BYTE wInv = *reinterpret_cast<const BYTE*>(c_pData);
				c_pData+=  sizeof(BYTE);
				const WORD wPos = *reinterpret_cast<const WORD*>(c_pData);
				c_pData+=  sizeof(WORD);
				const DWORD bCount = *reinterpret_cast<const DWORD*>(c_pData);

				sys_log(0, "INPUT: %s SHOP: SELL2", ch->GetName());
				CShopManager::instance().Sell(ch, wInv, wPos, bCount);
				return sizeof(BYTE) + sizeof(WORD) + sizeof(DWORD);
			}
#else
		case SHOP_SUBHEADER_CG_SELL2:
			{
				if (uiBytes < sizeof(WORD) + sizeof(DWORD))
					return -1;

				WORD pos = *(c_pData++);
				DWORD count = *(c_pData);

				sys_log(0, "INPUT: %s SHOP: SELL2", ch->GetName());
				CShopManager::instance().Sell(ch, pos, count);
				return sizeof(WORD) + sizeof(DWORD);
			}
#endif


		default:
			sys_err("CInputMain::Shop : Unknown subheader %d : %s", p->subheader, ch->GetName());
			break;
	}

	return 0;
}

void CInputMain::OnClick(LPCHARACTER ch, const char * data)
{
	struct command_on_click *	pinfo = (struct command_on_click *) data;
	LPCHARACTER			victim;

	if ((victim = CHARACTER_MANAGER::instance().Find(pinfo->vid)))
		victim->OnClick(ch);
	else if (test_server)
	{
		sys_err("CInputMain::OnClick %s.Click.NOT_EXIST_VID[%d]", ch->GetName(), pinfo->vid);
	}
}

void CInputMain::Exchange(LPCHARACTER ch, const char * data)
{
	struct command_exchange * pinfo = (struct command_exchange *) data;
	LPCHARACTER	to_ch = NULL;

	if (!ch->CanHandleItem())
		return;

#if defined(__BL_BATTLE_ROYALE__)
	if (ch->IsBattleRoyale())
		return;
#endif

	int iPulse = thecore_pulse();

	if ((to_ch = CHARACTER_MANAGER::instance().Find(pinfo->arg1)))
	{
		if (iPulse - to_ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			to_ch->ChatPacket(CHAT_TYPE_INFO, "754 %d", g_nPortalLimitTime);
			return;
		}

		if( true == to_ch->IsDead() )
		{
			return;
		}
	}

	sys_log(0, "CInputMain()::Exchange()  SubHeader %d ", pinfo->sub_header);

	if (iPulse - ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "754 %d", g_nPortalLimitTime);
		return;
	}


	switch (pinfo->sub_header)
	{
		case EXCHANGE_SUBHEADER_START:	// arg1 == vid of target character
			if (!ch->GetExchange())
			{
				if ((to_ch = CHARACTER_MANAGER::instance().Find(pinfo->arg1)))
				{
					//MONARCH_LIMIT
					/*
					if (to_ch->IsMonarch() || ch->IsMonarch())
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "755", g_nPortalLimitTime);
						return;
					}
					//END_MONARCH_LIMIT
					*/
					if (iPulse - ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "756 %d", g_nPortalLimitTime);

						if (test_server)
							ch->ChatPacket(CHAT_TYPE_INFO, "[TestOnly][Safebox]Pulse %d LoadTime %d PASS %d", iPulse, ch->GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
						return;
					}

					if (iPulse - to_ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
					{
						to_ch->ChatPacket(CHAT_TYPE_INFO, "756 %d", g_nPortalLimitTime);


						if (test_server)
							to_ch->ChatPacket(CHAT_TYPE_INFO, "[TestOnly][Safebox]Pulse %d LoadTime %d PASS %d", iPulse, to_ch->GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
						return;
					}

					if (to_ch->IsPC())
					{
						if (quest::CQuestManager::instance().GiveItemToPC(ch->GetPlayerID(), to_ch))
						{
							sys_log(0, "Exchange canceled by quest %s %s", ch->GetName(), to_ch->GetName());
							return;
						}
					}


					if (ch->IsOpenSafebox()
						 || ch->GetShop()
						 || ch->IsCubeOpen()
#ifdef __BL_67_ATTR__
						|| ch->Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
						|| ch->IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
						 || ch->isSashOpened(true)
						 || ch->isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
						|| ch->IsShardCraftOpened()
#endif
						)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "758");
						return;
					}

					ch->ExchangeStart(to_ch);
				}
			}
			break;

		case EXCHANGE_SUBHEADER_ITEM_ADD:	// arg1 == position of item, arg2 == position in exchange window
			if (ch->GetExchange())
			{
				if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
					ch->GetExchange()->AddItem(pinfo->Pos, pinfo->arg2);
			}
			break;

		case EXCHANGE_SUBHEADER_ITEM_DEL:	// arg1 == position of item
			if (ch->GetExchange())
			{
				if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
					ch->GetExchange()->RemoveItem(pinfo->arg1);
			}
			break;
		case EXCHANGE_SUBHEADER_GOLD_ADD:
#ifdef USE_CHEQUE_CURRENCY
		case EXCHANGE_SUBHEADER_CHEQUE_ADD:
#endif
		{
			auto selfExchange = ch ? ch->GetExchange() : nullptr;
			if (selfExchange)
			{
				auto targetExchange = selfExchange->GetCompany();
				if (!targetExchange->GetAcceptStatus())
				{
#ifdef USE_CHEQUE_CURRENCY
					if (pinfo->sub_header == EXCHANGE_SUBHEADER_CHEQUE_ADD)
					{
						selfExchange->AddCheque(pinfo->arg1);
					}
					else
					{
#endif
					selfExchange->AddGold(pinfo->arg1);
#ifdef USE_CHEQUE_CURRENCY
					}
#endif
				}
			}

			break;
		}
		case EXCHANGE_SUBHEADER_ACCEPT:	// arg1 == not used
			if (ch->GetExchange())
			{
				sys_log(0, "CInputMain()::Exchange() ==> ACCEPT ");
				ch->GetExchange()->Accept(true);
			}

			break;

		case EXCHANGE_SUBHEADER_END:	// arg1 == not used
			if (ch->GetExchange())
				ch->GetExchange()->Cancel();
			break;
	}
}

void CInputMain::Position(LPCHARACTER ch, const char * data)
{
	struct command_position * pinfo = (struct command_position *) data;

	switch (pinfo->position)
	{
		case POSITION_GENERAL:
			ch->Standup();
			break;

		case POSITION_SITTING_CHAIR:
			ch->Sitdown(0);
			break;

		case POSITION_SITTING_GROUND:
			ch->Sitdown(1);
			break;
	}
}

static const int ComboSequenceBySkillLevel[3][8] =
{
	// 0   1   2   3   4   5   6   7
	{ 14, 15, 16, 17,  0,  0,  0,  0 },
	{ 14, 15, 16, 18, 20,  0,  0,  0 },
	{ 14, 15, 16, 18, 19, 17,  0,  0 },
};

#define COMBO_HACK_ALLOWABLE_MS	100

bool CheckComboHack(LPCHARACTER ch, BYTE bArg, DWORD dwTime, bool CheckSpeedHack)
{
	if(!gHackCheckEnable) return false;

	//	Á×°Å³ª ±âÀý »óÅÂ¿¡¼­´Â °ø°ÝÇÒ ¼ö ¾øÀ¸¹Ç·Î, skipÇÑ´Ù.
	//	ÀÌ·¸°Ô ÇÏÁö ¸»°í, CHRACTER::CanMove()¿¡
	//	if (IsStun() || IsDead()) return false;
	//	¸¦ Ãß°¡ÇÏ´Â°Ô ¸Â´Ù°í »ý°¢ÇÏ³ª,
	//	ÀÌ¹Ì ´Ù¸¥ ºÎºÐ¿¡¼­ CanMove()´Â IsStun(), IsDead()°ú
	//	µ¶¸³ÀûÀ¸·Î Ã¼Å©ÇÏ°í ÀÖ±â ¶§¹®¿¡ ¼öÁ¤¿¡ ÀÇÇÑ ¿µÇâÀ»
	//	ÃÖ¼ÒÈ­ÇÏ±â À§ÇØ ÀÌ·¸°Ô ¶«»§ ÄÚµå¸¦ ½á³õ´Â´Ù.
	if (ch->IsStun() || ch->IsDead())
		return false;
	int ComboInterval = dwTime - ch->GetLastComboTime();
	int HackScalar = 0; // ±âº» ½ºÄ®¶ó ´ÜÀ§ 1
#if 0
	sys_log(0, "COMBO: %s arg:%u seq:%u delta:%d checkspeedhack:%d",
			ch->GetName(), bArg, ch->GetComboSequence(), ComboInterval - ch->GetValidComboInterval(), CheckSpeedHack);
#endif
	// bArg 14 ~ 21¹ø ±îÁö ÃÑ 8ÄÞº¸ °¡´É
	// 1. Ã¹ ÄÞº¸(14)´Â ÀÏÁ¤ ½Ã°£ ÀÌÈÄ¿¡ ¹Ýº¹ °¡´É
	// 2. 15 ~ 21¹øÀº ¹Ýº¹ ºÒ°¡´É
	// 3. Â÷·Ê´ë·Î Áõ°¡ÇÑ´Ù.
	if (bArg == 14)
	{
		if (CheckSpeedHack && ComboInterval > 0 && ComboInterval < ch->GetValidComboInterval() - COMBO_HACK_ALLOWABLE_MS)
		{
			// FIXME Ã¹¹øÂ° ÄÞº¸´Â ÀÌ»óÇÏ°Ô »¡¸® ¿Ã ¼ö°¡ ÀÖ¾î¼­ 300À¸·Î ³ª´® -_-;

			// ´Ù¼öÀÇ ¸ó½ºÅÍ¿¡ ÀÇÇØ ´Ù¿îµÇ´Â »óÈ²¿¡¼­ °ø°ÝÀ» ÇÏ¸é
			// Ã¹¹øÂ° ÄÞº¸°¡ ¸Å¿ì ÀûÀº ÀÎÅÍ¹ú·Î µé¾î¿À´Â »óÈ² ¹ß»ý.
			// ÀÌ·Î ÀÎÇØ ÄÞº¸ÇÙÀ¸·Î Æ¨±â´Â °æ¿ì°¡ ÀÖ¾î ´ÙÀ½ ÄÚµå ºñ È°¼ºÈ­.
			//HackScalar = 1 + (ch->GetValidComboInterval() - ComboInterval) / 300;

			//sys_log(0, "COMBO_HACK: 2 %s arg:%u interval:%d valid:%u atkspd:%u riding:%s",
			//		ch->GetName(),
			//		bArg,
			//		ComboInterval,
			//		ch->GetValidComboInterval(),
			//		ch->GetPoint(POINT_ATT_SPEED),
			//		ch->IsRiding() ? "yes" : "no");
		}

		ch->SetComboSequence(1);
		ch->SetValidComboInterval((int) (ani_combo_speed(ch, 1) / (ch->GetPoint(POINT_ATT_SPEED) / 100.f)));
		ch->SetLastComboTime(dwTime);
	}
	else if (bArg > 14 && bArg < 22)
	{
		int idx = MIN(2, ch->GetComboIndex());

		if (ch->GetComboSequence() > 5) // ÇöÀç 6ÄÞº¸ ÀÌ»óÀº ¾ø´Ù.
		{
			HackScalar = 1;
			ch->SetValidComboInterval(300);
			sys_log(0, "COMBO_HACK: 5 %s combo_seq:%d", ch->GetName(), ch->GetComboSequence());
		}
		// ÀÚ°´ ½Ö¼ö ÄÞº¸ ¿¹¿ÜÃ³¸®
		else if (bArg == 21 &&
				 idx == 2 &&
				 ch->GetComboSequence() == 5 &&
				 ch->GetJob() == JOB_ASSASSIN &&
				 ch->GetWear(WEAR_WEAPON) &&
				 ch->GetWear(WEAR_WEAPON)->GetSubType() == WEAPON_DAGGER)
			ch->SetValidComboInterval(300);
#ifdef ENABLE_WOLFMAN_CHARACTER
		else if (bArg == 21 && idx == 2 && ch->GetComboSequence() == 5 && ch->GetJob() == JOB_WOLFMAN && ch->GetWear(WEAR_WEAPON) && ch->GetWear(WEAR_WEAPON)->GetSubType() == WEAPON_CLAW)
			ch->SetValidComboInterval(300);
#endif
		else if (ComboSequenceBySkillLevel[idx][ch->GetComboSequence()] != bArg)
		{
			HackScalar = 1;
			ch->SetValidComboInterval(300);

			sys_log(0, "COMBO_HACK: 3 %s arg:%u valid:%u combo_idx:%d combo_seq:%d",
					ch->GetName(),
					bArg,
					ComboSequenceBySkillLevel[idx][ch->GetComboSequence()],
					idx,
					ch->GetComboSequence());
		}
		else
		{
			if (CheckSpeedHack && ComboInterval < ch->GetValidComboInterval() - COMBO_HACK_ALLOWABLE_MS)
			{
				HackScalar = 1 + (ch->GetValidComboInterval() - ComboInterval) / 100;

				sys_log(0, "COMBO_HACK: 2 %s arg:%u interval:%d valid:%u atkspd:%u riding:%s",
						ch->GetName(),
						bArg,
						ComboInterval,
						ch->GetValidComboInterval(),
						ch->GetPoint(POINT_ATT_SPEED),
						ch->IsRiding() ? "yes" : "no");
			}

			// ¸»À» ÅÀÀ» ¶§´Â 15¹ø ~ 16¹øÀ» ¹Ýº¹ÇÑ´Ù
			//if (ch->IsHorseRiding())
			if (ch->IsRiding())
				ch->SetComboSequence(ch->GetComboSequence() == 1 ? 2 : 1);
			else
				ch->SetComboSequence(ch->GetComboSequence() + 1);

			ch->SetValidComboInterval((int) (ani_combo_speed(ch, bArg - 13) / (ch->GetPoint(POINT_ATT_SPEED) / 100.f)));
			ch->SetLastComboTime(dwTime);
		}
	}
	else if (bArg == 13) // ±âº» °ø°Ý (µÐ°©(Polymorph)ÇßÀ» ¶§ ¿Â´Ù)
	{
		if (CheckSpeedHack && ComboInterval > 0 && ComboInterval < ch->GetValidComboInterval() - COMBO_HACK_ALLOWABLE_MS)
		{
			// ´Ù¼öÀÇ ¸ó½ºÅÍ¿¡ ÀÇÇØ ´Ù¿îµÇ´Â »óÈ²¿¡¼­ °ø°ÝÀ» ÇÏ¸é
			// Ã¹¹øÂ° ÄÞº¸°¡ ¸Å¿ì ÀûÀº ÀÎÅÍ¹ú·Î µé¾î¿À´Â »óÈ² ¹ß»ý.
			// ÀÌ·Î ÀÎÇØ ÄÞº¸ÇÙÀ¸·Î Æ¨±â´Â °æ¿ì°¡ ÀÖ¾î ´ÙÀ½ ÄÚµå ºñ È°¼ºÈ­.
			//HackScalar = 1 + (ch->GetValidComboInterval() - ComboInterval) / 100;

			//sys_log(0, "COMBO_HACK: 6 %s arg:%u interval:%d valid:%u atkspd:%u",
			//		ch->GetName(),
			//		bArg,
			//		ComboInterval,
			//		ch->GetValidComboInterval(),
			//		ch->GetPoint(POINT_ATT_SPEED));
		}

		if (ch->GetRaceNum() >= MAIN_RACE_MAX_NUM)
		{
			// POLYMORPH_BUG_FIX

			// DELETEME
			/*
			const CMotion * pkMotion = CMotionManager::instance().GetMotion(ch->GetRaceNum(), MAKE_MOTION_KEY(MOTION_MODE_GENERAL, MOTION_NORMAL_ATTACK));

			if (!pkMotion)
				sys_err("cannot find motion by race %u", ch->GetRaceNum());
			else
			{
				// Á¤»óÀû °è»êÀÌ¶ó¸é 1000.f¸¦ °öÇØ¾ß ÇÏÁö¸¸ Å¬¶óÀÌ¾ðÆ®°¡ ¾Ö´Ï¸ÞÀÌ¼Ç ¼ÓµµÀÇ 90%¿¡¼­
				// ´ÙÀ½ ¾Ö´Ï¸ÞÀÌ¼Ç ºí·»µùÀ» Çã¿ëÇÏ¹Ç·Î 900.f¸¦ °öÇÑ´Ù.
				int k = (int) (pkMotion->GetDuration() / ((float) ch->GetPoint(POINT_ATT_SPEED) / 100.f) * 900.f);
				ch->SetValidComboInterval(k);
				ch->SetLastComboTime(dwTime);
			}
			*/
			float normalAttackDuration = CMotionManager::instance().GetNormalAttackDuration(ch->GetRaceNum());
			int k = (int) (normalAttackDuration / ((float) ch->GetPoint(POINT_ATT_SPEED) / 100.f) * 900.f);
			ch->SetValidComboInterval(k);
			ch->SetLastComboTime(dwTime);
			// END_OF_POLYMORPH_BUG_FIX
		}
		else
		{
			// ¸»ÀÌ ¾ÈµÇ´Â ÄÞº¸°¡ ¿Ô´Ù ÇØÄ¿ÀÏ °¡´É¼º?
			//if (ch->GetDesc()->DelayedDisconnect(number(2, 9)))
			//{
			//	LogManager::instance().HackLog("Hacker", ch);
			//	sys_log(0, "HACKER: %s arg %u", ch->GetName(), bArg);
			//}

			// À§ ÄÚµå·Î ÀÎÇØ, Æú¸®¸ðÇÁ¸¦ Çª´Â Áß¿¡ °ø°Ý ÇÏ¸é,
			// °¡²û ÇÙÀ¸·Î ÀÎ½ÄÇÏ´Â °æ¿ì°¡ ÀÖ´Ù.

			// ÀÚ¼¼È÷ ¸»Çô¸é,
			// ¼­¹ö¿¡¼­ poly 0¸¦ Ã³¸®ÇßÁö¸¸,
			// Å¬¶ó¿¡¼­ ±× ÆÐÅ¶À» ¹Þ±â Àü¿¡, ¸÷À» °ø°Ý. <- Áï, ¸÷ÀÎ »óÅÂ¿¡¼­ °ø°Ý.
			//
			// ±×·¯¸é Å¬¶ó¿¡¼­´Â ¼­¹ö¿¡ ¸÷ »óÅÂ·Î °ø°ÝÇß´Ù´Â Ä¿¸Çµå¸¦ º¸³»°í (arg == 13)
			//
			// ¼­¹ö¿¡¼­´Â race´Â ÀÎ°£ÀÎµ¥ °ø°ÝÇüÅÂ´Â ¸÷ÀÎ ³ðÀÌ´Ù! ¶ó°í ÇÏ¿© ÇÙÃ¼Å©¸¦ Çß´Ù.

			// »ç½Ç °ø°Ý ÆÐÅÏ¿¡ ´ëÇÑ °ÍÀº Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÆÇ´ÜÇØ¼­ º¸³¾ °ÍÀÌ ¾Æ´Ï¶ó,
			// ¼­¹ö¿¡¼­ ÆÇ´ÜÇØ¾ß ÇÒ °ÍÀÎµ¥... ¿Ö ÀÌ·¸°Ô ÇØ³ùÀ»±î...
			// by rtsummit
		}
	}
	else
	{
		// ¸»ÀÌ ¾ÈµÇ´Â ÄÞº¸°¡ ¿Ô´Ù ÇØÄ¿ÀÏ °¡´É¼º?
		if (ch->GetDesc()->DelayedDisconnect(number(2, 9)))
		{
			LogManager::instance().HackLog("Hacker", ch);
			sys_log(0, "HACKER: %s arg %u", ch->GetName(), bArg);
		}

		HackScalar = 10;
		ch->SetValidComboInterval(300);
	}

	if (HackScalar)
	{
		// ¸»¿¡ Å¸°Å³ª ³»·ÈÀ» ¶§ 1.5ÃÊ°£ °ø°ÝÀº ÇÙÀ¸·Î °£ÁÖÇÏÁö ¾ÊµÇ °ø°Ý·ÂÀº ¾ø°Ô ÇÏ´Â Ã³¸®
		if (get_dword_time() - ch->GetLastMountTime() > 1500)
			ch->IncreaseComboHackCount(1 + HackScalar);

		ch->SkipComboAttackByTime(ch->GetValidComboInterval());
	}

	return HackScalar;


}

void CInputMain::Move(LPCHARACTER ch, const char * data)
{
	if (!ch->CanMove())
	{
		return;
	}

#ifdef USE_CAPTCHA_SYSTEM
	if (ch->IsWaitingForCaptcha())
	{
		return;
	}
#endif

	struct command_move * pinfo = (struct command_move *) data;

	if (pinfo->bFunc >= FUNC_MAX_NUM && !(pinfo->bFunc & 0x80))
	{
		sys_err("invalid move type: %s", ch->GetName());
		return;
	}

#ifdef ENABLE_CHECK_GHOSTMODE
	if (ch->IsPC() && ch->IsDead())
	{
		if (ch->GetGhostmodeCount() <= 10)
		{
			ch->AddGhostmodeCount();
		}
		else
		{
			LPDESC d = ch->GetDesc();
			if (d)
			{
				if (d->DelayedDisconnect(3))
				{
					LogManager::instance().HackLog("CShield-ServerSide-ErrorCode: Ghostmode", ch);
					ch->ResetGhostmodeCount();
				}
			}
			return;
		}
	}
	else
	{
		ch->ResetGhostmodeCount();
	}
#endif

#ifdef ENABLE_CHECK_WALLHACK
	if (ch->IsPC())
	{
		if (!SECTREE_MANAGER::instance().IsMovablePosition(ch->GetMapIndex(), pinfo->lX, pinfo->lY))
		{
			if (ch->GetWallhackCount() <= 5)
			{
				ch->AddWallhackCount();
			}
			else
			{
				LPDESC d = ch->GetDesc();
				if (d)
				{
					if (d->DelayedDisconnect(3))
					{
						LogManager::instance().HackLog("CShield-ServerSide-ErrorCode: Wallhack", ch);
						ch->ResetWallhackCount();
					}
				}
				return;
			}
		}
		else
		{
			ch->ResetWallhackCount();
		}
	}
#endif

	//enum EMoveFuncType
	//{
	//	FUNC_WAIT,
	//	FUNC_MOVE,
	//	FUNC_ATTACK,
	//	FUNC_COMBO,
	//	FUNC_MOB_SKILL,
	//	_FUNC_SKILL,
	//	FUNC_MAX_NUM,
	//	FUNC_SKILL = 0x80,
	//};

	// ÅÚ·¹Æ÷Æ® ÇÙ Ã¼Å©

//	if (!test_server)
	{
		const float fDist = DISTANCE_SQRT((ch->GetX() - pinfo->lX) / 100, (ch->GetY() - pinfo->lY) / 100);
		// @fixme106 (changed 40 to 60)
		if (((false == ch->IsRiding() && fDist > 150) || fDist > 150) && OXEVENT_MAP_INDEX != ch->GetMapIndex())
		{
#ifdef ENABLE_HACK_TELEPORT_LOG // @warme006
			{
				const PIXEL_POSITION & warpPos = ch->GetWarpPosition();

				if (warpPos.x == 0 && warpPos.y == 0)
					LogManager::instance().HackLog("Teleport", ch); // ºÎÁ¤È®ÇÒ ¼ö ÀÖÀ½
			}
#endif
			sys_log(0, "MOVE: %s trying to move too far (dist: %.1fm) Riding(%d)", ch->GetName(), fDist, ch->IsRiding());

			ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
			ch->Stop();
			return;
		}
#ifdef ENABLE_CHECK_GHOSTMODE
		if (ch->IsPC() && ch->IsDead())
		{
			sys_log(0, "MOVE: %s trying to move as dead", ch->GetName());

			ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
			ch->Stop();
			return;
		}
#endif
		//
		// ½ºÇÇµåÇÙ(SPEEDHACK) Check
		//
		DWORD dwCurTime = get_dword_time();
		// ½Ã°£À» SyncÇÏ°í 7ÃÊ ÈÄ ºÎÅÍ °Ë»çÇÑ´Ù. (20090702 ÀÌÀü¿£ 5ÃÊ¿´À½)
		bool CheckSpeedHack = (false == ch->GetDesc()->IsHandshaking() && dwCurTime - ch->GetDesc()->GetClientTime() > 7000);

		if (CheckSpeedHack)
		{
			int iDelta = (int) (pinfo->dwTime - ch->GetDesc()->GetClientTime());
			int iServerDelta = (int) (dwCurTime - ch->GetDesc()->GetClientTime());

			iDelta = (int) (dwCurTime - pinfo->dwTime);

			// ½Ã°£ÀÌ ´Ê°Ô°£´Ù. ÀÏ´Ü ·Î±×¸¸ ÇØµÐ´Ù. ÁøÂ¥ ÀÌ·± »ç¶÷µéÀÌ ¸¹ÀºÁö Ã¼Å©ÇØ¾ßÇÔ. TODO
			if (iDelta >= 30000)
			{
				sys_log(0, "SPEEDHACK: slow timer name %s delta %d", ch->GetName(), iDelta);
				ch->GetDesc()->DelayedDisconnect(3);
			}
			// 1ÃÊ¿¡ 20msec »¡¸® °¡´Â°Å ±îÁö´Â ÀÌÇØÇÑ´Ù.
			else if (iDelta < -(iServerDelta / 50))
			{
				sys_log(0, "SPEEDHACK: DETECTED! %s (delta %d %d)", ch->GetName(), iDelta, iServerDelta);
				ch->GetDesc()->DelayedDisconnect(3);
			}
		}

		//
		// ÄÞº¸ÇÙ ¹× ½ºÇÇµåÇÙ Ã¼Å©
		//
		if (pinfo->bFunc == FUNC_COMBO && g_bCheckMultiHack)
		{
			CheckComboHack(ch, pinfo->bArg, pinfo->dwTime, CheckSpeedHack); // ÄÞº¸ Ã¼Å©
		}
	}

	if (pinfo->bFunc == FUNC_MOVE)
	{
		if (ch->GetLimitPoint(POINT_MOV_SPEED) == 0)
			return;

		ch->SetRotation(pinfo->bRot * 5);	// Áßº¹ ÄÚµå
		ch->ResetStopTime();				// ""

		ch->Goto(pinfo->lX, pinfo->lY);
	}
	else
	{
		if (pinfo->bFunc == FUNC_ATTACK || pinfo->bFunc == FUNC_COMBO)
			ch->OnMove(true);
		else if (pinfo->bFunc & FUNC_SKILL)
		{
			const int MASK_SKILL_MOTION = 0x7F;
			unsigned int motion = pinfo->bFunc & MASK_SKILL_MOTION;

			if (!ch->IsUsableSkillMotion(motion))
			{
				const char* name = ch->GetName();
				unsigned int job = ch->GetJob();
				unsigned int group = ch->GetSkillGroup();

				char szBuf[256];
				snprintf(szBuf, sizeof(szBuf), "SKILL_HACK: name=%s, job=%d, group=%d, motion=%d", name, job, group, motion);
				LogManager::instance().HackLog(szBuf, ch->GetDesc()->GetAccountTable().login, ch->GetName(), ch->GetDesc()->GetHostName());
				sys_log(0, "%s", szBuf);

				if (test_server)
				{
					ch->GetDesc()->DelayedDisconnect(number(2, 8));
					ch->ChatPacket(CHAT_TYPE_INFO, szBuf);
				}
				else
				{
					ch->GetDesc()->DelayedDisconnect(number(150, 500));
				}
			}

			ch->OnMove();
		}

		ch->SetRotation(pinfo->bRot * 5);	// Áßº¹ ÄÚµå
		ch->ResetStopTime();				// ""

		ch->Move(pinfo->lX, pinfo->lY);
		ch->Stop();
		ch->StopStaminaConsume();
	}

	TPacketGCMove pack;

	pack.bHeader	  = HEADER_GC_MOVE;
	pack.bFunc		= pinfo->bFunc;
	pack.bArg		 = pinfo->bArg;
	pack.bRot		 = pinfo->bRot;
	pack.dwVID		= ch->GetVID();
	pack.lX		   = pinfo->lX;
	pack.lY		   = pinfo->lY;
	pack.dwTime	   = pinfo->dwTime;
	pack.dwDuration   = (pinfo->bFunc == FUNC_MOVE) ? ch->GetCurrentMoveDuration() : 0;

	ch->PacketAround(&pack, sizeof(TPacketGCMove), ch);
/*
	if (pinfo->dwTime == 10653691) // µð¹ö°Å ¹ß°ß
	{
		if (ch->GetDesc()->DelayedDisconnect(number(15, 30)))
			LogManager::instance().HackLog("Debugger", ch);

	}
	else if (pinfo->dwTime == 10653971) // Softice ¹ß°ß
	{
		if (ch->GetDesc()->DelayedDisconnect(number(15, 30)))
			LogManager::instance().HackLog("Softice", ch);
	}
*/
	/*
	sys_log(0,
			"MOVE: %s Func:%u Arg:%u Pos:%dx%d Time:%u Dist:%.1f",
			ch->GetName(),
			pinfo->bFunc,
			pinfo->bArg,
			pinfo->lX / 100,
			pinfo->lY / 100,
			pinfo->dwTime,
			fDist);
	*/
}

void CInputMain::Attack(LPCHARACTER ch, const BYTE header, const char* data)
{
	if (!ch)
	{
		return;
	}

#ifdef USE_CAPTCHA_SYSTEM
	if (ch->IsWaitingForCaptcha())
	{
		return;
	}
#endif

	struct type_identifier
	{
		BYTE header;
		BYTE type;
	};

	const struct type_identifier* const type = reinterpret_cast<const struct type_identifier*>(data);

	if (type->type > 0)
	{
#if defined(__BL_BATTLE_ROYALE__)
		if (ch->IsBattleRoyale())
			return;
#endif

		if (false == ch->CanUseSkill(type->type))
		{
			return;
		}

		switch (type->type)
		{
			case SKILL_GEOMPUNG:
			case SKILL_SANGONG:
			case SKILL_YEONSA:
			case SKILL_KWANKYEOK:
			case SKILL_HWAJO:
			case SKILL_GIGUNG:
			case SKILL_PABEOB:
			case SKILL_MARYUNG:
			case SKILL_TUSOK:
			case SKILL_MAHWAN:
			case SKILL_BIPABU:
			case SKILL_PAERYONG:
			case SKILL_NOEJEON:
			case SKILL_CHAIN:
			case SKILL_HORSE_WILDATTACK_RANGE:
#ifdef ENABLE_YOHARA_SYSTEM
			case SKILL_ILGWANGPYO:
			case SKILL_MABEOBAGGWI:
			case SKILL_METEO:
#endif
				if (HEADER_CG_SHOOT != type->header)
				{
					if (test_server)
						ch->ChatPacket(CHAT_TYPE_INFO, "760 %s %d", type->type);
					return;
				}
				break;
		}
	}

	switch (header)
	{
		case HEADER_CG_ATTACK:
			{
				if (NULL == ch->GetDesc())
					return;

				const TPacketCGAttack* const packMelee = reinterpret_cast<const TPacketCGAttack*>(data);

				ch->GetDesc()->AssembleCRCMagicCube(packMelee->bCRCMagicCubeProcPiece, packMelee->bCRCMagicCubeFilePiece);

				LPCHARACTER	victim = CHARACTER_MANAGER::instance().Find(packMelee->dwVID);

				if (NULL == victim || ch == victim)
					return;

				switch (victim->GetCharType())
				{
					case CHAR_TYPE_NPC:
					case CHAR_TYPE_WARP:
					case CHAR_TYPE_GOTO:
						return;
				}

				if (packMelee->bType > 0)
				{
					if (false == ch->CheckSkillHitCount(packMelee->bType, victim->GetVID()))
					{
						return;
					}
				}

				ch->Attack(victim, packMelee->bType);
			}
			break;

		case HEADER_CG_SHOOT:
			{
				const TPacketCGShoot* const packShoot = reinterpret_cast<const TPacketCGShoot*>(data);

				ch->Shoot(packShoot->bType);
			}
			break;
	}
}

int CInputMain::SyncPosition(LPCHARACTER ch, const char * c_pcData, size_t uiBytes)
{
	const TPacketCGSyncPosition* pinfo = reinterpret_cast<const TPacketCGSyncPosition*>( c_pcData );

	if (uiBytes < pinfo->wSize)
		return -1;

	int iExtraLen = pinfo->wSize - sizeof(TPacketCGSyncPosition);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->wSize, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}

	if (0 != (iExtraLen % sizeof(TPacketCGSyncPositionElement)))
	{
		sys_err("invalid packet length %d (name: %s)", pinfo->wSize, ch->GetName());
		return iExtraLen;
	}

	int iCount = iExtraLen / sizeof(TPacketCGSyncPositionElement);

	if (iCount <= 0)
		return iExtraLen;

	static const int nCountLimit = 16;

	if( iCount > nCountLimit )
	{
		//LogManager::instance().HackLog( "SYNC_POSITION_HACK", ch );
		sys_err( "Too many SyncPosition Count(%d) from Name(%s)", iCount, ch->GetName() );
		//ch->GetDesc()->SetPhase(PHASE_CLOSE);
		//return -1;
		iCount = nCountLimit;
	}

	TEMP_BUFFER tbuf;
	LPBUFFER lpBuf = tbuf.getptr();

	TPacketGCSyncPosition * pHeader = (TPacketGCSyncPosition *) buffer_write_peek(lpBuf);
	buffer_write_proceed(lpBuf, sizeof(TPacketGCSyncPosition));

	const TPacketCGSyncPositionElement* e =
		reinterpret_cast<const TPacketCGSyncPositionElement*>(c_pcData + sizeof(TPacketCGSyncPosition));

	timeval tvCurTime;
	gettimeofday(&tvCurTime, NULL);

	for (int i = 0; i < iCount; ++i, ++e)
	{
		LPCHARACTER victim = CHARACTER_MANAGER::instance().Find(e->dwVID);

		if (!victim)
			continue;

		switch (victim->GetCharType())
		{
			case CHAR_TYPE_NPC:
			case CHAR_TYPE_WARP:
			case CHAR_TYPE_GOTO:
				continue;
		}

		// ¼ÒÀ¯±Ç °Ë»ç
		if (!victim->SetSyncOwner(ch))
			continue;

		const float fDistWithSyncOwner = DISTANCE_SQRT( (victim->GetX() - ch->GetX()) / 100, (victim->GetY() - ch->GetY()) / 100 );
		static const float fLimitDistWithSyncOwner = 2500.f + 1000.f;
		// victim°úÀÇ °Å¸®°¡ 2500 + a ÀÌ»óÀÌ¸é ÇÙÀ¸·Î °£ÁÖ.
		//	°Å¸® ÂüÁ¶ : Å¬¶óÀÌ¾ðÆ®ÀÇ __GetSkillTargetRange, __GetBowRange ÇÔ¼ö
		//	2500 : ½ºÅ³ proto¿¡¼­ °¡Àå »ç°Å¸®°¡ ±ä ½ºÅ³ÀÇ »ç°Å¸®, ¶Ç´Â È°ÀÇ »ç°Å¸®
		//	a = POINT_BOW_DISTANCE °ª... ÀÎµ¥ ½ÇÁ¦·Î »ç¿ëÇÏ´Â °ªÀÎÁö´Â Àß ¸ð¸£°ÚÀ½. ¾ÆÀÌÅÛÀÌ³ª Æ÷¼Ç, ½ºÅ³, Äù½ºÆ®¿¡´Â ¾ø´Âµ¥...
		//		±×·¡µµ È¤½Ã³ª ÇÏ´Â ¸¶À½¿¡ ¹öÆÛ·Î »ç¿ëÇÒ °âÇØ¼­ 1000.f ·Î µÒ...
		if (fDistWithSyncOwner > fLimitDistWithSyncOwner)
		{
			// g_iSyncHackLimitCount¹ø ±îÁö´Â ºÁÁÜ.
			if (ch->GetSyncHackCount() < g_iSyncHackLimitCount)
			{
				ch->SetSyncHackCount(ch->GetSyncHackCount() + 1);
				continue;
			}
			else
			{
				LogManager::instance().HackLog( "SYNC_POSITION_HACK", ch );

				sys_err( "Too far SyncPosition DistanceWithSyncOwner(%f)(%s) from Name(%s) CH(%d,%d) VICTIM(%d,%d) SYNC(%d,%d)",
					fDistWithSyncOwner, victim->GetName(), ch->GetName(), ch->GetX(), ch->GetY(), victim->GetX(), victim->GetY(),
					e->lX, e->lY );
#ifndef FIX_SyncPosition
				ch->GetDesc()->SetPhase(PHASE_CLOSE);

				return -1;
#endif
			}
		}

		const float fDist = DISTANCE_SQRT( (victim->GetX() - e->lX) / 100, (victim->GetY() - e->lY) / 100 );
		static const long g_lValidSyncInterval = 100 * 1000; // 100ms
		const timeval &tvLastSyncTime = victim->GetLastSyncTime();
		timeval *tvDiff = timediff(&tvCurTime, &tvLastSyncTime);

		// SyncPositionÀ» ¾Ç¿ëÇÏ¿© Å¸À¯Àú¸¦ ÀÌ»óÇÑ °÷À¸·Î º¸³»´Â ÇÙ ¹æ¾îÇÏ±â À§ÇÏ¿©,
		// °°Àº À¯Àú¸¦ g_lValidSyncInterval ms ÀÌ³»¿¡ ´Ù½Ã SyncPositionÇÏ·Á°í ÇÏ¸é ÇÙÀ¸·Î °£ÁÖ.
		if (tvDiff->tv_sec == 0 && tvDiff->tv_usec < g_lValidSyncInterval)
		{
			// g_iSyncHackLimitCount¹ø ±îÁö´Â ºÁÁÜ.
			if (ch->GetSyncHackCount() < g_iSyncHackLimitCount)
			{
				ch->SetSyncHackCount(ch->GetSyncHackCount() + 1);
				continue;
			}
			else
			{
				LogManager::instance().HackLog( "SYNC_POSITION_HACK", ch );

				sys_err( "Too often SyncPosition Interval(%ldms)(%s) from Name(%s) VICTIM(%d,%d) SYNC(%d,%d)",
					tvDiff->tv_sec * 1000 + tvDiff->tv_usec / 1000, victim->GetName(), ch->GetName(), victim->GetX(), victim->GetY(),
					e->lX, e->lY );

#ifndef FIX_SyncPosition
				ch->GetDesc()->SetPhase(PHASE_CLOSE);

				return -1;
#endif
			}
		}
		else if( fDist > 25.0f )
		{
			LogManager::instance().HackLog( "SYNC_POSITION_HACK", ch );

			sys_err( "Too far SyncPosition Distance(%f)(%s) from Name(%s) CH(%d,%d) VICTIM(%d,%d) SYNC(%d,%d)",
				   	fDist, victim->GetName(), ch->GetName(), ch->GetX(), ch->GetY(), victim->GetX(), victim->GetY(),
				  e->lX, e->lY );

#ifndef FIX_SyncPosition
				ch->GetDesc()->SetPhase(PHASE_CLOSE);

				return -1;
#endif
		}
		else
		{
			victim->SetLastSyncTime(tvCurTime);
			victim->Sync(e->lX, e->lY);
			buffer_write(lpBuf, e, sizeof(TPacketCGSyncPositionElement));
		}
	}

	if (buffer_size(lpBuf) != sizeof(TPacketGCSyncPosition))
	{
		pHeader->bHeader = HEADER_GC_SYNC_POSITION;
		pHeader->wSize = buffer_size(lpBuf);

		ch->PacketAround(buffer_read_peek(lpBuf), buffer_size(lpBuf), ch);
	}

	return iExtraLen;
}

void CInputMain::FlyTarget(LPCHARACTER ch, const char * pcData, BYTE bHeader)
{
	TPacketCGFlyTargeting * p = (TPacketCGFlyTargeting *) pcData;
	ch->FlyTarget(p->dwTargetVID, p->x, p->y, bHeader);
}

void CInputMain::UseSkill(LPCHARACTER ch, const char * pcData)
{
	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	const auto* p = reinterpret_cast<const TPacketCGUseSkill*>(pcData);
	if (!p)
	{
		return;
	}

	ch->UseSkill(p->dwVnum, CHARACTER_MANAGER::instance().Find(p->dwVID)
#if defined(__AUTOMATIC_HUNTING_SYSTEM__) && defined(USE_AUTOMATIC_HUNTING_USE_SKILLS_RENEWED)
, true, p->bFromAutoHunt
#endif
	);
}

void CInputMain::ScriptButton(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptButton * p = (TPacketCGScriptButton *) c_pData;
	sys_log(0, "QUEST ScriptButton pid %d idx %u", ch->GetPlayerID(), p->idx);

	quest::PC* pc = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());
	if (pc && pc->IsConfirmWait())
	{
		quest::CQuestManager::instance().Confirm(ch->GetPlayerID(), quest::CONFIRM_TIMEOUT);
	}
	else if (p->idx & 0x80000000)
	{
		//Äù½ºÆ® Ã¢¿¡¼­ Å¬¸¯½Ã(__SelectQuest) ¿©±â·Î
		quest::CQuestManager::Instance().QuestInfo(ch->GetPlayerID(), p->idx & 0x7fffffff);
	}
	else
	{
		quest::CQuestManager::Instance().QuestButton(ch->GetPlayerID(), p->idx);
	}
}

void CInputMain::ScriptAnswer(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptAnswer * p = (TPacketCGScriptAnswer *) c_pData;
	sys_log(0, "QUEST ScriptAnswer pid %d answer %d", ch->GetPlayerID(), p->answer);

	if (p->answer > 250) // ´ÙÀ½ ¹öÆ°¿¡ ´ëÇÑ ÀÀ´äÀ¸·Î ¿Â ÆÐÅ¶ÀÎ °æ¿ì
	{
		quest::CQuestManager::Instance().Resume(ch->GetPlayerID());
	}
	else // ¼±ÅÃ ¹öÆ°À» °ñ¶ó¼­ ¿Â ÆÐÅ¶ÀÎ °æ¿ì
	{
		quest::CQuestManager::Instance().Select(ch->GetPlayerID(),  p->answer);
	}
}


// SCRIPT_SELECT_ITEM
void CInputMain::ScriptSelectItem(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptSelectItem* p = (TPacketCGScriptSelectItem*) c_pData;
	sys_log(0, "QUEST ScriptSelectItem pid %d answer %d", ch->GetPlayerID(), p->selection);
	quest::CQuestManager::Instance().SelectItem(ch->GetPlayerID(), p->selection);
}
// END_OF_SCRIPT_SELECT_ITEM

void CInputMain::QuestInputString(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGQuestInputString * p = (TPacketCGQuestInputString*) c_pData;

	char msg[65];
	strlcpy(msg, p->msg, sizeof(msg));
	sys_log(0, "QUEST InputString pid %u msg %s", ch->GetPlayerID(), msg);

	quest::CQuestManager::Instance().Input(ch->GetPlayerID(), msg);
}

void CInputMain::QuestInputLongString(LPCHARACTER ch, const void * c_pvData)
{
	const TPacketCGQuestInputLongString * p = reinterpret_cast<const TPacketCGQuestInputLongString*>(c_pvData);
	sys_log(0, "QUEST InputLongString pid %u msg %s", ch->GetPlayerID(), p->szMsg);
	quest::CQuestManager::Instance().Input(ch->GetPlayerID(), p->szMsg);
}

void CInputMain::QuestConfirm(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGQuestConfirm* p = (TPacketCGQuestConfirm*) c_pData;
	LPCHARACTER ch_wait = CHARACTER_MANAGER::instance().FindByPID(p->requestPID);
	if (p->answer)
		p->answer = quest::CONFIRM_YES;
	sys_log(0, "QuestConfirm from %s pid %u name %s answer %d", ch->GetName(), p->requestPID, (ch_wait)?ch_wait->GetName():"", p->answer);
	if (ch_wait)
	{
		quest::CQuestManager::Instance().Confirm(ch_wait->GetPlayerID(), (quest::EQuestConfirmType) p->answer, ch->GetPlayerID());
	}
}

void CInputMain::Target(LPCHARACTER ch, const char * pcData)
{
	TPacketCGTarget * p = (TPacketCGTarget *) pcData;

	building::LPOBJECT pkObj = building::CManager::instance().FindObjectByVID(p->dwVID);

	if (pkObj)
	{
		TPacketGCTarget pckTarget;
		pckTarget.header = HEADER_GC_TARGET;
		pckTarget.dwVID = p->dwVID;
		ch->GetDesc()->Packet(&pckTarget, sizeof(TPacketGCTarget));
	}
	else
		ch->SetTarget(CHARACTER_MANAGER::instance().Find(p->dwVID));
}

void CInputMain::Warp(LPCHARACTER ch, const char * pcData)
{
	ch->WarpEnd();
}

void CInputMain::SafeboxCheckin(LPCHARACTER ch, const char * c_pData)
{

#ifdef ENABLE_PREVENT_FLOOD_PACKETS // Not in gameforge, but useful
	const int iPulse = thecore_pulse();
	if (iPulse - ch->GetLastSafeboxCheckOutTime() < 15) // 0.5 sec
	{
		ch->ChatPacket(CHAT_TYPE_INFO, ("[1554]Please try again later."));
		return;
	}
#endif	
	
	if (quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID())->IsRunning() == true)
		return;

	TPacketCGSafeboxCheckin * p = (TPacketCGSafeboxCheckin *) c_pData;

	if (!ch->CanHandleItem())
		return;

	CSafebox * pkSafebox = ch->GetSafebox();
	LPITEM pkItem = ch->GetItem(p->ItemPos);

	if (!pkSafebox || !pkItem)
		return;

	if (pkItem->GetCell() >= INVENTORY_MAX_NUM && IS_SET(pkItem->GetFlag(), ITEM_FLAG_IRREMOVABLE))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "761");
		return;
	}

	if (!pkSafebox->IsEmpty(p->bSafePos, pkItem->GetSize()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "762");
		return;
	}

	if (pkItem->GetVnum() == UNIQUE_ITEM_SAFEBOX_EXPAND)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "763");
		return;
	}

	if( IS_SET(pkItem->GetAntiFlag(), ITEM_ANTIFLAG_SAFEBOX) )
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "763");
		return;
	}

	if (true == pkItem->isLocked())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "763");
		return;
	}

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	if (pkItem->IsEquipped())
	{
		int iWearCell = pkItem->FindEquipCell(ch);
		if (iWearCell == WEAR_WEAPON)
		{
			LPITEM costumeWeapon = ch->GetWear(WEAR_COSTUME_WEAPON);
			if (costumeWeapon && !ch->UnequipItem(costumeWeapon))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "300");
				return;
			}
		}
	}
#endif

	// @fixme140 BEGIN
	if (ITEM_BELT == pkItem->GetType() && CBeltInventoryHelper::IsExistItemInBeltInventory(ch))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "764");
		return;
	}
	// @fixme140 END

	pkItem->RemoveFromCharacter();

#ifdef ENABLE_SPECIAL_STORAGE
	if (!pkItem->IsDragonSoul() && !pkItem->IsUpgradeItem() && !pkItem->IsBook() && !pkItem->IsStone() && !pkItem->IsChange() && !pkItem->IsCostume() && !pkItem->IsChest())
#else
	if (!pkItem->IsDragonSoul())
#endif
	{
		ch->SyncQuickslot(QUICKSLOT_TYPE_ITEM, p->ItemPos.cell, WORD_MAX);
	}

	pkSafebox->Add(p->bSafePos, pkItem);

#ifdef ENABLE_PREVENT_FLOOD_PACKETS
	ch->SetLastSafeboxCheckOutTime();
#endif

	char szHint[128];
	snprintf(szHint, sizeof(szHint), "%s %u", pkItem->GetName(), pkItem->GetCount());
	LogManager::instance().ItemLog(ch, pkItem, "SAFEBOX PUT", szHint);
}

void CInputMain::SafeboxCheckout(LPCHARACTER ch, const char * c_pData, bool bMall)
{
	
#ifdef ENABLE_PREVENT_FLOOD_PACKETS
	const int iPulse = thecore_pulse();
	if (iPulse - ch->GetLastSafeboxCheckOutTime() < 15) // 0.5 sec
	{
		ch->ChatPacket(CHAT_TYPE_INFO, ("[1554]Please try again later."));
		return;
	}
#endif	
	
	TPacketCGSafeboxCheckout * p = (TPacketCGSafeboxCheckout *) c_pData;

	if (!ch->CanHandleItem())
		return;

	CSafebox * pkSafebox;

	if (bMall)
		pkSafebox = ch->GetMall();
	else
		pkSafebox = ch->GetSafebox();

	if (!pkSafebox)
		return;

	LPITEM pkItem = pkSafebox->Get(p->bSafePos);

	if (!pkItem)
		return;

	if (!ch->IsEmptyItemGrid(p->ItemPos, pkItem->GetSize()))
		return;

	// protect.
#ifdef ENABLE_SPECIAL_STORAGE
	if(UPGRADE_INVENTORY == p->ItemPos.window_type || BOOK_INVENTORY == p->ItemPos.window_type ||
		CHANGE_INVENTORY == p->ItemPos.window_type || COSTUME_INVENTORY == p->ItemPos.window_type || STONE_INVENTORY == p->ItemPos.window_type || CHEST_INVENTORY == p->ItemPos.window_type)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "762");
		return;
	}
#endif

	// ¾ÆÀÌÅÛ ¸ô¿¡¼­ ÀÎº¥À¸·Î ¿Å±â´Â ºÎºÐ¿¡¼­ ¿ëÈ¥¼® Æ¯¼ö Ã³¸®
	// (¸ô¿¡¼­ ¸¸µå´Â ¾ÆÀÌÅÛÀº item_proto¿¡ Á¤ÀÇµÈ´ë·Î ¼Ó¼ºÀÌ ºÙ±â ¶§¹®¿¡,
	//  ¿ëÈ¥¼®ÀÇ °æ¿ì, ÀÌ Ã³¸®¸¦ ÇÏÁö ¾ÊÀ¸¸é ¼Ó¼ºÀÌ ÇÏ³ªµµ ºÙÁö ¾Ê°Ô µÈ´Ù.)
	if (pkItem->IsDragonSoul())
	{
		if (bMall)
		{
			DSManager::instance().DragonSoulItemInitialize(pkItem);
		}

		if (DRAGON_SOUL_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return;
		}

		TItemPos DestPos = p->ItemPos;
		if (!DSManager::instance().IsValidCellForThisItem(pkItem, DestPos))
		{
			int iCell = ch->GetEmptyDragonSoulInventory(pkItem);
			if (iCell < 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "762");
				return ;
			}
			DestPos = TItemPos (DRAGON_SOUL_INVENTORY, iCell);
		}

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
#ifdef ENABLE_SPECIAL_STORAGE_CLOSE_SAFEBOX_PART
	else if (pkItem->IsUpgradeItem())
	{
		if (UPGRADE_INVENTORY != p->ItemPos.window_type)
		{
			
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyUpgradeInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return ;
		}
		DestPos = TItemPos (UPGRADE_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsBook())
	{
		if (BOOK_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyBookInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return ;
		}
		DestPos = TItemPos (BOOK_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsStone())
	{
		if (STONE_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyStoneInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return ;
		}
		DestPos = TItemPos (STONE_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}

	else if (pkItem->IsChange())
	{
		if (CHANGE_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyChangeInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return ;
		}
		DestPos = TItemPos (CHANGE_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsCostume())
	{
		if (COSTUME_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyCostumeInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return ;
		}
		DestPos = TItemPos (COSTUME_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsChest())
	{
		if (CHEST_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyChestInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return ;
		}
		DestPos = TItemPos (CHEST_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}

#endif
	else
	{

		if (DRAGON_SOUL_INVENTORY == p->ItemPos.window_type
#ifdef ENABLE_SPECIAL_STORAGE_CLOSE_SAFEBOX_PART
		||
			UPGRADE_INVENTORY == p->ItemPos.window_type ||
			BOOK_INVENTORY == p->ItemPos.window_type ||
			CHANGE_INVENTORY == p->ItemPos.window_type ||
			COSTUME_INVENTORY == p->ItemPos.window_type ||
			STONE_INVENTORY == p->ItemPos.window_type ||
			CHEST_INVENTORY == p->ItemPos.window_type
#endif
		)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "762");
			return;
		}
		// @fixme119
		if (p->ItemPos.IsBeltInventoryPosition() && false == CBeltInventoryHelper::CanMoveIntoBeltInventory(pkItem))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "765");
			return;
		}

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, p->ItemPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}

	DWORD dwID = pkItem->GetID();
	db_clientdesc->DBPacketHeader(HEADER_GD_ITEM_FLUSH, 0, sizeof(DWORD));
	db_clientdesc->Packet(&dwID, sizeof(DWORD));

#ifdef ENABLE_PREVENT_FLOOD_PACKETS
	ch->SetLastSafeboxCheckOutTime();
#endif

	char szHint[128];
	snprintf(szHint, sizeof(szHint), "%s %u", pkItem->GetName(), pkItem->GetCount());
	if (bMall)
		LogManager::instance().ItemLog(ch, pkItem, "MALL GET", szHint);
	else
		LogManager::instance().ItemLog(ch, pkItem, "SAFEBOX GET", szHint);
}

void CInputMain::SafeboxItemMove(LPCHARACTER ch, const char * data)
{
	struct command_item_move * pinfo = (struct command_item_move *) data;

	if (!ch->CanHandleItem())
		return;

	if (!ch->GetSafebox())
		return;

	ch->GetSafebox()->MoveItem(pinfo->Cell.cell, pinfo->CellTo.cell, pinfo->count);
}

// PARTY_JOIN_BUG_FIX
void CInputMain::PartyInvite(LPCHARACTER ch, const char * c_pData)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "383");
		return;
	}

	TPacketCGPartyInvite * p = (TPacketCGPartyInvite*) c_pData;

	LPCHARACTER pInvitee = CHARACTER_MANAGER::instance().Find(p->vid);

	if (!pInvitee || !ch->GetDesc() || !pInvitee->GetDesc())
	{
		sys_err("PARTY Cannot find invited character");
		return;
	}
#ifdef ENABLE_MESSENGER_BLOCK
	if (MessengerManager::instance().IsBlocked(ch->GetName(), pInvitee->GetName()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You need to unblock %s do to that.", pInvitee->GetName());
		return;
	}
	if (MessengerManager::instance().IsBlocked(pInvitee->GetName(), ch->GetName()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "%s has blocked you.", pInvitee->GetName());
		return;
	}
#endif

	ch->PartyInvite(pInvitee);
}

void CInputMain::PartyInviteAnswer(LPCHARACTER ch, const char * c_pData)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "383");
		return;
	}

	TPacketCGPartyInviteAnswer * p = (TPacketCGPartyInviteAnswer*) c_pData;

	LPCHARACTER pInviter = CHARACTER_MANAGER::instance().Find(p->leader_vid);

	// pInviter °¡ ch ¿¡°Ô ÆÄÆ¼ ¿äÃ»À» Çß¾ú´Ù.

	if (!pInviter)
		ch->ChatPacket(CHAT_TYPE_INFO, "766");
	else if (!p->accept)
		pInviter->PartyInviteDeny(ch->GetPlayerID());
	else
		pInviter->PartyInviteAccept(ch);
}
// END_OF_PARTY_JOIN_BUG_FIX

void CInputMain::PartySetState(LPCHARACTER ch, const char* c_pData)
{
	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "46");
		return;
	}

	TPacketCGPartySetState* p = (TPacketCGPartySetState*) c_pData;

	if (!ch->GetParty())
		return;

	if (ch->GetParty()->GetLeaderPID() != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "767");
		return;
	}

	if (!ch->GetParty()->IsMember(p->pid))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "768");
		return;
	}

	DWORD pid = p->pid;
	sys_log(0, "PARTY SetRole pid %d to role %d state %s", pid, p->byRole, p->flag ? "on" : "off");

	switch (p->byRole)
	{
		case PARTY_ROLE_NORMAL:
			break;

		case PARTY_ROLE_ATTACKER:
		case PARTY_ROLE_TANKER:
		case PARTY_ROLE_BUFFER:
		case PARTY_ROLE_SKILL_MASTER:
		case PARTY_ROLE_HASTE:
		case PARTY_ROLE_DEFENDER:
			if (ch->GetParty()->SetRole(pid, p->byRole, p->flag))
			{
				TPacketPartyStateChange pack;
				pack.dwLeaderPID = ch->GetPlayerID();
				pack.dwPID = p->pid;
				pack.bRole = p->byRole;
				pack.bFlag = p->flag;
				db_clientdesc->DBPacket(HEADER_GD_PARTY_STATE_CHANGE, 0, &pack, sizeof(pack));
			}
			/* else
			   ch->ChatPacket(CHAT_TYPE_INFO, "769"); */
			break;

		default:
			sys_err("wrong byRole in PartySetState Packet name %s state %d", ch->GetName(), p->byRole);
			break;
	}
}

void CInputMain::PartyRemove(LPCHARACTER ch, const char* c_pData)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "383");
		return;
	}

	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "46");
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "770");
		return;
	}

	TPacketCGPartyRemove* p = (TPacketCGPartyRemove*) c_pData;

	if (!ch->GetParty())
		return;

	LPPARTY pParty = ch->GetParty();
	if (pParty->GetLeaderPID() == ch->GetPlayerID())
	{
		if (ch->GetDungeon())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "771");
		}
		else
		{
			// Àû·æ¼º¿¡¼­ ÆÄÆ¼ÀåÀÌ ´øÁ¯ ¹Û¿¡¼­ ÆÄÆ¼ ÇØ»ê ¸øÇÏ°Ô ¸·ÀÚ
			if(pParty->IsPartyInDungeon(351))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "772");
				return;
			}
#ifdef ENABLE_QUEEN_NETHIS
			if (SnakeLair::CSnk::instance().IsSnakeMap(ch->GetMapIndex()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "[LS;531]");
				return;
			}
#endif

			// leader can remove any member
			if (p->pid == ch->GetPlayerID() || pParty->GetMemberCount() == 2)
			{
				// party disband
				CPartyManager::instance().DeleteParty(pParty);
			}
			else
			{
				LPCHARACTER B = CHARACTER_MANAGER::instance().FindByPID(p->pid);
				if (B)
				{
					//pParty->SendPartyRemoveOneToAll(B);
					B->ChatPacket(CHAT_TYPE_INFO, "773");
					//pParty->Unlink(B);
					//CPartyManager::instance().SetPartyMember(B->GetPlayerID(), NULL);
				}
				pParty->Quit(p->pid);
			}
		}
	}
	else
	{
		// otherwise, only remove itself
		if (p->pid == ch->GetPlayerID())
		{
			if (ch->GetDungeon())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "774");
			}
			else
			{
#ifdef ENABLE_QUEEN_NETHIS
				if (SnakeLair::CSnk::instance().IsSnakeMap(ch->GetMapIndex()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "[LS;676]");
					return;
				}
#endif

				if (pParty->GetMemberCount() == 2)
				{
					// party disband
					CPartyManager::instance().DeleteParty(pParty);
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "423");
					//pParty->SendPartyRemoveOneToAll(ch);
					pParty->Quit(ch->GetPlayerID());
					//pParty->SendPartyRemoveAllToOne(ch);
					//CPartyManager::instance().SetPartyMember(ch->GetPlayerID(), NULL);
				}
			}
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "775");
		}
	}
}

void CInputMain::AnswerMakeGuild(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGAnswerMakeGuild* p = (TPacketCGAnswerMakeGuild*) c_pData;

	if (ch->GetGold() < 200000)
		return;

	if (get_global_time() - ch->GetQuestFlag("guild_manage.new_disband_time") <
			CGuildManager::instance().GetDisbandDelay())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "776 %d",
				quest::CQuestManager::instance().GetEventFlag("guild_disband_delay"));
		return;
	}

	if (get_global_time() - ch->GetQuestFlag("guild_manage.new_withdraw_time") <
			CGuildManager::instance().GetWithdrawDelay())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "777 %d",
				quest::CQuestManager::instance().GetEventFlag("guild_withdraw_delay"));
		return;
	}

	if (ch->GetGuild())
		return;

	CGuildManager& gm = CGuildManager::instance();

	TGuildCreateParameter cp;
	memset(&cp, 0, sizeof(cp));

	cp.master = ch;
	strlcpy(cp.name, p->guild_name, sizeof(cp.name));

	if (cp.name[0] == 0 || !check_name(cp.name))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "571");
		return;
	}

	DWORD dwGuildID = gm.CreateGuild(cp);

	if (dwGuildID)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "778 %s", cp.name);

		int GuildCreateFee = 200000;

		ch->PointChange(POINT_GOLD, -GuildCreateFee);
		DBManager::instance().SendMoneyLog(MONEY_LOG_GUILD, ch->GetPlayerID(), -GuildCreateFee);

		char Log[128];
		snprintf(Log, sizeof(Log), "GUILD_NAME %s MASTER %s", cp.name, ch->GetName());
		LogManager::instance().CharLog(ch, 0, "MAKE_GUILD", Log);

		ch->RemoveSpecifyItem(GUILD_CREATE_ITEM_VNUM, 1);
		//ch->SendGuildName(dwGuildID);
	}
	else
		ch->ChatPacket(CHAT_TYPE_INFO, "779");
}

void CInputMain::PartyUseSkill(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGPartyUseSkill* p = (TPacketCGPartyUseSkill*) c_pData;
	if (!ch->GetParty())
		return;

	if (ch->GetPlayerID() != ch->GetParty()->GetLeaderPID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "780");
		return;
	}

	switch (p->bySkillIndex)
	{
		case PARTY_SKILL_HEAL:
			ch->GetParty()->HealParty();
			break;
		case PARTY_SKILL_WARP:
			{
				LPCHARACTER pch = CHARACTER_MANAGER::instance().Find(p->vid);
				if (pch)
					ch->GetParty()->SummonToLeader(pch->GetPlayerID());
				else
					ch->ChatPacket(CHAT_TYPE_INFO, "781");
			}
			break;
	}
}

void CInputMain::PartyParameter(LPCHARACTER ch, const char * c_pData)
{
	TPacketCGPartyParameter * p = (TPacketCGPartyParameter *) c_pData;

	if (ch->GetParty())
		ch->GetParty()->SetParameter(p->bDistributeMode);
}

size_t GetSubPacketSize(const GUILD_SUBHEADER_CG& header)
{
	switch (header)
	{
		case GUILD_SUBHEADER_CG_DEPOSIT_MONEY:				return sizeof(int);
		case GUILD_SUBHEADER_CG_WITHDRAW_MONEY:				return sizeof(int);
		case GUILD_SUBHEADER_CG_ADD_MEMBER:					return sizeof(DWORD);
		case GUILD_SUBHEADER_CG_REMOVE_MEMBER:				return sizeof(DWORD);
		case GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME:			return 10;
		case GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY:		return sizeof(BYTE) + sizeof(BYTE);
		case GUILD_SUBHEADER_CG_OFFER:						return sizeof(DWORD);
		case GUILD_SUBHEADER_CG_CHARGE_GSP:					return sizeof(int);
		case GUILD_SUBHEADER_CG_POST_COMMENT:				return 1;
		case GUILD_SUBHEADER_CG_DELETE_COMMENT:				return sizeof(DWORD);
		case GUILD_SUBHEADER_CG_REFRESH_COMMENT:			return 0;
		case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE:		return sizeof(DWORD) + sizeof(BYTE);
		case GUILD_SUBHEADER_CG_USE_SKILL:					return sizeof(TPacketCGGuildUseSkill);
		case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL:		return sizeof(DWORD) + sizeof(BYTE);
		case GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER:		return sizeof(DWORD) + sizeof(BYTE);
	}

	return 0;
}

int CInputMain::Guild(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	if (uiBytes < sizeof(TPacketCGGuild))
		return -1;

	const TPacketCGGuild* p = reinterpret_cast<const TPacketCGGuild*>(data);
	const char* c_pData = data + sizeof(TPacketCGGuild);

	uiBytes -= sizeof(TPacketCGGuild);

	const GUILD_SUBHEADER_CG SubHeader = static_cast<GUILD_SUBHEADER_CG>(p->subheader);
	const size_t SubPacketLen = GetSubPacketSize(SubHeader);

	if (uiBytes < SubPacketLen)
	{
		return -1;
	}

	CGuild* pGuild = ch->GetGuild();

	if (NULL == pGuild)
	{
		if (SubHeader != GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "420");
			return SubPacketLen;
		}
	}

	switch (SubHeader)
	{
		case GUILD_SUBHEADER_CG_DEPOSIT_MONEY:
			{
				// by mhh : ±æµåÀÚ±ÝÀº ´çºÐ°£ ³ÖÀ» ¼ö ¾ø´Ù.
				return SubPacketLen;

				const int gold = MIN(*reinterpret_cast<const int*>(c_pData), __deposit_limit());

				if (gold < 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "782");
					return SubPacketLen;
				}

				if (ch->GetGold() < gold)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "696");
					return SubPacketLen;
				}

				pGuild->RequestDepositMoney(ch, gold);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_WITHDRAW_MONEY:
			{
				// by mhh : ±æµåÀÚ±ÝÀº ´çºÐ°£ »¬ ¼ö ¾ø´Ù.
				return SubPacketLen;

				const int gold = MIN(*reinterpret_cast<const int*>(c_pData), 500000);

				if (gold < 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "782");
					return SubPacketLen;
				}

				pGuild->RequestWithdrawMoney(ch, gold);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_ADD_MEMBER:
			{
				const DWORD vid = *reinterpret_cast<const DWORD*>(c_pData);
				LPCHARACTER newmember = CHARACTER_MANAGER::instance().Find(vid);

				if (!newmember)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "783");
					return SubPacketLen;
				}

				// @fixme145 BEGIN (+newmember ispc check)
				if (!ch->IsPC() || !newmember->IsPC())
					return SubPacketLen;
				// @fixme145 END
				
#ifdef ENABLE_MESSENGER_BLOCK
				if (MessengerManager::instance().IsBlocked(ch->GetName(), newmember->GetName()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "You need to unblock %s do to that.", newmember->GetName());
					return SubPacketLen;
				}
				if (MessengerManager::instance().IsBlocked(newmember->GetName(), ch->GetName()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "%s has blocked you.", newmember->GetName());
					return SubPacketLen;
				}
#endif
				pGuild->Invite(ch, newmember);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_REMOVE_MEMBER:
			{
				if (pGuild->UnderAnyWar() != 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "784");
					return SubPacketLen;
				}

				const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				LPCHARACTER member = CHARACTER_MANAGER::instance().FindByPID(pid);

				if (member)
				{
					if (member->GetGuild() != pGuild)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "785");
						return SubPacketLen;
					}

					if (!pGuild->HasGradeAuth(m->grade, GUILD_AUTH_REMOVE_MEMBER))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "786");
						return SubPacketLen;
					}

					member->SetQuestFlag("guild_manage.new_withdraw_time", get_global_time());
					pGuild->RequestRemoveMember(member->GetPlayerID());

					if (g_bGuildInviteLimit)
					{
						DBManager::instance().Query("REPLACE INTO guild_invite_limit VALUES(%d, %d)", pGuild->GetID(), get_global_time());
					}
				}
				else
				{
					if (!pGuild->HasGradeAuth(m->grade, GUILD_AUTH_REMOVE_MEMBER))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "786");
						return SubPacketLen;
					}

					if (pGuild->RequestRemoveMember(pid))
						ch->ChatPacket(CHAT_TYPE_INFO, "787");
					else
						ch->ChatPacket(CHAT_TYPE_INFO, "783");
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME:
			{
				char gradename[GUILD_GRADE_NAME_MAX_LEN + 1];
				strlcpy(gradename, c_pData + 1, sizeof(gradename));

				const TGuildMember * m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (m->grade != GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "788");
				}
				else if (*c_pData == GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "789");
				}
				else if (!check_name(gradename))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "790");
				}
				else
				{
					pGuild->ChangeGradeName(*c_pData, gradename);
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY:
			{
				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (m->grade != GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "791");
				}
				else if (*c_pData == GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "792");
				}
				else
				{
					pGuild->ChangeGradeAuth(*c_pData, *(c_pData + 1));
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_OFFER:
			{
#ifdef __ANTI_EXP__
				if (ch->GetQuestFlag("anti_exp.block_exp") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "793");
					return SubPacketLen;
				}

#endif

				DWORD offer = *reinterpret_cast<const DWORD*>(c_pData);

				if (pGuild->GetLevel() >= GUILD_MAX_LEVEL)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "794");
				}
				else
				{
					offer /= 100;
					offer *= 100;

					if (pGuild->OfferExp(ch, offer))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "795 %u", offer);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "796");
					}
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHARGE_GSP:
			{
				const int offer = *reinterpret_cast<const int*>(c_pData);
				const int gold = offer * 100;

				if (offer < 0 || gold < offer || gold < 0 || ch->GetGold() < gold)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "797");
					return SubPacketLen;
				}

				if (!pGuild->ChargeSP(ch, offer))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "798");
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_POST_COMMENT:
			{
				const size_t length = *c_pData;

				if (length > GUILD_COMMENT_MAX_LEN)
				{
					// Àß¸øµÈ ±æÀÌ.. ²÷¾îÁÖÀÚ.
					sys_err("POST_COMMENT: %s comment too long (length: %u)", ch->GetName(), length);
					ch->GetDesc()->SetPhase(PHASE_CLOSE);
					return -1;
				}

				if (uiBytes < 1 + length)
					return -1;

				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (length && !pGuild->HasGradeAuth(m->grade, GUILD_AUTH_NOTICE) && *(c_pData + 1) == '!')
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "799");
				}
				else
				{
					std::string str(c_pData + 1, length);
					pGuild->AddComment(ch, str);
				}

				return (1 + length);
			}

		case GUILD_SUBHEADER_CG_DELETE_COMMENT:
			{
				const DWORD comment_id = *reinterpret_cast<const DWORD*>(c_pData);

				pGuild->DeleteComment(ch, comment_id);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_REFRESH_COMMENT:
			pGuild->RefreshComment(ch);
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE:
			{
				const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
				const BYTE grade = *(c_pData + sizeof(DWORD));
				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (m->grade != GUILD_LEADER_GRADE)
					ch->ChatPacket(CHAT_TYPE_INFO, "800");
				else if (ch->GetPlayerID() == pid)
					ch->ChatPacket(CHAT_TYPE_INFO, "801");
				else if (grade == 1)
					ch->ChatPacket(CHAT_TYPE_INFO, "802");
				else
					pGuild->ChangeMemberGrade(pid, grade);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_USE_SKILL:
			{
				const TPacketCGGuildUseSkill* p = reinterpret_cast<const TPacketCGGuildUseSkill*>(c_pData);

				pGuild->UseSkill(p->dwVnum, ch, p->dwPID);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL:
			{
				const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
				const BYTE is_general = *(c_pData + sizeof(DWORD));
				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (m->grade != GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "803");
				}
				else
				{
					if (!pGuild->ChangeMemberGeneral(pid, is_general))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, "804");
					}
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER:
			{
				const DWORD guild_id = *reinterpret_cast<const DWORD*>(c_pData);
				const BYTE accept = *(c_pData + sizeof(DWORD));

				CGuild * g = CGuildManager::instance().FindGuild(guild_id);

				if (g)
				{
					if (accept)
						g->InviteAccept(ch);
					else
						g->InviteDeny(ch->GetPlayerID());
				}
			}
			return SubPacketLen;

	}

	return 0;
}

void CInputMain::Fishing(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGFishing* p = (TPacketCGFishing*)c_pData;
	ch->SetRotation(p->dir * 5);
	ch->fishing(p->fishing_key);
	return;
}

#if defined(__BL_67_ATTR__)
#include "../common/VnumHelper.h"

void CInputMain::Attr67(LPCHARACTER ch, const char* c_pData)
{
	const TPacketCG67Attr* p = reinterpret_cast<const TPacketCG67Attr*>(c_pData);
	
	if (ch->IsDead())
		return;
	
	if (ch->GetExchange()
		 || ch->IsOpenSafebox()
		 || ch->GetShop()
		 || ch->IsCubeOpen()
#ifdef __AURA_SYSTEM__
		 || ch->IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
		 || ch->isSashOpened(true)
		 || ch->isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
		 || ch->IsShardCraftOpened()
#endif
	)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "89");
		return;
	}
	
	const LPITEM item = ch->GetInventoryItem(p->sItemPos);
	if (!item)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "There is no item.");
		return;
	}

	switch (item->GetType())
	{
	case ITEM_WEAPON:
	case ITEM_ARMOR:
	case ITEM_COSTUME:
	case ITEM_NEW_RINGS:
		break;
	default:
		ch->ChatPacket(CHAT_TYPE_INFO, "The item type is not suitable for 6-7 attr.");
		return;
	}

	if (ITEM_COSTUME == item->GetType() && COSTUME_SASH == item->GetSubType())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "The item type is not suitable for 6-7 attr.");
		return;
	}

	if (item->IsEquipped())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You cannot add a bonus to a equipped item.");
		return;
	}

	if (item->IsExchanging())
		return;

	const int norm_attr_count = item->GetAttributeCount();
	const int rare_attr_count = item->GetRareAttrCount();
	const int attr_set_index = item->GetAttributeSetIndex();

	if (attr_set_index == -1 || norm_attr_count < 5 || rare_attr_count >= 2)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "This item is not suitable for 6-7 attr.");
		return;
	}

	enum
	{
		SUCCESS_PER_MATERIAL = 2,
		MATERIAL_MAX_COUNT = 10,
		SUPPORT_MAX_COUNT = 5,
	};

	if (p->bMaterialCount > MATERIAL_MAX_COUNT || p->bSupportCount > SUPPORT_MAX_COUNT)
		return;

#ifdef USE_BL_67_ATTR_RENEWED
	const DWORD dwMaterialVnum = CItemVnumHelper::Get67MaterialVnum(item->GetType(), item->GetSubType());
#else
	const DWORD dwMaterialVnum = CItemVnumHelper::Get67MaterialVnum(item->GetLevelLimit()
#ifdef ENABLE_YOHARA_SYSTEM
, item->GetConquerorLevelLimit()
#endif
	);
#endif

	if (dwMaterialVnum == 0 || p->bMaterialCount < 1 
		|| ch->CountSpecifyItem(dwMaterialVnum) < p->bMaterialCount)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "You don't have enough material item.");
		return;
	}

	int success = SUCCESS_PER_MATERIAL * p->bMaterialCount;

	if (p->sSupportPos != -1)
	{
		const LPITEM support_item = ch->GetInventoryItem(p->sSupportPos);
		if (support_item)
		{
			if (support_item->GetCount() < p->bSupportCount)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "You don't have enough support item.");
				return;
			}

			BYTE uSupportSuccess = 0;
			switch (support_item->GetVnum())
			{
			case 72064:
				uSupportSuccess = 1;
				break;
			case 72065:
				uSupportSuccess = 2;
				break;
			case 72066:
				uSupportSuccess = 4;
				break;
			case 72067:
				uSupportSuccess = 10;
				break;
			case 72068:
				uSupportSuccess = 100;
				break;
			default:
				ch->ChatPacket(CHAT_TYPE_INFO, "The support item is inappropriate.");
				return;
			}

			success += uSupportSuccess * p->bSupportCount;
			support_item->SetCount(support_item->GetCount() - p->bSupportCount);
		}
	}

	ch->RemoveSpecifyItem(dwMaterialVnum, p->bMaterialCount);
	
	const bool bAdded = (number(1, 100) <= success && item->AddRareAttribute());
	ch->ChatPacket(CHAT_TYPE_INFO, "%s!", bAdded ? "Success" : "Failed");
}

void CInputMain::Attr67Close(LPCHARACTER ch, const char* c_pData)
{
	ch->Set67Attr(false);
}
#endif

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
void CInputMain::RequestFlag(LPCHARACTER ch, const char* c_pData)
{
	const TPacketCGRequestFlag* p = reinterpret_cast<const TPacketCGRequestFlag*>(c_pData);
	if (!ch)
		return;
	BYTE language = 0;
	BYTE empire = 0;

	LPCHARACTER chTarget = CHARACTER_MANAGER::Instance().FindPC(p->name);
	if(chTarget != NULL) // same core.
	{
		language = chTarget->GetCharacterLanguage();
		empire = chTarget->GetEmpire();
	}
	else
	{
		CCI* pkCCI = P2P_MANAGER::Instance().Find(p->name);
		if(pkCCI)//other ch
		{
			language = pkCCI->lang;
			empire = pkCCI->bEmpire;
		}
	}
	if(language > 0 && empire > 0)
		ch->ChatPacket(CHAT_TYPE_COMMAND, "BINARY_SET_LANG_AND_EMPIRE_FLAG %s %d %d", p->name, language, empire);
}
#endif
#if defined(ENABLE_FISHING_GAME)
void CInputMain::FishingGame(const LPCHARACTER c_lpChar, const char* c_pszData)
{
	const TPacketCGFishingGame* c_pData = reinterpret_cast<const TPacketCGFishingGame*>(c_pszData);
	if (c_pData == nullptr || c_lpChar == nullptr)
		return;

	const LPITEM c_lpItem = c_lpChar->GetWear(WEAR_WEAPON);
	if (c_lpChar == nullptr)
		return;

	if (c_lpChar->m_pkFishingEvent)
	{
		switch (c_pData->bSubHeader)
		{
		case FISHING_GAME_SUBHEADER_GOAL:
			c_lpChar->SetFishingGameGoals(c_pData->bGoals);
			if (c_lpChar->GetFishingGameGoals() >= 3)
				c_lpChar->fishing(c_pData->fishing_key);
			break;

		case FISHING_GAME_SUBHEADER_QUIT:
			event_cancel(&c_lpChar->m_pkFishingEvent);
			fishing::FishingFail(c_lpChar);
			break;
		};
	}
}
#endif

void CInputMain::ItemGive(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGGiveItem* p = (TPacketCGGiveItem*) c_pData;
	LPCHARACTER to_ch = CHARACTER_MANAGER::instance().Find(p->dwTargetVID);

	if (to_ch)
		ch->GiveItem(to_ch, p->ItemPos);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, "659");
}
/*
void CInputMain::Hack(LPCHARACTER ch, const char * c_pData)
{
	TPacketCGHack * p = (TPacketCGHack *) c_pData;
	
	char buf[sizeof(p->szBuf)];
	strlcpy(buf, p->szBuf, sizeof(buf));

	sys_err("HACK_DETECT: %s %s", ch->GetName(), buf);

	ch->GetDesc()->SetPhase(PHASE_CLOSE);
}
*/

void CInputMain::Hack(LPCHARACTER ch, const char * c_pData)
{
	TPacketCGHack * p = (TPacketCGHack *) c_pData;
#ifdef ENABLE_CSHIELD
	if (!p || !p->szBuf || !strlen(p->szBuf))
		return;
	
	char buf[sizeof(p->szBuf)];
	strlcpy(buf, p->szBuf, sizeof(buf));

	char __escape_string[1024];
	DBManager::instance().EscapeString(__escape_string, sizeof(__escape_string), buf, strlen(p->szBuf));

	LogManager::instance().HackLog(__escape_string, ch);
	
	if (strcmp(p->szBuf, "CShield-ClientSide-ErrorCode: 240") == 0 ||
		strcmp(p->szBuf, "CShield-ClientSide-ErrorCode: 173") == 0)
		return;
#else
	char buf[sizeof(p->szBuf)];
	strlcpy(buf, p->szBuf, sizeof(buf));

	sys_err("HACK_DETECT: %s %s", ch->GetName(), buf);
#endif

	//ch->GetDesc()->SetPhase(PHASE_CLOSE);
}

void CInputMain::Refine(LPCHARACTER ch, const char* c_pData)
{
	const auto* p = reinterpret_cast<const TPacketCGRefine*>(c_pData);
	if (!p)
	{
		return;
	}

	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return;
	}

	if (ch->GetExchange()
		 || ch->IsOpenSafebox()
		 || ch->GetShop()
		 || ch->IsCubeOpen()
#ifdef __BL_67_ATTR__
		 || ch->Is67AttrOpen()
#endif
#ifdef __AURA_SYSTEM__
		 || ch->IsAuraRefineWindowOpen()
#endif
#ifdef __SASH_SYSTEM__
		 || ch->isSashOpened(true)
		 || ch->isSashOpened(false)
#endif
#ifdef USE_CRYSTAL_SYSTEM
		 || ch->IsShardCraftOpened()
#endif
	)
	{
		ch->ChatPacket(CHAT_TYPE_INFO,  "805");
		ch->ClearRefineMode();
		return;
	}

	if (p->type == 255)
	{
		// DoRefine Cancel
		ch->ClearRefineMode();
		return;
	}

	if (p->pos >= INVENTORY_MAX_NUM)
	{
		ch->ClearRefineMode();
		return;
	}

	LPITEM item = nullptr;

	switch (p->special_storage)
	{
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
		{
			item = ch->GetUpgradeInventoryItem(p->pos);
			break;
		}
		case BOOK_INVENTORY:
		{
			item = ch->GetBookInventoryItem(p->pos);
			break;
		}
		case STONE_INVENTORY:
		{
			item = ch->GetStoneInventoryItem(p->pos);
			break;
		}
#endif
		case INVENTORY:
		{
			item = ch->GetInventoryItem(p->pos);
			break;
		}
		default:
		{
			break;
		}
	}

	if (!item)
	{
		ch->ClearRefineMode();
		return;
	}

#ifdef __SOULBINDING_SYSTEM__
	if (item->IsBind() || item->IsUntilBind())
	{
		ch->ClearRefineMode();
		ch->ChatPacket(CHAT_TYPE_INFO, "You can't refine this item because is binded!");
		return;
	}
#endif

	ch->SetRefineTime();

	if (p->type == REFINE_TYPE_NORMAL)
	{
		sys_log (0, "refine_type_noraml");
		ch->DoRefine(item, false
#ifdef ENABLE_PITTY_REFINE
, p->seal_of_god
#endif
		);
	}
	else if (p->type == REFINE_TYPE_SCROLL
				|| p->type == REFINE_TYPE_HYUNIRON
				|| p->type == REFINE_TYPE_MUSIN
				|| p->type == REFINE_TYPE_BDRAGON
				|| p->type == REFINE_TYPE_STONE_OF_BLESS)
	{
		sys_log (0, "refine_type_scroll, ...");
		ch->DoRefineWithScroll(item
#ifdef ENABLE_PITTY_REFINE
, p->seal_of_god
#endif
		);
	}
	else if (p->type == REFINE_TYPE_MONEY_ONLY)
	{
		if (500 <= item->GetRefineSet())
		{
			LogManager::instance().HackLog("DEVIL_TOWER_REFINE_HACK", ch);
		}
		else
		{
#ifdef ENABLE_QUEEN_NETHIS
			if (SnakeLair::CSnk::Instance().IsSnakeMap(ch->GetMapIndex()))
			{
				if (get_global_time() > ch->GetQuestFlag("snake_lair.refine_time"))
				{
					if (ch->DoRefineSerpent(item
#ifdef ENABLE_PITTY_REFINE
, p->seal_of_god
#endif
					))
					{
						ch->SetQuestFlag("snake_lair.refine_time", get_global_time() + 60 * 60 * 24);
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "#Unk You can only be rewarded each 24h for the Serpent Temple Smith."); // Custom
				}
			}
			else
			{
#endif
			if (ch->GetQuestFlag("deviltower_zone.can_refine"))
			{
				ch->DoRefine(item, true
#ifdef ENABLE_PITTY_REFINE
, p->seal_of_god
#endif
				);
				ch->SetQuestFlag("deviltower_zone.can_refine", 0);
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "»ç±Í Å¸¿ö ¿Ï·á º¸»óÀº ÇÑ¹ø±îÁö »ç¿ë°¡´ÉÇÕ´Ï´Ù.");
			}
#ifdef ENABLE_QUEEN_NETHIS
			}
#endif
		}
	}

	ch->ClearRefineMode();
}

#ifdef __SASH_SYSTEM__
void CInputMain::Sash(LPCHARACTER pkChar, const char* c_pData)
{
	quest::PC * pPC = quest::CQuestManager::instance().GetPCForce(pkChar->GetPlayerID());
	if (pPC->IsRunning())
		return;
	
	TPacketSash * sPacket = (TPacketSash*) c_pData;
	switch (sPacket->subheader)
	{
		case SASH_SUBHEADER_CG_CLOSE:
			{
				pkChar->CloseSash();
			}
			break;
		case SASH_SUBHEADER_CG_ADD:
			{
				pkChar->AddSashMaterial(sPacket->tPos, sPacket->bPos);
			}
			break;
		case SASH_SUBHEADER_CG_REMOVE:
			{
				pkChar->RemoveSashMaterial(sPacket->bPos);
			}
			break;
		case SASH_SUBHEADER_CG_REFINE:
			{
				pkChar->RefineSashMaterials();
			}
			break;
		default:
			break;
	}
}
#endif


#ifdef ENABLE_IKASHOP_RENEWAL
#include "ikarus_shop.h"
#include "ikarus_shop_manager.h"

struct DecodeException : public std::exception {
	const char* what() const noexcept override{
		return "Decode failed receiving ikashop packet";
	}
};

template <class T>
const T& Decode(const char*& data, int& left, int& len)
{
	if(left < sizeof(T))
		throw DecodeException();

	auto obj = reinterpret_cast<const T*>(data);
	data += sizeof(T);
	len  += sizeof(T);
	left -= sizeof(T);
	return *obj;
}

int IkarusShopPacketCreateNewShop(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	ikashop::GetManager().RecvShopCreateNewClientPacket(ch);
	return extra;
}

int IkarusShopPacketForceCloseShop(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	ikashop::GetManager().RecvShopForceCloseClientPacket(ch);
	return 0;
}

int IkarusShopPacketOpenShop(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGShopOpen>(data, left, extra);
	ikashop::GetManager().RecvShopOpenClientPacket(ch, pack.ownerid);
	return extra;
}

int IkarusShopPacketOpenShowOwner(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	ikashop::GetManager().RecvShopOpenMyShopClientPacket(ch);
	return 0;
}

int IkarusShopPacketBuyItem(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGShopBuyItem>(data, left, extra);
	ikashop::GetManager().RecvShopBuyItemClientPacket(ch, pack.ownerid, pack.itemid, pack.searching, pack.seenprice);
	return extra;
}

int IkarusShopPacketAddItem(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGAddItem>(data, left, extra);
	ikashop::GetManager().RecvShopAddItemClientPacket(ch, pack.pos, pack.price, pack.destpos);
	return extra;
}

int IkarusShopPacketRemoveItem(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGRemoveItem>(data, left, extra);
	ikashop::GetManager().RecvShopRemoveItemClientPacket(ch, pack.itemid);
	return extra;
}

int IkarusShopPacketEditItem(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGEditItem>(data, left, extra);
	ikashop::GetManager().RecvShopEditItemClientPacket(ch, pack.itemid, pack.price);
	return extra;
}

int IkarusShopPacketFilterRequest(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGFilterRequest>(data, left, extra);
	ikashop::GetManager().RecvShopFilterRequestClientPacket(ch, pack.filter);
	return extra;
}

int IkarusShopPacketSearchFillRequest(LPCHARACTER ch, const char* data, int left)
{
	ikashop::GetManager().RecvShopSearchFillRequestClientPacket(ch);
	return 0;
}

int IkarusShopPacketCreateOffer(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGOfferCreate>(data, left, extra);
	ikashop::GetManager().RecvShopCreateOfferClientPacket(ch, pack.offer);
	return extra;
}

int IkarusShopPacketAcceptOffer(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGOfferAccept>(data, left, extra);
	ikashop::GetManager().RecvShopAcceptOfferClientPacket(ch, pack.offerid);
	return extra;
}

int IkarusShopPacketOfferCancel(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGOfferCancel>(data, left, extra);
	ikashop::GetManager().RecvShopCancelOfferClientPacket(ch, pack.offerid, pack.ownerid);
	return extra;
}

int IkarusShopPacketOfferListRequest(LPCHARACTER ch)
{
	ikashop::GetManager().RecvOfferListRequestPacket(ch);
	return 0;
}

int IkarusShopPacketOpenSafebox(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	ikashop::GetManager().RecvShopSafeboxOpenClientPacket(ch);
	return 0;
}

int IkarusShopPacketCloseMyShopBoard(LPCHARACTER ch)
{
	ikashop::GetManager().RecvCloseMyShopBoardClientPacket(ch);
	return 0;
}

int IkarusShopPacketCloseOfferList(LPCHARACTER ch)
{
	ikashop::GetManager().RecvCloseOfferListClientPacket(ch);
	return 0;
}

int IkarusShopPacketCloseShopGuest(LPCHARACTER ch)
{
	ikashop::GetManager().RecvCloseShopGuestClientPacket(ch);
	return 0;
}

int IkarusShopPacketGetItemSafebox(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGShopSafeboxGetItem>(data, left, extra);
	ikashop::GetManager().RecvShopSafeboxGetItemClientPacket(ch, pack.itemid);
	return extra;

}

int IkarusShopPacketGetValutesSafebox(LPCHARACTER ch, const char* data, int left)
{
	ikashop::GetManager().RecvShopSafeboxGetValutesClientPacket(ch);
	return 0;
}

int IkarusShopPacketCloseSafebox(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	ikashop::CShopManager& rManager = ikashop::GetManager();
	rManager.RecvShopSafeboxCloseClientPacket(ch);
	return 0;
}

int IkarusShopPacketListRequest(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	ikashop::CShopManager& rManager = ikashop::GetManager();
	rManager.RecvAuctionListRequestClientPacket(ch);
	return 0;
}

int IkarusShopPacketOpenAuctionRequest(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGAuctionOpenRequest>(data, left, extra);
	ikashop::GetManager().RecvAuctionOpenRequestClientPacket(ch, pack.ownerid);
	return extra;
}

int IkarusShopPacketOpenMyAuctionRequest(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	ikashop::GetManager().RecvMyAuctionOpenRequestClientPacket(ch);
	return 0;
}

int IkarusShopPacketCloseMyAuction(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	ikashop::GetManager().RecvMyAuctionCloseClientPacket(ch);
	return 0;
}

int IkarusShopPacketCancelMyAuction(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	ikashop::GetManager().RecvMyAuctionCancelClientPacket(ch);
	return 0;
}

int IkarusShopPacketCreateAuction(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGAuctionCreate>(data, left, extra);
	ikashop::GetManager().RecvAuctionCreateClientPacket(ch, pack.startprice, pack.pos);
	return extra;
}

int IkarusShopPacketAddOffer(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGAuctionAddOffer>(data, left, extra);
	ikashop::GetManager().RecvAuctionAddOfferClientPacket(ch, pack.ownerid, pack.price);
	return extra;
}

int IkarusShopPacketExitFromAuction(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGAuctionExitFrom>(data, left, extra);
	ikashop::GetManager().RecvAuctionExitFromAuction(ch, pack.ownerid);
	return extra;
}

#ifdef ENABLE_IKASHOP_ENTITIES
int IkarusShopPacketClickEntity(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGShopClickEntity>(data, left, extra);

	ikashop::GetManager().RecvShopClickEntityClientPacket(ch, pack.vid);
	return extra;
}

#if defined(EXTEND_IKASHOP_PRO) && !defined(USE_IKASHOP_UI_RENEWED)
int IkarusShopPacketMoveEntity(LPCHARACTER ch)
{
	ikashop::GetManager().RecvMoveShopEntityClientPacket(ch);
	return 0;
}
#endif

#ifdef USE_IKASHOP_UI_RENEWED
int IkarusShopPacketTeleportToEntity(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGShopClickEntity>(data, left, extra);

	ikashop::GetManager().RecvTeleportToShopEntityClientPacket(ch, pack.vid);
	return 0;
}
#endif
#endif

#ifdef EXTEND_IKASHOP_PRO
int IkarusShopPacketNotificationListRequest(LPCHARACTER ch)
{
	ikashop::GetManager().SendNotificationListClientPacket(ch);
	ch->SetLookingNotificationList(true);
	return 0;
}

int IkarusShopPacketNotificationListClose(LPCHARACTER ch)
{
	ch->SetLookingNotificationList(false);
	return 0;
}
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
int IkarusShopPacketPriceAverageRequest(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGPriceAverageRequest>(data, left, extra);

	ikashop::GetManager().RecvPriceAverageRequestClientPacket(ch, pack.vnum, pack.count);
	return extra;
}

int IkarusShopPacketShopDecorationUse(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& index = Decode<int>(data, left, extra);

	ikashop::GetManager().RecvShopDecorationUseClientPacket(ch, index);
	return extra;
}

int IkarusShopPacketShopMoveItem(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& subpack = Decode<ikashop::TSubPacketCGShopMoveItem>(data, left, extra);

	ikashop::GetManager().RecvShopMoveItemClientPacket(ch, subpack.srcPos, subpack.destPos);
	return extra;
}
#endif

#ifdef USE_OFFLINESHOP_SEARCH_V2
int OfflineshopRecvShopSearchOpen(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pack = Decode<ikashop::TSubPacketCGFilterRequest>(data, left, extra);

	auto& shopMgr = ikashop::GetManager();

	const auto pid = ch ? ch->GetPlayerID() : 0;
	if (pid != 0)
	{
		if (!ch->IkarusShopFloodCheck(ikashop::SHOP_ACTION_WEIGHT_FILTER_REQUEST))
		{
			return extra;
		}

		if (!shopMgr.CheckSearchCooldown(pid))
		{
			return extra;
		}
	}

	if (ch->SetShopSearchFilterInfo(pack.filter))
	{
		shopMgr.RecvShopSearchOpen(ch);
	}

	return extra;
}

int OfflineshopRecvShopSearchChangePage(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& pageIdx = Decode<int32_t>(data, left, extra);

	auto& shopMgr = ikashop::GetManager();
	shopMgr.RecvShopSearchChangePage(ch, pageIdx);
	return extra;
}

int OfflineshopRecvShopSearchBuyItem(LPCHARACTER ch, const char* data, int left)
{
	int extra = 0;
	const auto& count = Decode<size_t>(data, left, extra);

	if (count < 1 || count > OFFLINESHOP_MAX_ITEM_PER_SEARCH_PAGE)
	{
		return -1;
	}

	std::vector<ikashop::TSubPacketCGShopBuyItem> vecItems;
	vecItems.reserve(count);

	for (size_t i = 0; i < count; ++i)
	{
		vecItems.emplace_back(Decode<ikashop::TSubPacketCGShopBuyItem>(data, left, extra));
	}

	auto& shopMgr = ikashop::GetManager();
	shopMgr.RecvShopSearchBuyItem(ch, vecItems);
	return extra;
}
#endif

int IkarusShopPacket(const char* data , LPCHARACTER ch, int iBufferLeft)
{
	BYTE subheader = 255;

	try
	{
		int extra = 0;
		const auto& pack = Decode<TPacketCGNewIkarusShop>(data, iBufferLeft, extra);
		subheader = pack.subheader;

		switch (pack.subheader)
		{

		case ikashop::SUBHEADER_CG_SHOP_CREATE_NEW:
			return IkarusShopPacketCreateNewShop(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_FORCE_CLOSE:
			return IkarusShopPacketForceCloseShop(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_OPEN:
			return IkarusShopPacketOpenShop(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_OPEN_OWNER:
			return IkarusShopPacketOpenShowOwner(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_BUY_ITEM:
			return IkarusShopPacketBuyItem(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_ADD_ITEM:
			return IkarusShopPacketAddItem(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_REMOVE_ITEM:
			return IkarusShopPacketRemoveItem(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_EDIT_ITEM:
			return IkarusShopPacketEditItem(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_FILTER_REQUEST:
			return IkarusShopPacketFilterRequest(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_SEARCH_FILL_REQUEST:
			return IkarusShopPacketSearchFillRequest(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_OFFER_CREATE:
			return IkarusShopPacketCreateOffer(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_OFFER_ACCEPT:
			return IkarusShopPacketAcceptOffer(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_REQUEST_OFFER_LIST:
			return IkarusShopPacketOfferListRequest(ch);

		case ikashop::SUBHEADER_CG_SHOP_OFFER_CANCEL:
			return IkarusShopPacketOfferCancel(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_SAFEBOX_OPEN:
			return IkarusShopPacketOpenSafebox(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_SAFEBOX_GET_ITEM:
			return IkarusShopPacketGetItemSafebox(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_SAFEBOX_GET_VALUTES:
			return IkarusShopPacketGetValutesSafebox(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_SHOP_SAFEBOX_CLOSE:
			return IkarusShopPacketCloseSafebox(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_AUCTION_LIST_REQUEST:
			return IkarusShopPacketListRequest(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_AUCTION_OPEN_REQUEST:
			return IkarusShopPacketOpenAuctionRequest(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_MY_AUCTION_OPEN_REQUEST:
			return IkarusShopPacketOpenMyAuctionRequest(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_MY_AUCTION_CLOSE:
			return IkarusShopPacketCloseMyAuction(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_MY_AUCTION_CANCEL:
			return IkarusShopPacketCancelMyAuction(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_CREATE_AUCTION:
			return IkarusShopPacketCreateAuction(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_AUCTION_ADD_OFFER:
			return IkarusShopPacketAddOffer(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_EXIT_FROM_AUCTION:
			return IkarusShopPacketExitFromAuction(ch, data, iBufferLeft);

		case ikashop::SUBHEADER_CG_CLOSE_MY_SHOP_BOARD:
			return IkarusShopPacketCloseMyShopBoard(ch);

		case ikashop::SUBHEADER_CG_CLOSE_OFFER_LIST:
			return IkarusShopPacketCloseOfferList(ch);

		case ikashop::SUBHEADER_CG_CLOSE_SHOP_GUEST:
			return IkarusShopPacketCloseShopGuest(ch);

#ifdef ENABLE_IKASHOP_ENTITIES
		case ikashop::SUBHEADER_CG_CLICK_ENTITY:
			return IkarusShopPacketClickEntity(ch, data, iBufferLeft);
#if defined(EXTEND_IKASHOP_PRO) && !defined(USE_IKASHOP_UI_RENEWED)
		case ikashop::SUBHEADER_CG_MOVE_SHOP_ENTITY:
			return IkarusShopPacketMoveEntity(ch);
#endif
#ifdef USE_IKASHOP_UI_RENEWED
		case ikashop::SUBHEADER_CG_TELEPORT_TO_SHOP_ENTITY:
		{
			return IkarusShopPacketTeleportToEntity(ch, data, iBufferLeft);
		}
#endif
#endif
#ifdef EXTEND_IKASHOP_PRO
		case ikashop::SUBHEADER_CG_NOTIFICATION_LIST_REQUEST:
			return IkarusShopPacketNotificationListRequest(ch);
		case ikashop::SUBHEADER_CG_NOTIFICATION_LIST_CLOSE:
			return IkarusShopPacketNotificationListClose(ch);
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		case ikashop::SUBHEADER_CG_PRICE_AVERAGE_REQUEST:
			return IkarusShopPacketPriceAverageRequest(ch, data, iBufferLeft);
		case ikashop::SUBHEADER_CG_SHOP_DECORATION_USE:
			return IkarusShopPacketShopDecorationUse(ch, data, iBufferLeft);
		case ikashop::SUBHEADER_CG_SHOP_MOVE_ITEM:
			return IkarusShopPacketShopMoveItem(ch, data, iBufferLeft);
#endif
#ifdef USE_OFFLINESHOP_SEARCH_V2
		case ikashop::SUBHEADER_CG_SHOP_SEARCH_OPEN:
		{
			return OfflineshopRecvShopSearchOpen(ch, data, iBufferLeft);
		}
		case ikashop::SUBHEADER_CG_SHOP_SEARCH_CHANGE_PAGE:
		{
			return OfflineshopRecvShopSearchChangePage(ch, data, iBufferLeft);
		}
		case ikashop::SUBHEADER_CG_SHOP_SEARCH_BUY_ITEM:
		{
			return OfflineshopRecvShopSearchBuyItem(ch, data, iBufferLeft);
		}
#endif
		default:
			sys_err("UNKNOWN SUBHEADER %d ", subheader);
			return -1;
		}
	}

	catch(const DecodeException& e)
	{
		sys_err("FAILED TO DECODE SUBHEADER %u", subheader);
		return -1;
	}
}
#endif

#ifdef ENABLE_CALENDAR_SYSTEM

void CInputMain::CalendarSystemSend(LPCHARACTER ch, const char * data)
{

	struct packet_send_calendar_system * pinfo = (struct packet_send_calendar_system *) data;
	switch (pinfo->subheader)
	{
		case CALENDAR_SUB_HEADER_OPEN:
		{
			ch->OpenCalendarSystem();

		}
		break;

		case CALENDAR_SUB_HEADER_CLOSE:
		{
			if (ch->IsOpenCalendar()){
				ch->SetOpenCalendarSystem(false);
			}

		}
		break;

		case CALENDAR_SUB_HEADER_SEND_EVENT:
		{
			if (!ch->IsGM()){
				return;
			}
			
			
			int day = pinfo->day;
			int index_event = pinfo->index_event;
			char hora_inicio[6];
			char hora_final[6];
			
			strlcpy(hora_inicio, pinfo->hora_inicio, sizeof(hora_inicio));
			strlcpy(hora_final, pinfo->hora_final, sizeof(hora_final));

			ch->SendEvents(day,index_event,hora_inicio,hora_final);
			

		}
		break;

	}
	return;
}
#endif

#ifdef ENABLE_DUNGEON_INFO
void CInputMain::DungeonInfoSend(LPCHARACTER ch, const char * data)
{
	
	struct packet_send_dungeon_info_system * pinfo = (struct packet_send_dungeon_info_system *) data;
	switch (pinfo->subheader)
	{
		case DUNGEON_INFO_SUB_HEADER_OPEN:
		{
			CDungeonInfoExtern::instance().Open(ch);
		}
		break;

		case DUNGEON_INFO_SUB_HEADER_TELEPORT:
		{
			int index_teleport = pinfo->index;
			CDungeonInfoExtern::instance().Teleport(ch,index_teleport);
		}
		break;

		case DUNGEON_INFO_SUB_HEADER_MISION:
		{
			int index = pinfo->index;
			CDungeonInfoExtern::instance().GetLlaveDungeon(ch,index);

		}	
		break;

		case DUNGEON_INFO_SUB_HEADER_RANKING:
		{
			int index = pinfo->index;
			CDungeonInfoExtern::instance().SetDateDungeonRanking(ch, index);
		}
	}
}
#endif
#ifdef ENABLE_DROP_ITEM_WORLDARD 
void CInputMain::DropItemInfoSend(LPCHARACTER ch, const char* data)
{
	struct packet_send_drop_item_system * pinfo = (struct packet_send_drop_item_system *) data;

	switch (pinfo->subheader)
	{
		case DROP_ITEM_SUB_HEADER_OPEN:
		{
			CDropItem::instance().Open(ch);
		}
		break;

		case DROP_ITEM_SUB_HEADER_SEARCH:
		{
			int vnum_item = pinfo->vnum_item;
			CDropItem::instance().Search(ch,vnum_item);
		}
		break;

	}
}
#endif
#ifdef __AURA_SYSTEM__
size_t GetAuraSubPacketLength(const EPacketCGAuraSubHeader& SubHeader)
{
	switch (SubHeader)
	{
	case AURA_SUBHEADER_CG_REFINE_CHECKIN:
		return sizeof(TSubPacketCGAuraRefineCheckIn);
	case AURA_SUBHEADER_CG_REFINE_CHECKOUT:
		return sizeof(TSubPacketCGAuraRefineCheckOut);
	case AURA_SUBHEADER_CG_REFINE_ACCEPT:
		return sizeof(TSubPacketCGAuraRefineAccept);
	case AURA_SUBHEADER_CG_REFINE_CANCEL:
		return 0;
	}

	return 0;
}

int CInputMain::Aura(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	if (uiBytes < sizeof(TPacketCGAura))
		return -1;

	const TPacketCGAura* pinfo = reinterpret_cast<const TPacketCGAura*>(data);
	const char* c_pData = data + sizeof(TPacketCGAura);

	uiBytes -= sizeof(TPacketCGAura);

	const EPacketCGAuraSubHeader SubHeader = static_cast<EPacketCGAuraSubHeader>(pinfo->bSubHeader);
	const size_t SubPacketLength = GetAuraSubPacketLength(SubHeader);
	if (uiBytes < SubPacketLength)
	{
		sys_err("invalid aura subpacket length (sublen %d size %u buffer %u)", SubPacketLength, sizeof(TPacketCGAura), uiBytes);
		return -1;
	}

	switch (SubHeader)
	{
		case AURA_SUBHEADER_CG_REFINE_CHECKIN:
			{
				const TSubPacketCGAuraRefineCheckIn* sp = reinterpret_cast<const TSubPacketCGAuraRefineCheckIn*>(c_pData);
				ch->AuraRefineWindowCheckIn(sp->byAuraRefineWindowType, sp->AuraCell, sp->ItemCell);
			}
			return SubPacketLength;
		case AURA_SUBHEADER_CG_REFINE_CHECKOUT:
			{
				const TSubPacketCGAuraRefineCheckOut* sp = reinterpret_cast<const TSubPacketCGAuraRefineCheckOut*>(c_pData);
				ch->AuraRefineWindowCheckOut(sp->byAuraRefineWindowType, sp->AuraCell);
			}
			return SubPacketLength;
		case AURA_SUBHEADER_CG_REFINE_ACCEPT:
			{
				const TSubPacketCGAuraRefineAccept* sp = reinterpret_cast<const TSubPacketCGAuraRefineAccept*>(c_pData);
				ch->AuraRefineWindowAccept(sp->byAuraRefineWindowType);
			}
			return SubPacketLength;
		case AURA_SUBHEADER_CG_REFINE_CANCEL:
			{
				ch->AuraRefineWindowClose();
			}
			return SubPacketLength;
	}

	return 0;
}
#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
void CInputMain::CubeRenewalSend(LPCHARACTER ch, const char* data)
{
	struct packet_send_cube_renewal * pinfo = (struct packet_send_cube_renewal *) data;
	switch (pinfo->subheader)
	{
		case CUBE_RENEWAL_SUB_HEADER_MAKE_ITEM:
		{

			int index_item = pinfo->index_item;
			int count_item = pinfo->count_item;

			Cube_Make(ch,index_item,count_item);
		}
		break;

		case CUBE_RENEWAL_SUB_HEADER_CLOSE:
		{
			Cube_close(ch);
		}
		break;
	}
}
#endif

#ifdef ENABLE_CSHIELD
void CInputMain::CShieldReceive(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGCShield* p = (TPacketCGCShield*)c_pData;

	if (!ch)
		return;

	if (!c_pData)
		return;

	if (strlen(p->key) < 64)
		return;

	ch->GetCShield()->SetKeyStored(p->key);
}
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
void CInputMain::SwitchBotSend(LPCHARACTER ch, const char * data)
{
	struct packet_send_switchbot * pinfo = (struct packet_send_switchbot *) data;
	switch (pinfo->subheader)
	{
		case SWITCH_SUB_HEADER_CHANGE:
		{
			if (CSwitchBonus::Instance().CheckElementsSB(ch))
				return;

			CSwitchBonus::Instance().ChangeSB(ch,pinfo->aAttr);
		}
		break;

		case SWITCH_SUB_HEADER_OPEN:
		{
			CSwitchBonus::Instance().OpenSB(ch);
		}
		break;

		case SWITCH_SUB_HEADER_CLOSE:
		{
			CSwitchBonus::Instance().CloseSB(ch);
		}
		break;
	}
}
#endif

#ifdef USE_CAPTCHA_SYSTEM
void CInputMain::Captcha(LPCHARACTER ch, const char* c_pData)
{
	if (!ch)
	{
		return;
	}

	const auto* pInfo = reinterpret_cast<const TPacketCGCaptcha*>(c_pData);
	if (!pInfo)
	{
		return;
	}

	ch->ValidateCaptcha(pInfo->code);
}
#endif

#ifdef USE_CURRENCY_EXCHANGE
void CInputMain::CurrencyExchange(LPCHARACTER ch, const char* data)
{
	if (!ch)
	{
		return;
	}

	const auto* info = reinterpret_cast<const TPacketCGCurrencyExchange*>(data);
	if (!info)
	{
		return;
	}

	ch->OnCurrencyExchange(info->llAmount, info->bSubHeader);
}
#endif

#ifdef ENABLE_VOICE_CHAT
struct FuncSendVoiceChatPacket
{
	FuncSendVoiceChatPacket(LPCHARACTER ch, const TVoiceChatPacket& packet, const char* audioData)
	{
		m_Character = ch;
		m_Packet = packet;
		m_AudioData = audioData;
		m_PlayerCount = 0;
	}

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			LPDESC d = ch->GetDesc();

			if (!d)
				return;

#ifdef _WIN32
			if (quest::CQuestManager::Instance().GetEventFlag("voice_chat_hear_yourself") == 0)
#endif
			if (ch == m_Character)
				return;

			if (ch->VoiceChatIsBlock(m_Packet.type))
				return;

			// If you have messenger block
			//if (BlocklistManager::instance().IsBlocked(m_Character->GetName(), ch->GetName()))
			//	return;

			//if (BlocklistManager::instance().IsBlocked(ch->GetName(), m_Character->GetName()))
			//	return;

			int distance = DISTANCE_APPROX(m_Character->GetX() - ch->GetX(), m_Character->GetY() - ch->GetY());
			if (m_Packet.type == VOICE_CHAT_TYPE_LOCAL && distance > 5'000)
				return;

			m_Packet.distance = static_cast<uint16_t>(distance);
			d->BufferedPacket(&m_Packet, sizeof(m_Packet));
			d->Packet(m_AudioData, m_Packet.dataSize);

			m_PlayerCount++;
		}
	}

	int m_PlayerCount;

private:
	LPCHARACTER m_Character;
	TVoiceChatPacket m_Packet;
	const char* m_AudioData;
};

int CInputMain::HandleVoiceChatPacket(const LPCHARACTER ch, const char* c_pData, size_t uiBytes)
{
	LPDESC d = ch->GetDesc();
	TVoiceChatPacket pack = *(TVoiceChatPacket*)c_pData;
	pack.header = HEADER_GC_VOICE_CHAT;
	pack.vid = ch->GetVID();
	strlcpy(pack.name, ch->GetName(), sizeof(pack.name));

	if (!pack.dataSize)
		return 0;

	if (pack.dataSize != pack.size - sizeof(TVoiceChatPacket))
		return -1;

	if (uiBytes < pack.size)
	{
		// sys_err("Not enough bytes. req %u, avail %u recv from %s", pack.size, uiBytes, ch->GetName());
		return -1;
	}

	if (!pack.type || pack.type >= VOICE_CHAT_TYPE_MAX_NUM || ch->FindAffect(AFFECT_BLOCK_CHAT))
		return pack.dataSize;

	if (ch->GetLevel() < VOICE_CHAT_MIN_LEVEL
#ifdef __BL_BATTLE_ROYALE__
		&& !ch->IsBattleRoyale()
#endif
	)
	{
		static std::unordered_map<uint32_t, int> toNotify;
		int& nextNotification = toNotify[ch->GetPlayerID()];
		if (nextNotification < thecore_pulse())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You need level %d to use the voice chat."), VOICE_CHAT_MIN_LEVEL);
			nextNotification = thecore_pulse() + PASSES_PER_SEC(1);
		}
		return pack.dataSize;
	}

	if (ch->VoiceChatIsBlock(pack.type))
		return pack.dataSize;

	if (g_bVoiceChatDisabled)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "voice_chat_disabled 1");
		return pack.dataSize;
	}

	FuncSendVoiceChatPacket f(ch, pack, c_pData + sizeof(pack));

	switch (pack.type)
	{
	case VOICE_CHAT_TYPE_LOCAL:
	{
		if (LPSECTREE tree = ch->GetSectree())
		{
			ch->EffectPacket(SE_VOICE_CHAT, d);
			tree->ForEachAround(f);
		}
	} break;

	case VOICE_CHAT_TYPE_PARTY:
	{
		if (LPPARTY party = ch->GetParty())
		{
			ch->EffectPacket(SE_VOICE_CHAT, d);
			party->ForEachOnMapMember(f, ch->GetMapIndex());
		}
	} break;

	case VOICE_CHAT_TYPE_GUILD:
	{
		if (CGuild* guild = ch->GetGuild())
		{
			ch->EffectPacket(SE_VOICE_CHAT, d);
			guild->ForEachOnMapMember(f, ch->GetMapIndex());
		}
	} break;

	default:
		sys_err("Unhandled voice type %u", pack.type);
		break;
	}

	return pack.dataSize;
}
#endif

#ifdef USE_CRYSTAL_SYSTEM
int CInputMain::RecvShardCraft(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	if (uiBytes < sizeof(TPacketCGShardCraft))
	{
		return -1;
	}

	const auto* p = reinterpret_cast<const TPacketCGShardCraft*>(data);
	if (!p)
	{
		return -1;
	}

	LPDESC d = ch ? ch->GetDesc() : nullptr;
	if (!d)
	{
		return -1;
	}

	switch (p->bSubHeader)
	{
		case SUBHEADER_SHARD_CRAFT_CLOSE:
		case SUBHEADER_SHARD_CRAFT_REFINE:
		{
			ch->SendShardCraft(p->bSubHeader);
			return 0;
		}
		case SUBHEADER_SHARD_CRAFT_ADD:
		{
			uiBytes -= sizeof(TPacketCGShardCraft);

			if (uiBytes < sizeof(TShardCraftAdd))
			{
				sys_err("RecvShardCraft >> invalid size (left: %d, expected: %d) -> subHeader (%u).", uiBytes, sizeof(TShardCraftAdd), p->bSubHeader);
				return -1;
			}

			data += sizeof(TPacketCGShardCraft);

			const auto* p2 = reinterpret_cast<const TShardCraftAdd*>(data);
			if (!p2)
			{
				sys_err("RecvShardCraft >> cannot get the subPacket -> subHeader (%u).", p->bSubHeader);
				return -1;
			}

			ch->SendShardCraft(p->bSubHeader, p2->iSourceSlot, p2->iTargetSlot);
			return sizeof(TShardCraftAdd);
		}
		case SUBHEADER_SHARD_CRAFT_OUT:
		{
			uiBytes -= sizeof(TPacketCGShardCraft);

			if (uiBytes < sizeof(TShardCraftOut))
			{
				sys_err("RecvShardCraft >> invalid size (left: %d, expected: %d) -> subHeader (%u).", uiBytes, sizeof(TShardCraftOut), p->bSubHeader);
				return -1;
			}

			data += sizeof(TPacketCGShardCraft);

			const auto* p2 = reinterpret_cast<const TShardCraftOut*>(data);
			if (!p2)
			{
				sys_err("RecvShardCraft >> cannot get the subPacket -> subHeader (%u).", p->bSubHeader);
				return -1;
			}

			ch->SendShardCraft(p->bSubHeader, -1, p2->iTargetSlot);
			return sizeof(TShardCraftOut);
		}
		default:
		{
			sys_err("RecvShardCraft >> unknown subHeader (%u).", p->bSubHeader);
			return -1;
		}
	}
}
#endif

int CInputMain::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)
{
	LPCHARACTER ch;

	if (!(ch = d->GetCharacter()))
	{
		sys_err("no character on desc");
		d->SetPhase(PHASE_CLOSE);
#ifdef ENABLE_ANTI_PACKET_FLOOD
		return -1;
#else
		return (0);
#endif
	}

	int iExtraLen = 0;

#ifdef ENABLE_FLOOD_PRETECTION
	if (ch && ch->IsPC())
	{
		if (get_global_time() < ch->analyze_protect)
		{
			ch->analyze_protect_count = ch->analyze_protect_count + 1;
			//ch->ChatPacket(CHAT_TYPE_INFO, "<test server> analyze_protect_count. Count %d", ch>analyze_protect_count);

			if (ch->analyze_protect_count >= 300)
			{
				ch->analyze_protect_count = 0;
				d->SetPhase(PHASE_CLOSE);
				return (0);
			}
		}
		else
			ch->analyze_protect_count = 0;

		ch->analyze_protect = get_global_time() + 1;
	}
#endif

#ifdef ENABLE_ANTI_PACKET_FLOOD
	if (bHeader != HEADER_CG_ITEM_USE
#if defined(__AUTOMATIC_HUNTING_SYSTEM__)
		&& bHeader != HEADER_CG_TARGET
#endif
		)
	{
		if (thecore_pulse() > ch->GetPacketAntiFloodPulse() + PASSES_PER_SEC(1))
		{
			ch->SetPacketAntiFloodCount(0);
			ch->SetPacketAntiFloodPulse(thecore_pulse());
		}

		if (ch->IncreasePacketAntiFloodCount() >= 250)
		{
			sys_err("<Flood packet> name(%s) header(%u) host(%s)", ch->GetName(), bHeader, inet_ntoa(d->GetAddr().sin_addr));
			ch->GetDesc()->DelayedDisconnect(0);
			return -1;
		}
	}
#endif


	if (test_server && bHeader != HEADER_CG_MOVE)
		sys_log(0, "CInputMain::Analyze() ==> Header [%d] ", bHeader);

	switch (bHeader)
	{
#ifdef ENABLE_VOICE_CHAT
		case HEADER_CG_VOICE_CHAT:
		{
			if ((iExtraLen = HandleVoiceChatPacket(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
		}
		break;
#endif

		case HEADER_CG_PONG:
			Pong(d);
			break;

		case HEADER_CG_TIME_SYNC:
			Handshake(d, c_pData);
			break;

		case HEADER_CG_CHAT:
			if (test_server)
			{
				char* pBuf = (char*)c_pData;
				sys_log(0, "%s", pBuf + sizeof(TPacketCGChat));
			}

			if ((iExtraLen = Chat(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_CG_WHISPER:
			if ((iExtraLen = Whisper(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_CG_MOVE:
			Move(ch, c_pData);
			// @fixme103 (removed CheckClientVersion since useless in here)
			break;

		case HEADER_CG_CHARACTER_POSITION:
			Position(ch, c_pData);
			break;

		case HEADER_CG_ITEM_USE:
			if (!ch->IsObserverMode())
				ItemUse(ch, c_pData);
			break;

		case HEADER_CG_ITEM_DROP:
			if (!ch->IsObserverMode())
			{
				ItemDrop(ch, c_pData);
			}
			break;

		case HEADER_CG_ITEM_DROP2:
			if (!ch->IsObserverMode())
				ItemDrop2(ch, c_pData);
			break;
			
#ifdef __AURA_SYSTEM__
		case HEADER_CG_AURA:
			if ((iExtraLen = Aura(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#endif

		case HEADER_CG_ITEM_MOVE:
			if (!ch->IsObserverMode())
				ItemMove(ch, c_pData);
			break;

#ifdef ENABLE_SELL_ITEM
		case HEADER_CG_ITEM_SELL:
			if (!ch->IsObserverMode())
				ItemSell(ch, c_pData);
			break;
#endif

#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
		case HEADER_CG_ITEMS_PICKUP:
			if ((iExtraLen = ItemsPickup(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#endif 
		case HEADER_CG_ITEM_PICKUP:
			if (!ch->IsObserverMode())
				ItemPickup(ch, c_pData);
			break;

		case HEADER_CG_ITEM_USE_TO_ITEM:
			if (!ch->IsObserverMode())
				ItemToItem(ch, c_pData);
			break;

		case HEADER_CG_ITEM_GIVE:
			if (!ch->IsObserverMode())
				ItemGive(ch, c_pData);
			break;

		case HEADER_CG_EXCHANGE:
			if (!ch->IsObserverMode())
				Exchange(ch, c_pData);
			break;

		case HEADER_CG_ATTACK:
		case HEADER_CG_SHOOT:
			if (!ch->IsObserverMode())
			{
				Attack(ch, bHeader, c_pData);
			}
			break;

		case HEADER_CG_USE_SKILL:
			if (!ch->IsObserverMode())
				UseSkill(ch, c_pData);
			break;

		case HEADER_CG_QUICKSLOT_ADD:
			QuickslotAdd(ch, c_pData);
			break;

		case HEADER_CG_QUICKSLOT_DEL:
			QuickslotDelete(ch, c_pData);
			break;

		case HEADER_CG_QUICKSLOT_SWAP:
			QuickslotSwap(ch, c_pData);
			break;
#ifdef __SEND_TARGET_INFO__
		case HEADER_CG_TARGET_INFO_LOAD:
			{
				TargetInfoLoad(ch, c_pData);
			}
			break;
#endif
		case HEADER_CG_SHOP:
			{
#ifdef __BL_BATTLE_ROYALE__
			if (ch->IsBattleRoyale())
				return 0;
#endif
			if ((iExtraLen = Shop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			}
			break;

		case HEADER_CG_MESSENGER:
			if ((iExtraLen = Messenger(ch, c_pData, m_iBufferLeft))<0)
				return -1;
			break;

		case HEADER_CG_ON_CLICK:
			OnClick(ch, c_pData);
			break;

		case HEADER_CG_SYNC_POSITION:
			if ((iExtraLen = SyncPosition(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_CG_ADD_FLY_TARGETING:
		case HEADER_CG_FLY_TARGETING:
			FlyTarget(ch, c_pData, bHeader);
			break;

		case HEADER_CG_SCRIPT_BUTTON:
			ScriptButton(ch, c_pData);
			break;

			// SCRIPT_SELECT_ITEM
		case HEADER_CG_SCRIPT_SELECT_ITEM:
			ScriptSelectItem(ch, c_pData);
			break;
			// END_OF_SCRIPT_SELECT_ITEM

		case HEADER_CG_SCRIPT_ANSWER:
			ScriptAnswer(ch, c_pData);
			break;

		case HEADER_CG_QUEST_INPUT_STRING:
			QuestInputString(ch, c_pData);
			break;

		case HEADER_CG_QUEST_INPUT_LONG_STRING:
			QuestInputLongString(ch, c_pData);
			break;
			
		case HEADER_CG_QUEST_CONFIRM:
			QuestConfirm(ch, c_pData);
			break;

#if defined(__BL_67_ATTR__)
		case HEADER_CG_67_ATTR:
			Attr67(ch, c_pData);
			break;
			
		case HEADER_CG_CLOSE_67_ATTR:
			Attr67Close(ch, c_pData);
			break;
#endif

		case HEADER_CG_TARGET:
			Target(ch, c_pData);
			break;

		case HEADER_CG_WARP:
			Warp(ch, c_pData);
			break;

		case HEADER_CG_SAFEBOX_CHECKIN:
			SafeboxCheckin(ch, c_pData);
			break;

		case HEADER_CG_SAFEBOX_CHECKOUT:
			SafeboxCheckout(ch, c_pData, false);
			break;

		case HEADER_CG_SAFEBOX_ITEM_MOVE:
			SafeboxItemMove(ch, c_pData);
			break;

		case HEADER_CG_MALL_CHECKOUT:
			SafeboxCheckout(ch, c_pData, true);
			break;

		case HEADER_CG_PARTY_INVITE:
			PartyInvite(ch, c_pData);
			break;

		case HEADER_CG_PARTY_REMOVE:
			PartyRemove(ch, c_pData);
			break;

		case HEADER_CG_PARTY_INVITE_ANSWER:
			PartyInviteAnswer(ch, c_pData);
			break;

		case HEADER_CG_PARTY_SET_STATE:
			PartySetState(ch, c_pData);
			break;

		case HEADER_CG_PARTY_USE_SKILL:
			PartyUseSkill(ch, c_pData);
			break;

		case HEADER_CG_PARTY_PARAMETER:
			PartyParameter(ch, c_pData);
			break;

		case HEADER_CG_ANSWER_MAKE_GUILD:
#ifdef ENABLE_NEWGUILDMAKE
			ch->ChatPacket(CHAT_TYPE_INFO, "<%s> AnswerMakeGuild disabled", __FUNCTION__);
#else
			AnswerMakeGuild(ch, c_pData);
#endif
			break;

		case HEADER_CG_GUILD:
			if ((iExtraLen = Guild(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_CG_FISHING:
			Fishing(ch, c_pData);
			break;

		case HEADER_CG_HACK:
			Hack(ch, c_pData);
			break;
			
			
#if defined(ENABLE_FISHING_GAME)
		case HEADER_CG_FISHING_GAME:
			FishingGame(ch, c_pData);
			break;
#endif

#ifdef __NEWPET_SYSTEM__
		case HEADER_CG_PetSetName:
			BraveRequestPetName(ch, c_pData);
			break;
#endif

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		case HEADER_CG_REQUEST_FLAG:
			RequestFlag(ch, c_pData);
			break;	
#endif
		case HEADER_CG_REFINE:
			Refine(ch, c_pData);
			break;
#ifdef ENABLE_CSHIELD
		case HEADER_CG_CSHIELD:
			CShieldReceive(ch, c_pData);
			break;
#endif
#ifdef __SASH_SYSTEM__
		case HEADER_CG_SASH:
			{
				Sash(ch, c_pData);
			}
			break;
#endif
#if defined(WJ_COMBAT_ZONE)
		case HEADER_CG_COMBAT_ZONE_REQUEST_ACTION:
			CCombatZoneManager::instance().RequestAction(ch, c_pData);
			break;
#endif


		case HEADER_CG_CLIENT_VERSION:
			Version(ch, c_pData);
			break;
			
#ifdef ENABLE_SWITCHBOT
		case HEADER_CG_SWITCHBOT:
			if ((iExtraLen = Switchbot(ch, c_pData, m_iBufferLeft)) < 0)
			{
				return -1;
			}
			break;
#endif
			
		case HEADER_CG_DRAGON_SOUL_REFINE:
			{
				TPacketCGDragonSoulRefine* p = reinterpret_cast <TPacketCGDragonSoulRefine*>((void*)c_pData);
				switch(p->bSubType)
				{
				case DS_SUB_HEADER_CLOSE:
					ch->DragonSoul_RefineWindow_Close();
					break;
				case DS_SUB_HEADER_DO_REFINE_GRADE:
					{
						DSManager::instance().DoRefineGrade(ch, p->ItemGrid);
					}
					break;
				case DS_SUB_HEADER_DO_REFINE_STEP:
					{
						DSManager::instance().DoRefineStep(ch, p->ItemGrid);
					}
					break;
				case DS_SUB_HEADER_DO_REFINE_STRENGTH:
					{
						DSManager::instance().DoRefineStrength(ch, p->ItemGrid);
					}
					break;
				}
			}
			break;

#ifdef ENABLE_NINJEYE
        case HEADER_CG_NINJEYE_RESPONSE:
        {
            TPacketCGNinjEyeResponse* p = static_cast<TPacketCGNinjEyeResponse*>((void*)c_pData);
            ch->OnNinjEyeResponse(p);
            break;
        }
#endif

#ifdef ENABLE_IKASHOP_RENEWAL
		case HEADER_CG_NEW_OFFLINESHOP:
			{
#ifdef __BL_BATTLE_ROYALE__
			if (ch->IsBattleRoyale())
				return 0;
#endif
			if((iExtraLen = IkarusShopPacket(c_pData, ch, m_iBufferLeft))< 0)
				return -1;
			}
			break;
#endif

#ifdef ENABLE_CALENDAR_SYSTEM
		case HEADER_CG_CALENDAR_SYSTEM:
			CalendarSystemSend(ch, c_pData);
			break;
#endif

#ifdef ENABLE_SWITCHBOT_WORLDARD
		case HEADER_CG_SWITCHBOT:
			{
				SwitchBotSend(ch, c_pData);
				break;
			}
			break;
#endif

#ifdef ENABLE_DUNGEON_INFO
		case HEADER_CG_DUNGEON_INFO_SYSTEM:
			DungeonInfoSend(ch, c_pData);
			break;
#endif

#ifdef ENABLE_DROP_ITEM_WORLDARD
		case HEADER_CG_DROP_ITEM_SYSTEM:
			DropItemInfoSend(ch, c_pData);
			break;
#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
		case HEADER_CG_CUBE_RENEWAL:
			CubeRenewalSend(ch, c_pData);
			break;
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
		case HEADER_CG_MINI_GAME_CATCH_KING:
			if ((iExtraLen = CMiniGameManager::Instance().MiniGameCatchKing(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#endif

#ifdef USE_CAPTCHA_SYSTEM
		case HEADER_CG_CAPTCHA:
		{
			Captcha(ch, c_pData);
			break;
		}
#endif
#ifdef USE_CURRENCY_EXCHANGE
		case HEADER_CG_CURRENCY_EXCHANGE:
		{
			CurrencyExchange(ch, c_pData);
			break;
		}
#endif
#if defined(__BL_BATTLE_ROYALE__)
		case HEADER_CG_BATTLE_ROYALE:
			BattleRoyale(ch, c_pData);
			break;
#endif
#ifdef USE_CRYSTAL_SYSTEM
		case HEADER_CG_SHARD_CRAFT:
		{
			if ((iExtraLen = RecvShardCraft(ch, c_pData, m_iBufferLeft)) < 0)
			{
				return -1;
			}

			break;
		}
#endif
		default:
		{
			break;
		}
	}

	return (iExtraLen);
}

int CInputDead::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)
{
	LPCHARACTER ch;

	if (!(ch = d->GetCharacter()))
	{
		sys_err("no character on desc");
#ifdef ENABLE_ANTI_PACKET_FLOOD
		return -1;
#else
		return 0;
#endif
	}

	int iExtraLen = 0;

	switch (bHeader)
	{
		case HEADER_CG_PONG:
			Pong(d);
			break;

		case HEADER_CG_TIME_SYNC:
			Handshake(d, c_pData);
			break;

		case HEADER_CG_CHAT:
			if ((iExtraLen = Chat(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;

			break;

		case HEADER_CG_WHISPER:
			if ((iExtraLen = Whisper(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;

			break;

		case HEADER_CG_HACK:
			Hack(ch, c_pData);
			break;

		default:
#ifdef ENABLE_ANTI_PACKET_FLOOD
		return -1;
#else
		return (0);
#endif
	}

	return (iExtraLen);
}

#ifdef ENABLE_SWITCHBOT
int CInputMain::Switchbot(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	const TPacketCGSwitchbot* p = reinterpret_cast<const TPacketCGSwitchbot*>(data);

	if (uiBytes < sizeof(TPacketCGSwitchbot))
	{
		return -1;
	}

	const char* c_pData = data + sizeof(TPacketCGSwitchbot);
	uiBytes -= sizeof(TPacketCGSwitchbot);

	switch (p->subheader)
	{
	case SUBHEADER_CG_SWITCHBOT_START:
	{
		size_t extraLen = sizeof(TSwitchbotAttributeAlternativeTable) * SWITCHBOT_ALTERNATIVE_COUNT;
		if (uiBytes < extraLen)
		{
			return -1;
		}

		std::vector<TSwitchbotAttributeAlternativeTable> vec_alternatives;

		for (BYTE alternative = 0; alternative < SWITCHBOT_ALTERNATIVE_COUNT; ++alternative)
		{
			const TSwitchbotAttributeAlternativeTable* pAttr = reinterpret_cast<const TSwitchbotAttributeAlternativeTable*>(c_pData);
			c_pData += sizeof(TSwitchbotAttributeAlternativeTable);

			vec_alternatives.emplace_back(*pAttr);
		}

		CSwitchbotManager::Instance().Start(ch->GetPlayerID(), p->slot, vec_alternatives);
		return extraLen;
	}

	case SUBHEADER_CG_SWITCHBOT_STOP:
	{
		CSwitchbotManager::Instance().Stop(ch->GetPlayerID(), p->slot);
		return 0;
	}
	}

	return 0;
}
#endif

#if defined(__BL_BATTLE_ROYALE__)
void CInputMain::BattleRoyale(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGBattleRoyale* p = (TPacketCGBattleRoyale*)c_pData;

	enum
	{
		BATTLE_ROYALE_CG_APPLICATION,
		BATTLE_ROYALE_CG_APPLICATION_CANCEL,
		BATTLE_ROYALE_CG_EXIT,
		BATTLE_ROYALE_CG_RANKING,
		BATTLE_ROYALE_CG_START,
		BATTLE_ROYALE_CG_CLAIM_REWARD,
	};

	switch (p->subheader)
	{
	case BATTLE_ROYALE_CG_APPLICATION:
		BattleRoyale::Instance().SendActionRequest(ch, BattleRoyale::BR_SUB_REGISTER);
		break;
	case BATTLE_ROYALE_CG_APPLICATION_CANCEL:
		BattleRoyale::Instance().SendActionRequest(ch, BattleRoyale::BR_SUB_CANCEL_REGISTRATION);
		break;
	case BATTLE_ROYALE_CG_EXIT:
		BattleRoyale::Instance().BattleCore_Leave(ch);
		break;
	case BATTLE_ROYALE_CG_RANKING:
		break;
	case BATTLE_ROYALE_CG_START:
		BattleRoyale::Instance().SendActionRequest(ch, BattleRoyale::BR_SUB_ENTER);
		break;
	case BATTLE_ROYALE_CG_CLAIM_REWARD:
		BattleRoyale::Instance().ClaimRewards(ch);
		break;
	default:
		sys_err("CInputMain::BattleRoyale : Unknown subheader %d : %s", p->subheader, ch->GetName());
		break;
	}
}
#endif
