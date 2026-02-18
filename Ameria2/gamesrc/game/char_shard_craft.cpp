#include "stdafx.h"

#ifdef USE_CRYSTAL_SYSTEM
#include "char.h"
#include "buffer_manager.h"
#include "desc.h"
#include "item.h"
#include "item_manager.h"
#include "db.h"
#include "questmanager.h"

void CHARACTER::ClearShardCraft(int32_t iTargetSlot/* = -1*/)
{
	if (m_playerData)
	{
		auto& itemMgr = ITEM_MANAGER::Instance();
		LPITEM item = nullptr;

		if (iTargetSlot == -1)
		{
			for (int i = 0; i < SHARD_CRAFT_SLOT_MAX; i++)
			{
				if (m_playerData->m_pShardCraft[i].dwID != 0)
				{
					item = itemMgr.Find(m_playerData->m_pShardCraft[i].dwID);
					if (item)
					{
						item->Lock(false);
					}

					m_playerData->m_pShardCraft[i].dwID = 0;
					m_playerData->m_pShardCraft[i].wCell = 0;
					m_playerData->m_pShardCraft[i].bWindow = RESERVED_WINDOW;
				}
			}

			return;
		}

		if (iTargetSlot < SHARD_CRAFT_SLOT_MAX)
		{
			if (m_playerData->m_pShardCraft[iTargetSlot].dwID != 0)
			{
				item = itemMgr.Find(m_playerData->m_pShardCraft[iTargetSlot].dwID);
				if (item)
				{
					item->Lock(false);
				}

				m_playerData->m_pShardCraft[iTargetSlot].dwID = 0;
				m_playerData->m_pShardCraft[iTargetSlot].wCell = 0;
				m_playerData->m_pShardCraft[iTargetSlot].bWindow = RESERVED_WINDOW;
			}
		}
	}
}

void CHARACTER::SendOutShardCraft(int32_t iTargetSlot/* = -1*/)
{
	ClearShardCraft(iTargetSlot);

	LPDESC d = GetDesc();
	if (d)
	{
		if (iTargetSlot == -1)
		{
			TPacketGCShardCraft p;
			p.bHeader = HEADER_GC_SHARD_CRAFT;
			p.bSubHeader = SUBHEADER_SHARD_CRAFT_OUT_ALL;
			p.wSize = sizeof(p);

			d->Packet(&p, sizeof(p));
		}
		else
		{
			TPacketGCShardCraft p;
			p.bHeader = HEADER_GC_SHARD_CRAFT;
			p.bSubHeader = SUBHEADER_SHARD_CRAFT_OUT;

			TShardCraftOut p2;
			p2.iTargetSlot = iTargetSlot;

			p.wSize = sizeof(p) + sizeof(p2);

			TEMP_BUFFER buffer;
			buffer.write(&p, sizeof(p));
			buffer.write(&p2, sizeof(p2));

			d->Packet(buffer.read_peek(), buffer.size());
		}
	}
}

