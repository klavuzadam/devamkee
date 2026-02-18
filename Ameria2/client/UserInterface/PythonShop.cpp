#include "stdafx.h"
#include "PythonShop.h"

#include "PythonNetworkStream.h"
//#include "PythonStackCheck.h"

void CPythonShop::SetTabCoinType(BYTE tabIdx, BYTE coinType)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return;
	}
	m_aShoptabs[tabIdx].coinType = coinType;
}

BYTE CPythonShop::GetTabCoinType(BYTE tabIdx)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return 0xff;
	}
	return m_aShoptabs[tabIdx].coinType;
}

void CPythonShop::SetTabName(BYTE tabIdx, const char* name)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return;
	}
	m_aShoptabs[tabIdx].name = name;
}

const char* CPythonShop::GetTabName(BYTE tabIdx)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return NULL;
	}

	return m_aShoptabs[tabIdx].name.c_str();
}

void CPythonShop::SetItemData(DWORD dwIndex, const TShopItemData & c_rShopItemData)
{
	BYTE tabIdx = dwIndex / SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % SHOP_HOST_ITEM_MAX_NUM;

	SetItemData(tabIdx, dwSlotPos, c_rShopItemData);
}

BOOL CPythonShop::GetItemData(DWORD dwIndex, const TShopItemData ** c_ppItemData)
{
	BYTE tabIdx = dwIndex / SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % SHOP_HOST_ITEM_MAX_NUM;

	return GetItemData(tabIdx, dwSlotPos, c_ppItemData);
}

void CPythonShop::SetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData & c_rShopItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, SHOP_HOST_ITEM_MAX_NUM);
		return;
	}

	m_aShoptabs[tabIdx].items[dwSlotPos] = c_rShopItemData;
}

BOOL CPythonShop::GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData ** c_ppItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, SHOP_HOST_ITEM_MAX_NUM);
		return FALSE;
	}

	*c_ppItemData = &m_aShoptabs[tabIdx].items[dwSlotPos];

	return TRUE;
}

void CPythonShop::Open()
{
	m_isShoping = TRUE;
}

void CPythonShop::Close()
{
	m_isShoping = FALSE;
}

BOOL CPythonShop::IsOpen()
{
	return m_isShoping;
}

void CPythonShop::Clear()
{
	m_isShoping = FALSE;

	m_bTabCount = 1;

	for (int i = 0; i < SHOP_TAB_COUNT_MAX; i++)
		memset (m_aShoptabs[i].items, 0, sizeof(TShopItemData) * SHOP_HOST_ITEM_MAX_NUM);
}

CPythonShop::CPythonShop(void)
{
	Clear();
}

CPythonShop::~CPythonShop(void)
{
}

PyObject * shopOpen(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop& rkShop=CPythonShop::Instance();
	rkShop.Open();
	return Py_BuildNone();
}

PyObject * shopClose(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop& rkShop=CPythonShop::Instance();
	rkShop.Close();
	return Py_BuildNone();
}

PyObject * shopClear(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop& rkShop=CPythonShop::Instance();
	rkShop.Clear();
	return Py_BuildNone();
}

PyObject * shopIsOpen(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop& rkShop=CPythonShop::Instance();
	return Py_BuildValue("i", rkShop.IsOpen());
}

PyObject * shopGetItemID(PyObject * poSelf, PyObject * poArgs)
{
	//CPythonStackController::Instance().CheckStackReference(CHEAT_TYPE_shop_GetItemID, PY_REF_FILE, PY_REF_FUNC);

	int nPos;
	if (!PyTuple_GetInteger(poArgs, 0, &nPos))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(nPos, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->vnum);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetItemCount(PyObject * poSelf, PyObject * poArgs)
{

	//CPythonStackController::Instance().CheckStackReference(CHEAT_TYPE_shop_GetItemCount, PY_REF_FILE, PY_REF_FUNC);

	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->count);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetItemPrice(PyObject * poSelf, PyObject * poArgs)
{

	//CPythonStackController::Instance().CheckStackReference(CHEAT_TYPE_shop_GetItemPrice, PY_REF_FILE, PY_REF_FUNC);

	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
#ifdef ENABLE_FULL_YANG
		return Py_BuildValue("L", c_pItemData->price);
#else
		return Py_BuildValue("i", c_pItemData->price);
#endif

	return Py_BuildValue("i", 0);
}
PyObject * shopGetItemMetinSocket(PyObject * poSelf, PyObject * poArgs)
{

	//CPythonStackController::Instance().CheckStackReference(CHEAT_TYPE_shop_GetItemMetinSocket, PY_REF_FILE, PY_REF_FUNC);

	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iMetinSocketIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->alSockets[iMetinSocketIndex]);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetItemAttribute(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		const TShopItemData * c_pItemData;
		if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
			return Py_BuildValue("ii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue);
	}

	return Py_BuildValue("ii", 0, 0);
}

#ifdef ENABLE_YOHARA_SYSTEM
PyObject* shopGetItemApplyRandom(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < APPLY_RANDOM_SLOT_MAX_NUM)
	{
		const TShopItemData* c_pItemData;
		if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
			return Py_BuildValue("ii", c_pItemData->aApplyRandom[iAttrSlotIndex].bType, c_pItemData->aApplyRandom[iAttrSlotIndex].sValue);
	}

	return Py_BuildValue("ii", 0, 0);
}
#endif

