#include "stdafx.h"
#include "constants.h"
#include "char.h"
#include "desc.h"
#include "desc_manager.h"
#include "packet.h"
#include "item.h"

/////////////////////////////////////////////////////////////////////////////
// QUICKSLOT HANDLING
/////////////////////////////////////////////////////////////////////////////
void CHARACTER::SyncQuickslot(BYTE bType, WORD wOldPos, WORD wNewPos) // wNewPos == WORD_MAX ¸é DELETE
{
	if (!m_playerData)
	{
		return;
	}

	if (wOldPos == wNewPos)
	{
		return;
	}

	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (m_playerData->pQuickSlot[i].type == bType && m_playerData->pQuickSlot[i].pos == wOldPos)
		{
			if (wNewPos == WORD_MAX)
			{
				DelQuickslot(i);
			}
			else
			{
				TQuickslot slot;
				slot.type = bType;
				slot.pos = wNewPos;

				SetQuickslot(i, slot);
			}
		}
	}
}

bool CHARACTER::GetQuickslot(BYTE pos, TQuickslot ** ppSlot)
{
	if (!m_playerData)
	{
		return false;
	}

	if (pos >= QUICKSLOT_MAX_NUM)
	{
		return false;
	}

	*ppSlot = &m_playerData->pQuickSlot[pos];
	return true;
}

bool CHARACTER::SetQuickslot(BYTE pos, TQuickslot & rSlot)
{
	if (!m_playerData)
	{
		return false;
	}

	if (pos >= QUICKSLOT_MAX_NUM)
	{
		return false;
	}

	if (rSlot.type >= QUICKSLOT_TYPE_MAX_NUM)
	{
		return false;
	}

	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (rSlot.type == 0)
		{
			continue;
		}
		else if (m_playerData->pQuickSlot[i].type == rSlot.type && m_playerData->pQuickSlot[i].pos == rSlot.pos)
		{
			DelQuickslot(i);
		}
	}

	switch (rSlot.type)
	{
		case QUICKSLOT_TYPE_ITEM:
		{
			TItemPos srcCell(INVENTORY, rSlot.pos);
			if (!srcCell.IsDefaultInventoryPosition() && !srcCell.IsBeltInventoryPosition())
			{
				return false;
			}

			break;
		}
		case QUICKSLOT_TYPE_SKILL:
		{
			if ((int) rSlot.pos >= SKILL_MAX_NUM)
			{
				return false;
			}

			break;
		}
		case QUICKSLOT_TYPE_COMMAND:
		{
			break;
		}
		default:
		{
			return false;
		}
	}

	m_playerData->pQuickSlot[pos] = rSlot;

	LPDESC d = GetDesc();

	if (d)
	{
		struct packet_quickslot_add p;
		p.header = HEADER_GC_QUICKSLOT_ADD;
		p.pos = pos;
		p.slot = m_playerData->pQuickSlot[pos];

		d->Packet(&p, sizeof(p));
	}

	return true;
}

bool CHARACTER::DelQuickslot(BYTE pos)
{
	if (!m_playerData)
	{
		return false;
	}

	if (pos >= QUICKSLOT_MAX_NUM)
	{
		return false;
	}

	memset(&m_playerData->pQuickSlot[pos], 0, sizeof(TQuickslot));

	LPDESC d = GetDesc();
	if (d)
	{
		struct packet_quickslot_del p;
		p.header = HEADER_GC_QUICKSLOT_DEL;
		p.pos = pos;

		d->Packet(&p, sizeof(p));
	}

	return true;
}

bool CHARACTER::SwapQuickslot(BYTE a, BYTE b)
{
	if (!m_playerData)
	{
		return false;
	}

	if (a >= QUICKSLOT_MAX_NUM || b >= QUICKSLOT_MAX_NUM)
	{
		return false;
	}

	TQuickslot quickslot = m_playerData->pQuickSlot[a];

	m_playerData->pQuickSlot[a] = m_playerData->pQuickSlot[b];
	m_playerData->pQuickSlot[b] = quickslot;

	LPDESC d = GetDesc();
	if (d)
	{
		struct packet_quickslot_swap p;
		p.header = HEADER_GC_QUICKSLOT_SWAP;
		p.pos = a;
		p.pos_to = b;

		GetDesc()->Packet(&p, sizeof(p));
	}

	return true;
}

void CHARACTER::ChainQuickslotItem(LPITEM pItem, BYTE bType, WORD wOldPos)
{
	if (!m_playerData)
	{
		return;
	}

	if (!pItem || pItem->IsDragonSoul())
	{
		return;
	}

	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (m_playerData->pQuickSlot[i].type == bType && m_playerData->pQuickSlot[i].pos == wOldPos)
		{
			TQuickslot slot;
			slot.type = bType;
			slot.pos = pItem->GetCell();

			SetQuickslot(i, slot);

			break;
		}
	}
}