void CHARACTER::SendShardCraft(const uint8_t bSubHeader,
					int32_t iSourceSlot/* = -1*/,
					int32_t iTargetSlot/* = -1*/)
{
	if (!m_playerData)
	{
		return;
	}

	LPDESC d = GetDesc();
	if (!d)
	{
		return;
	}

	switch (bSubHeader)
	{
		case SUBHEADER_SHARD_CRAFT_OPEN:
		case SUBHEADER_SHARD_CRAFT_CLOSE:
		{
			if (bSubHeader == SUBHEADER_SHARD_CRAFT_CLOSE && !IsShardCraftOpened())
			{
				break;
			}

			m_playerData->m_bShardCraftWindowStat = (bSubHeader == SUBHEADER_SHARD_CRAFT_OPEN) ? true : false;

			ClearShardCraft();

			TPacketGCShardCraft p;
			p.bHeader = HEADER_GC_SHARD_CRAFT;
			p.bSubHeader = bSubHeader;
			p.wSize = sizeof(p);

			d->Packet(&p, sizeof(p));

			break;
		}
		case SUBHEADER_SHARD_CRAFT_ADD:
		{
			if (!IsShardCraftOpened())
			{
				break;
			}

			if (IsDead())
			{
				ChatPacket(CHAT_TYPE_INFO, "1534");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this, because you're in dead."));
				break;
			}

			if (IsObserverMode())
			{
				ChatPacket(CHAT_TYPE_INFO, "1535");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this, because you're in observer mode."));
				break;
			}

			if (!CanHandleItem(false, false, false, true))
			{
				ChatPacket(CHAT_TYPE_INFO, "1536");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("If you have another system window opened please close it before."));
				break;
			}

			auto* pPC = quest::CQuestManager::instance().GetPCForce(GetPlayerID());
			if (!pPC
				 || pPC->IsRunning())
			{
				ChatPacket(CHAT_TYPE_INFO, "1537");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this because you have a quest instance opened."));
				break;
			}

			if (IsHack(false, true, 10, true))
			{
				ChatPacket(CHAT_TYPE_INFO, "1538");
				ChatPacket(CHAT_TYPE_INFO, "1539");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Please try again in 10 seconds if you recently interracted with"));
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("a player or another game system."));
				break;
			}

			if (iSourceSlot < 0 || iSourceSlot >= INVENTORY_MAX_NUM)
			{
				break;
			}

			LPITEM item = GetInventoryItem(iSourceSlot);
			if (!item
				 || item->GetType() != ITEM_USE
				 || item->GetSubType() != USE_ADD_ATTR_CRYSTAL
				 || item->GetValue(CRYSTAL_ADD_BONUS_SLOT_VALUE_SLOT) < 1
				 || item->GetValue(CRYSTAL_ADD_BONUS_SLOT_VALUE_SLOT) >= ITEM_ATTRIBUTE_NORM_NUM
				 || item->GetValue(CRYSTAL_ADD_BONUS_UPGRADE_CHANCE) < 1
				 || item->GetValue(CRYSTAL_ADD_BONUS_UPGRADE_CHANCE) > 100
				 || item->GetValue(CRYSTAL_ADD_BONUS_UPGRADE_RESULT_VNUM) < 1
				 || item->IsExchanging()
				 || item->isLocked())
			{
				break;
			}

			switch (iTargetSlot)
			{
				case SHARD_CRAFT_SLOT_MAIN:
				{
					if (m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].dwID != 0
						 || m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MATERIAL].dwID != 0)
					{
						break;
					}

					item->Lock(true);
					m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].dwID = item->GetID();
					m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].wCell = item->GetCell();
					m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].bWindow = item->GetWindow();

					TPacketGCShardCraft p;
					p.bHeader = HEADER_GC_SHARD_CRAFT;
					p.bSubHeader = bSubHeader;

					TShardCraftAdd p2;
					p2.iSourceSlot = iSourceSlot;
					p2.iTargetSlot = SHARD_CRAFT_SLOT_MAIN;

					p.wSize = sizeof(p) + sizeof(p2);

					TEMP_BUFFER buffer;
					buffer.write(&p, sizeof(p));
					buffer.write(&p2, sizeof(p2));

					d->Packet(buffer.read_peek(), buffer.size());
					break;
				}
				case SHARD_CRAFT_SLOT_MATERIAL:
				{
					if (m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].dwID == 0
						 || m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MATERIAL].dwID != 0)
					{
						break;
					}

					auto& itemMgr = ITEM_MANAGER::Instance();
					LPITEM item2 = itemMgr.Find(m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].dwID);
					if (!item2
						 || item->GetValue(CRYSTAL_ADD_BONUS_SLOT_VALUE_SLOT) != item2->GetValue(CRYSTAL_ADD_BONUS_SLOT_VALUE_SLOT)
						 || item->GetValue(CRYSTAL_ADD_BONUS_TYPE_VALUE_SLOT) != item2->GetValue(CRYSTAL_ADD_BONUS_TYPE_VALUE_SLOT))
					{
						break;
					}

					item->Lock(true);
					m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MATERIAL].dwID = item->GetID();
					m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MATERIAL].wCell = item->GetCell();
					m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MATERIAL].bWindow = item->GetWindow();

					TPacketGCShardCraft p;
					p.bHeader = HEADER_GC_SHARD_CRAFT;
					p.bSubHeader = bSubHeader;

					TShardCraftAdd p2;
					p2.iSourceSlot = iSourceSlot;
					p2.iTargetSlot = SHARD_CRAFT_SLOT_MATERIAL;

					p.wSize = sizeof(p) + sizeof(p2);

					TEMP_BUFFER buffer;
					buffer.write(&p, sizeof(p));
					buffer.write(&p2, sizeof(p2));

					d->Packet(buffer.read_peek(), buffer.size());
					break;
				}
				default: // -1
				{
					break;
				}
			}

			break;
		}
		case SUBHEADER_SHARD_CRAFT_OUT:
		{
			if (!IsShardCraftOpened())
			{
				break;
			}

			if (m_playerData->m_pShardCraft[iTargetSlot].dwID != 0)
			{
				if (IsDead())
				{
					ChatPacket(CHAT_TYPE_INFO, "1534");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this, because you're in dead."));
					break;
				}

				if (IsObserverMode())
				{
					ChatPacket(CHAT_TYPE_INFO, "1535");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this, because you're in observer mode."));
					break;
				}

				if (!CanHandleItem(false, false, false, true))
				{
					ChatPacket(CHAT_TYPE_INFO, "1536");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("If you have another system window opened please close it before."));
					break;
				}

				auto* pPC = quest::CQuestManager::instance().GetPCForce(GetPlayerID());
				if (!pPC
					 || pPC->IsRunning())
				{
					ChatPacket(CHAT_TYPE_INFO, "1537");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this because you have a quest instance opened."));
					break;
				}

				if (IsHack(false, true, 10, true))
				{
					ChatPacket(CHAT_TYPE_INFO, "1538");
					ChatPacket(CHAT_TYPE_INFO, "1539");
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Please try again in 10 seconds if you recently interracted with"));
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("a player or another game system."));
					break;
				}

				switch (iTargetSlot)
				{
					case SHARD_CRAFT_SLOT_MAIN:
					{
						SendOutShardCraft();
						break;
					}
					case SHARD_CRAFT_SLOT_MATERIAL:
					{
						SendOutShardCraft(iTargetSlot);
						break;
					}
					default: // -1
					{
						break;
					}
				}
			}

			break;
		}
		case SUBHEADER_SHARD_CRAFT_REFINE:
		{
			if (!IsShardCraftOpened())
			{
				break;
			}

			for (int i = 0; i < SHARD_CRAFT_SLOT_MAX; i++)
			{
				if (m_playerData->m_pShardCraft[i].dwID == 0)
				{
					return;
				}
			}

			if (IsDead())
			{
				ChatPacket(CHAT_TYPE_INFO, "1534");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this, because you're in dead."));
				break;
			}

			if (IsObserverMode())
			{
				ChatPacket(CHAT_TYPE_INFO, "1535");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this, because you're in observer mode."));
				break;
			}

			if (!CanHandleItem(false, false, false, true))
			{
				ChatPacket(CHAT_TYPE_INFO, "1536");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("If you have another system window opened please close it before."));
				break;
			}

			DWORD dwOwnerPID = GetPlayerID();

			auto* pPC = quest::CQuestManager::instance().GetPCForce(dwOwnerPID);
			if (!pPC
				 || pPC->IsRunning())
			{
				ChatPacket(CHAT_TYPE_INFO, "1537");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot do this because you have a quest instance opened."));
				break;
			}

			if (IsHack(false, true, 10, true))
			{
				ChatPacket(CHAT_TYPE_INFO, "1538");
				ChatPacket(CHAT_TYPE_INFO, "1539");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Please try again in 10 seconds if you recently interracted with"));
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("a player or another game system."));
				break;
			}

			auto& itemMgr = ITEM_MANAGER::Instance();

			LPITEM item1 = itemMgr.Find(m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].dwID);
			LPITEM item2 = itemMgr.Find(m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MATERIAL].dwID);

			if (!item1 || !item2)
			{
				SendOutShardCraft();
				break;
			}

			DWORD dwItemOwnerPID = (item1 && item1->GetOwner()) ? item1->GetOwner()->GetPlayerID() : 0;
			if (dwItemOwnerPID == 0 || dwItemOwnerPID != dwOwnerPID)
			{
				SendOutShardCraft();

				ChatPacket(CHAT_TYPE_INFO, "1540 %d", 1);
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something went wrong. Please contact the staff (err code: %d)!"), 1);
				break;
			}

			dwItemOwnerPID = (item2 && item2->GetOwner()) ? item2->GetOwner()->GetPlayerID() : 0;
			if (dwItemOwnerPID == 0 || dwItemOwnerPID != dwOwnerPID)
			{
				SendOutShardCraft();

				ChatPacket(CHAT_TYPE_INFO, "1540 %d", 2);
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something went wrong. Please contact the staff (err code: %d)!"), 2);
				break;
			}

			if (item1->GetWindow() != m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].bWindow
				|| item1->GetCell() != m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MAIN].wCell)
			{
				SendOutShardCraft();

				ChatPacket(CHAT_TYPE_INFO, "1540 %d", 3);
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something went wrong. Please contact the staff (err code: %d)!"), 3);
				break;
			}

			if (item2->GetWindow() != m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MATERIAL].bWindow
				|| item2->GetCell() != m_playerData->m_pShardCraft[SHARD_CRAFT_SLOT_MATERIAL].wCell)
			{
				SendOutShardCraft();

				ChatPacket(CHAT_TYPE_INFO, "1540 %d", 4);
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something went wrong. Please contact the staff (err code: %d)!"), 4);
				break;
			}

			const long long lldCraftPrice = static_cast<long long>(item1->GetValue(CRYSTAL_ADD_BONUS_UPGRADE_PRICE));
			if (GetGold() < lldCraftPrice)
			{
				ChatPacket(CHAT_TYPE_INFO, "1541");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough Yang."));
				break;
			}

			const DWORD dwItemVnum = item1->GetValue(CRYSTAL_ADD_BONUS_UPGRADE_RESULT_VNUM);

			const auto* pTable = itemMgr.GetTable(dwItemVnum);
			if (!pTable)
			{
				SendOutShardCraft();

				ChatPacket(CHAT_TYPE_INFO, "1542 %s", item1->GetName());
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Something is wrong with %s. Please contact the staff!"), item1->GetName());
				break;
			}

			if (GetEmptyInventory(pTable->bSize) == -1)
			{
				ChatPacket(CHAT_TYPE_INFO, "1543 %u", pTable->bSize);
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You need at least %d empty perpendicular slots in you're inventory."), pTable->bSize);
				break;
			}

			PointChange(POINT_GOLD, -lldCraftPrice);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, dwItemVnum, -lldCraftPrice);

			if (number(1, 100) <= item1->GetValue(CRYSTAL_ADD_BONUS_UPGRADE_CHANCE))
			{
				SendOutShardCraft();

				item1->SetCount(item1->GetCount() - 1);
				item2->SetCount(item2->GetCount() - 1);

				AutoGiveItem(dwItemVnum);

				ChatPacket(CHAT_TYPE_INFO, "1544");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The craft had success."));
			}
			else
			{
				SendOutShardCraft(SHARD_CRAFT_SLOT_MATERIAL);

				item2->SetCount(item2->GetCount() - 1);

				ChatPacket(CHAT_TYPE_INFO, "1545");
				// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The craft failed."));
			}

			break;
		}
		default:
		{
			sys_err("SendShardCraft >> unknown subHeader (%u).", bSubHeader);
			break;
		}
	}
}
#endif // USE_CRYSTAL_SYSTEM