PyObject * shopGetTabCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonShop::instance().GetTabCount());
}

PyObject * shopGetTabName(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bTabIdx;
	if (!PyTuple_GetInteger(poArgs, 0, &bTabIdx))
		return Py_BuildException();

	return Py_BuildValue("s", CPythonShop::instance().GetTabName(bTabIdx));
}

PyObject * shopGetTabCoinType(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bTabIdx;
	if (!PyTuple_GetInteger(poArgs, 0, &bTabIdx))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonShop::instance().GetTabCoinType(bTabIdx));
}

#ifdef ENABLE_BUY_WITH_ITEM
PyObject* shopGetBuyWithItem(PyObject* poSelf, PyObject* poArgs)
{
	int idx;
	if (!PyTuple_GetInteger(poArgs, 0, &idx))
		return Py_BuildException();

	int type;
	if (!PyTuple_GetInteger(poArgs, 1, &type))
		return Py_BuildException();

	if (type >= MAX_SHOP_PRICES)
		return Py_BuildValue("ii", 0, 0);

	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(idx, &c_pItemData))
		return Py_BuildValue("ii", c_pItemData->itemprice[type].vnum, c_pItemData->itemprice[type].count);

	return Py_BuildValue("ii", 0, 0);
}
#endif

void initshop()
{
	static PyMethodDef s_methods[] =
	{
		// Shop
		{ "Open",						shopOpen,						METH_VARARGS },
		{ "Close",						shopClose,						METH_VARARGS },
		{ "Clear",						shopClear,						METH_VARARGS },
		{ "IsOpen",						shopIsOpen,						METH_VARARGS },

		{ "GetItemID",					shopGetItemID,					METH_VARARGS },
		{ "GetItemCount",				shopGetItemCount,				METH_VARARGS },
		{ "GetItemPrice",				shopGetItemPrice,				METH_VARARGS },
		{ "GetItemMetinSocket",			shopGetItemMetinSocket,			METH_VARARGS },
		{ "GetItemAttribute",			shopGetItemAttribute,			METH_VARARGS },
		{ "GetTabCount",				shopGetTabCount,				METH_VARARGS },
		{ "GetTabName",					shopGetTabName,					METH_VARARGS },
		{ "GetTabCoinType",				shopGetTabCoinType,				METH_VARARGS },

#ifdef ENABLE_YOHARA_SYSTEM
		{ "GetItemApplyRandom", shopGetItemApplyRandom, METH_VARARGS },
#endif
#ifdef ENABLE_BUY_WITH_ITEM
		{"GetBuyWithItem",				shopGetBuyWithItem,				METH_VARARGS },
#endif

		{ NULL,							NULL,							NULL },
	};

	PyObject * poModule = Py_InitModule("shop", s_methods);

	PyModule_AddIntConstant(poModule, "SHOP_SLOT_COUNT", SHOP_HOST_ITEM_MAX_NUM);
	PyModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_GOLD", SHOP_COIN_TYPE_GOLD);
	PyModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_SECONDARY_COIN", SHOP_COIN_TYPE_SECONDARY_COIN);
#if defined(WJ_COMBAT_ZONE)
	PyModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_COMBAT_ZONE", SHOP_COIN_TYPE_COMBAT_ZONE);
#endif
#ifdef ENABLE_BUY_WITH_ITEM
	PyModule_AddIntConstant(poModule, "MAX_SHOP_PRICES", MAX_SHOP_PRICES);
#endif
}
