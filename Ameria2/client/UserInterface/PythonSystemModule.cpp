#include "StdAfx.h"
#include "PythonSystem.h"

PyObject * systemGetWidth(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetWidth());
}

PyObject * systemGetHeight(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetHeight());
}

PyObject * systemSetInterfaceHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poHandler;
	if (!PyTuple_GetObject(poArgs, 0, &poHandler))
		return Py_BuildException();

	CPythonSystem::Instance().SetInterfaceHandler(poHandler);
	return Py_BuildNone();
}

PyObject * systemDestroyInterfaceHandler(PyObject* poSelf, PyObject* poArgs)
{
	CPythonSystem::Instance().DestroyInterfaceHandler();
	return Py_BuildNone();
}

PyObject * systemReserveResource(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildNone();
}

PyObject * systemisInterfaceConfig(PyObject* poSelf, PyObject* poArgs)
{
	int isInterfaceConfig = CPythonSystem::Instance().isInterfaceConfig();
	return Py_BuildValue("i", isInterfaceConfig);
}

PyObject * systemSaveWindowStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	int iVisible;
	if (!PyTuple_GetInteger(poArgs, 1, &iVisible))
		return Py_BuildException();

	int iMinimized;
	if (!PyTuple_GetInteger(poArgs, 2, &iMinimized))
		return Py_BuildException();

	int ix;
	if (!PyTuple_GetInteger(poArgs, 3, &ix))
		return Py_BuildException();

	int iy;
	if (!PyTuple_GetInteger(poArgs, 4, &iy))
		return Py_BuildException();

	int iHeight;
	if (!PyTuple_GetInteger(poArgs, 5, &iHeight))
		return Py_BuildException();

	CPythonSystem::Instance().SaveWindowStatus(iIndex, iVisible, iMinimized, ix, iy, iHeight);
	return Py_BuildNone();
}

PyObject * systemGetWindowStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonSystem::TWindowStatus & c_rWindowStatus = CPythonSystem::Instance().GetWindowStatusReference(iIndex);
	return Py_BuildValue("iiiii", c_rWindowStatus.isVisible,
								  c_rWindowStatus.isMinimized,
								  c_rWindowStatus.ixPosition,
								  c_rWindowStatus.iyPosition,
								  c_rWindowStatus.iHeight);
}

PyObject * systemGetConfig(PyObject * poSelf, PyObject * poArgs)
{
	CPythonSystem::TConfig *tmp = CPythonSystem::Instance().GetConfig();

	int iRes = CPythonSystem::Instance().GetResolutionIndex(tmp->width, tmp->height, tmp->bpp);
	int iFrequency = CPythonSystem::Instance().GetFrequencyIndex(iRes, tmp->frequency);

	return Py_BuildValue("iiiiiiii",  iRes,
									  iFrequency,
									  tmp->is_software_cursor,
									  tmp->is_object_culling,
									  tmp->music_volume,
									  tmp->voice_volume,
									  tmp->gamma,
									  tmp->iDistance);
}

PyObject * systemSetSaveID(PyObject * poSelf, PyObject * poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();

	char * szSaveID;
	if (!PyTuple_GetString(poArgs, 1, &szSaveID))
		return Py_BuildException();

	CPythonSystem::Instance().SetSaveID(iValue, szSaveID);
	return Py_BuildNone();
}

PyObject * systemisSaveID(PyObject * poSelf, PyObject * poArgs)
{
	int value = CPythonSystem::Instance().IsSaveID();
	return Py_BuildValue("i", value);
}

PyObject * systemGetSaveID(PyObject * poSelf, PyObject * poArgs)
{
	const char * c_szSaveID = CPythonSystem::Instance().GetSaveID();
	return Py_BuildValue("s", c_szSaveID);
}

PyObject * systemGetMusicVolume(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetMusicVolume());
}

PyObject * systemGetSoundVolume(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetSoundVolume());
}

PyObject * systemSetMusicVolume(PyObject * poSelf, PyObject * poArgs)
{
	float fVolume;
	if (!PyTuple_GetFloat(poArgs, 0, &fVolume))
		return Py_BuildException();

	CPythonSystem::Instance().SetMusicVolume(fVolume);
	return Py_BuildNone();
}

PyObject * systemSetSoundVolumef(PyObject * poSelf, PyObject * poArgs)
{
	float fVolume;
	if (!PyTuple_GetFloat(poArgs, 0, &fVolume))
		return Py_BuildException();

	CPythonSystem::Instance().SetSoundVolumef(fVolume);
	return Py_BuildNone();
}

PyObject * systemIsSoftwareCursor(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsSoftwareCursor());
}

PyObject * systemSetViewChatFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetViewChatFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemIsViewChat(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsViewChat());
}

PyObject * systemSetAlwaysShowNameFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetAlwaysShowNameFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemSetShowDamageFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowDamageFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemSetShowSalesTextFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowSalesTextFlag(iFlag);

	return Py_BuildNone();
}

#ifdef WJ_SHOW_MOB_INFO
#include "PythonCharacterManager.h"
#include "PythonNonPlayer.h"
#include "PythonSystem.h"
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
PyObject * systemIsShowMobAIFlag(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowMobAIFlag());
}
PyObject * systemSetShowMobAIFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowMobAIFlagFlag(iFlag);

	for (CPythonCharacterManager::CharacterIterator it=CPythonCharacterManager::Instance().CharacterInstanceBegin(), ti=CPythonCharacterManager::Instance().CharacterInstanceEnd();
			it!=ti;
			++it)
	{
		CInstanceBase * pkInst = *it;
		if (pkInst && pkInst->IsEnemy())
			pkInst->MobInfoAiFlagRefresh();
	}
	return Py_BuildNone();
}
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
PyObject * systemIsShowMobLevel(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowMobLevel());
}
PyObject * systemSetShowMobLevel(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowMobLevelFlag(iFlag);

	for (CPythonCharacterManager::CharacterIterator it=CPythonCharacterManager::Instance().CharacterInstanceBegin(), ti=CPythonCharacterManager::Instance().CharacterInstanceEnd();
			it!=ti;
			++it)
	{
		CInstanceBase * pkInst = *it;
		if (pkInst && pkInst->IsEnemy())
			pkInst->MobInfoLevelRefresh();
	}
	return Py_BuildNone();
}
#endif
#endif
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_PRO
PyObject* systemGetShopRangeView(PyObject* poSelf, PyObject* poArgs)
{
	return PyFloat_FromDouble(CPythonSystem::instance().GetShopRangeView());
}

PyObject* systemSetShopRangeView(PyObject* poSelf, PyObject* poArgs)
{
	float view = 0.0f;
	if(!PyTuple_GetFloat(poArgs, 0, &view))
		return Py_BadArgument();

	CPythonSystem::instance().SetShopRangeView(view);
	return Py_BuildNone();
}
#endif
#endif

PyObject * systemIsAlwaysShowName(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsAlwaysShowName());
}

PyObject * systemIsShowDamage(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowDamage());
}

PyObject* systemIsShowOutline(PyObject* poSelf, PyObject* poArgs)
{
	static auto& system = CPythonSystem::Instance();
	return Py_BuildValue("b", system.IsShowOutline());
}

PyObject* systemSetShowOutlineFlag(PyObject* poSelf, PyObject* poArgs)
{
	bool isShow;
	PyTuple_GetBoolean(poArgs, 0, &isShow);

	static auto& system = CPythonSystem::Instance();
	system.SetShowOutlineFlag(isShow);
	return Py_BuildNone();
}

PyObject * systemIsShowSalesText(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowSalesText());
}

PyObject * systemSetConfig(PyObject * poSelf, PyObject * poArgs)
{
	int res_index;
	int width;
	int height;
	int bpp;
	int frequency_index;
	int frequency;
	int software_cursor;
	int shadow;
	int object_culling;
	int music_volume;
	int voice_volume;
	int gamma;
	int distance;

	if (!PyTuple_GetInteger(poArgs, 0, &res_index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &frequency_index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 2, &software_cursor))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 3, &shadow))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 4, &object_culling))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 5, &music_volume))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 6, &voice_volume))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 7, &gamma))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 8, &distance))
		return Py_BuildException();

	if (!CPythonSystem::Instance().GetResolution(res_index, (DWORD *) &width, (DWORD *) &height, (DWORD *) &bpp))
		return Py_BuildNone();

	if (!CPythonSystem::Instance().GetFrequency(res_index,frequency_index, (DWORD *) &frequency))
		return Py_BuildNone();

	CPythonSystem::TConfig tmp;

	memcpy(&tmp, CPythonSystem::Instance().GetConfig(), sizeof(tmp));

	tmp.width				= width;
	tmp.height				= height;
	tmp.bpp					= bpp;
	tmp.frequency			= frequency;
	tmp.is_software_cursor	= software_cursor ? true : false;
	tmp.is_object_culling	= object_culling ? true : false;
	tmp.music_volume		= (char) music_volume;
	tmp.voice_volume		= (char) voice_volume;
	tmp.gamma				= gamma;
	tmp.iDistance			= distance;

	CPythonSystem::Instance().SetConfig(&tmp);
	return Py_BuildNone();
}

PyObject * systemApplyConfig(PyObject * poSelf, PyObject * poArgs)
{
	CPythonSystem::Instance().ApplyConfig();
	return Py_BuildNone();
}

PyObject * systemSaveConfig(PyObject * poSelf, PyObject * poArgs)
{
	int ret = CPythonSystem::Instance().SaveConfig();
	return Py_BuildValue("i", ret);
}

PyObject * systemGetResolutionCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetResolutionCount());
}

PyObject * systemGetFrequencyCount(PyObject * poSelf, PyObject * poArgs)
{
	int	index;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonSystem::Instance().GetFrequencyCount(index));
}

PyObject * systemGetResolution(PyObject * poSelf, PyObject * poArgs)
{
	int	index;
	DWORD width = 0, height = 0, bpp = 0;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	CPythonSystem::Instance().GetResolution(index, &width, &height, &bpp);
	return Py_BuildValue("iii", width, height, bpp);
}

PyObject * systemGetCurrentResolution(PyObject * poSelf, PyObject *poArgs)
{
	CPythonSystem::TConfig *tmp = CPythonSystem::Instance().GetConfig();
	return Py_BuildValue("iii", tmp->width, tmp->height, tmp->bpp);
}

PyObject * systemGetFrequency(PyObject * poSelf, PyObject * poArgs)
{
	int	index, frequency_index;
	DWORD frequency = 0;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &frequency_index))
		return Py_BuildException();

	CPythonSystem::Instance().GetFrequency(index, frequency_index, &frequency);
	return Py_BuildValue("i", frequency);
}

#ifdef ENABLE_NEW_GAMEOPTION
PyObject* systemSetQuestIconMode(PyObject* poSelf, PyObject* poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();
	CPythonSystem::Instance().SetQuestLetter(iValue ? true:false);
	return Py_BuildNone();
}
PyObject* systemGetQuestIconMode(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetQuestLetter());
}
PyObject* systemSetCountryFlag(PyObject* poSelf, PyObject* poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();
	CPythonSystem::Instance().SetCountryFlag(iValue ? true : false);
	return Py_BuildNone();
}
PyObject* systemGetCountryFlag(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetCountryFlag());
}
PyObject* systemSetAffectIcons(PyObject* poSelf, PyObject* poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();
	CPythonSystem::Instance().SetAffectIcons(iValue ? true : false);
	return Py_BuildNone();
}
PyObject* systemGetAffectIcons(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetAffectIcons());
}
#endif

PyObject* systemSetFOVLevel(PyObject* poSelf, PyObject* poArgs)
{
	float fFOV;
	if (!PyTuple_GetFloat(poArgs, 0, &fFOV))
		return Py_BuildException();
	CPythonSystem::Instance().SetFOVLevel(fFOV);
	return Py_BuildNone();
}
PyObject* systemGetFOVLevel(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetFOVLevel());
}

#ifdef ENABLE_GRAPHIC_ON_OFF
PyObject* systemSetEffectLevel(PyObject* poSelf, PyObject* poArgs)
{
	BYTE iIndex;
	if (!PyTuple_GetByte(poArgs, 0, &iIndex))
		return Py_BuildException();
	CPythonSystem::Instance().SetEffectLevel(iIndex);
	return Py_BuildNone();
}
PyObject* systemGetEffectLevel(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetEffectLevel());
}
PyObject* systemSetPetRender(PyObject* poSelf, PyObject* poArgs)
{
	bool bFlag;
	if (!PyTuple_GetBoolean(poArgs, 0, &bFlag))
		return Py_BuildException();
	CPythonSystem::Instance().SetPetRender(bFlag);
	return Py_BuildNone();
}
PyObject* systemGetPetRender(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetPetRender());
}
PyObject* systemSetMountRender(PyObject* poSelf, PyObject* poArgs)
{
	bool bFlag;
	if (!PyTuple_GetBoolean(poArgs, 0, &bFlag))
		return Py_BuildException();
	CPythonSystem::Instance().SetMountRender(bFlag);
	return Py_BuildNone();
}
PyObject* systemGetMountRender(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetMountRender());
}

PyObject* systemSetShopNameStatus(PyObject* poSelf, PyObject* poArgs)
{
	bool bFlag;
	if (!PyTuple_GetBoolean(poArgs, 0, &bFlag))
		return Py_BuildException();
	CPythonSystem::Instance().SetShopNameStatus(bFlag);
	return Py_BuildNone();
}
PyObject* systemGetShopNameStatus(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetShopNameStatus());
}
#endif

#ifdef ENABLE_DISCORD_STUFF
PyObject* systemSetStuffMessage(PyObject* poSelf, PyObject* poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();
	CPythonSystem::Instance().SetStuffMessage(iValue);
	return Py_BuildNone();
}
PyObject* systemGetStuffMessage(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetStuffMessage());
}
#endif

#ifdef ENABLE_TRACK_WINDOW
PyObject* systemSetDungeonTrack(PyObject* poSelf, PyObject* poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();
	CPythonSystem::Instance().SetDungeonTrack(iValue);
	return Py_BuildNone();
}
PyObject* systemGetDungeonTrack(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetDungeonTrack());
}
PyObject* systemSetBossTrack(PyObject* poSelf, PyObject* poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();
	CPythonSystem::Instance().SetBossTrack(iValue);
	return Py_BuildNone();
}
PyObject* systemGetBossTrack(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetBossTrack());
}
#endif

#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
PyObject* systemSetMapObjectDistance(PyObject* poSelf, PyObject* poArgs)
{
	float fValue;
	if (!PyTuple_GetFloat(poArgs, 0, &fValue))
		return Py_BuildException();
	CPythonSystem::Instance().SetObjectDistance(fValue);
	return Py_BuildNone();
}
PyObject* systemGetMapObjectDistance(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetObjectDistance());
}
#endif

PyObject* systemSetDamageScale(PyObject* poSelf, PyObject* poArgs)
{
	float fVal;
	if (!PyTuple_GetFloat(poArgs, 0, &fVal))
		return Py_BuildException();
	CPythonSystem::Instance().SetScaleDamage(fVal);
	return Py_BuildNone();
}
PyObject* systemGetDamageScale(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetScaleDamage());
}

#ifdef STONE_SCALE
PyObject* systemGetScaleStone(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetStoneScale());
}

PyObject* systemSetScaleStone(PyObject* poSelf, PyObject* poArgs)
{
	float fScale;
	if (!PyTuple_GetFloat(poArgs, 0, &fScale))
		return Py_BuildException();

	CPythonSystem::Instance().SetStoneScale(fScale);
	return Py_BuildNone();
}
#endif

PyObject* systemSetInventoryChange(PyObject* poSelf, PyObject* poArgs)
{
	BYTE iIndex;
	if (!PyTuple_GetByte(poArgs, 0, &iIndex))
		return Py_BuildException();
	CPythonSystem::Instance().SetInventoryChange(iIndex);
	return Py_BuildNone();
}

PyObject* systemGetInventoryChange(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetInventoryChange());
}

PyObject* systemSetFogEnvironment(PyObject*/* poSelf*/, PyObject* poArgs)
{
	bool bFlag;
	if (!PyTuple_GetBoolean(poArgs, 0, &bFlag))
	{
		return Py_BuildException();
	}

	CPythonSystem::Instance().SetFogEnvironment(bFlag);
	return Py_BuildNone();
}

PyObject* systemIsFogEnvironment(PyObject*/* poSelf*/, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsFogEnvironment());
}

PyObject* systemSetFogDistance(PyObject*/* poSelf*/, PyObject* poArgs)
{
	float fDist;
	if (!PyTuple_GetFloat(poArgs, 0, &fDist))
	{
		return Py_BuildException();
	}

	CPythonSystem::Instance().SetFogDistance(fDist);
	return Py_BuildNone();
}

PyObject* systemGetFogDistance(PyObject*/* poSelf*/, PyObject* poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetFogDistance());
}

PyObject* systemSetNightEnvironment(PyObject*/* poSelf*/, PyObject* poArgs)
{
	bool bFlag;
	if (!PyTuple_GetBoolean(poArgs, 0, &bFlag))
	{
		return Py_BuildException();
	}

	CPythonSystem::Instance().SetNightEnvironment(bFlag);
	return Py_BuildNone();
}

PyObject* systemIsNightEnvironment(PyObject*/* poSelf*/, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsNightEnvironment());
}

PyObject* systemSetSnowingEnvironment(PyObject*/* poSelf*/, PyObject* poArgs)
{
	bool bFlag;
	if (!PyTuple_GetBoolean(poArgs, 0, &bFlag))
	{
		return Py_BuildException();
	}

	CPythonSystem::Instance().SetSnowingEnvironment(bFlag);
	return Py_BuildNone();
}

PyObject* systemIsSnowingEnvironment(PyObject*/* poSelf*/, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsSnowingEnvironment());
}

PyObject* systemSetSnowTerrainTexture(PyObject*/* poSelf*/, PyObject* poArgs)
{
	bool bFlag;
	if (!PyTuple_GetBoolean(poArgs, 0, &bFlag))
	{
		return Py_BuildException();
	}

	CPythonSystem::Instance().SetSnowTerrainTexture(bFlag);
	return Py_BuildNone();
}

PyObject* systemIsSnowTerrainTexture(PyObject*/* poSelf*/, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsSnowTerrainTexture());
}

PyObject* systemSetShadowLevel(PyObject*/* poSelf*/, PyObject* poArgs)
{
	BYTE bLevel;
	if (!PyTuple_GetByte(poArgs, 0, &bLevel))
		return Py_BuildException();

	return Py_BuildValue("b", CPythonSystem::Instance().SetShadowLevel(bLevel));
}

PyObject* systemGetShadowLevel(PyObject*/* poSelf*/, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetShadowLevel());
}

void initsystemSetting()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetWidth",					systemGetWidth,					METH_VARARGS },
		{ "GetHeight",					systemGetHeight,				METH_VARARGS },

		{ "SetInterfaceHandler",		systemSetInterfaceHandler,		METH_VARARGS },
		{ "DestroyInterfaceHandler",	systemDestroyInterfaceHandler,	METH_VARARGS },
		{ "ReserveResource",			systemReserveResource,			METH_VARARGS },

		{ "isInterfaceConfig",			systemisInterfaceConfig,		METH_VARARGS },
		{ "SaveWindowStatus",			systemSaveWindowStatus,			METH_VARARGS },
		{ "GetWindowStatus",			systemGetWindowStatus,			METH_VARARGS },

		{ "GetResolutionCount",			systemGetResolutionCount,		METH_VARARGS },
		{ "GetFrequencyCount",			systemGetFrequencyCount,		METH_VARARGS },

		{ "GetCurrentResolution",		systemGetCurrentResolution,		METH_VARARGS },

		{ "GetResolution",				systemGetResolution,			METH_VARARGS },
		{ "GetFrequency",				systemGetFrequency,				METH_VARARGS },

		{ "ApplyConfig",				systemApplyConfig,				METH_VARARGS },
		{ "SetConfig",					systemSetConfig,				METH_VARARGS },
		{ "SaveConfig",					systemSaveConfig,				METH_VARARGS },
		{ "GetConfig",					systemGetConfig,				METH_VARARGS },

		{ "SetSaveID",					systemSetSaveID,				METH_VARARGS },
		{ "isSaveID",					systemisSaveID,					METH_VARARGS },
		{ "GetSaveID",					systemGetSaveID,				METH_VARARGS },

		{ "GetMusicVolume",				systemGetMusicVolume,			METH_VARARGS },
		{ "GetSoundVolume",				systemGetSoundVolume,			METH_VARARGS },

		{ "SetMusicVolume",				systemSetMusicVolume,			METH_VARARGS },
		{ "SetSoundVolumef",			systemSetSoundVolumef,			METH_VARARGS },
		{ "IsSoftwareCursor",			systemIsSoftwareCursor,			METH_VARARGS },

		{ "SetViewChatFlag",			systemSetViewChatFlag,			METH_VARARGS },
		{ "IsViewChat",					systemIsViewChat,				METH_VARARGS },

		{ "SetAlwaysShowNameFlag",		systemSetAlwaysShowNameFlag,	METH_VARARGS },
		{ "IsAlwaysShowName",			systemIsAlwaysShowName,			METH_VARARGS },

		{ "SetShowDamageFlag",			systemSetShowDamageFlag,		METH_VARARGS },
		{ "IsShowDamage",				systemIsShowDamage,				METH_VARARGS },

		{ "SetShowSalesTextFlag",		systemSetShowSalesTextFlag,		METH_VARARGS },
		{ "IsShowSalesText",			systemIsShowSalesText,			METH_VARARGS },
		{"SetShowOutlineFlag",			systemSetShowOutlineFlag,		METH_VARARGS},
		{"IsShowOutline",				systemIsShowOutline,			METH_VARARGS},

#ifdef WJ_SHOW_MOB_INFO
		{ "IsShowMobAIFlag",			systemIsShowMobAIFlag,			METH_VARARGS },
		{ "SetShowMobAIFlag",			systemSetShowMobAIFlag,			METH_VARARGS },

		{ "IsShowMobLevel",				systemIsShowMobLevel,			METH_VARARGS },
		{ "SetShowMobLevel",			systemSetShowMobLevel,			METH_VARARGS },
#endif

#ifdef ENABLE_NEW_GAMEOPTION
		{ "SetQuestIconMode",			systemSetQuestIconMode, METH_VARARGS },
		{ "GetQuestIconMode",			systemGetQuestIconMode, METH_VARARGS },
		{ "SetCountryFlag",			systemSetCountryFlag, METH_VARARGS },
		{ "GetCountryFlag",			systemGetCountryFlag, METH_VARARGS },
		{ "SetAffectIcons",			systemSetAffectIcons, METH_VARARGS },
		{ "GetAffectIcons",			systemGetAffectIcons, METH_VARARGS },
#endif

		{ "GetFOVLevel",                systemGetFOVLevel,                METH_VARARGS },
		{ "SetFOVLevel",                systemSetFOVLevel,                METH_VARARGS },
#ifdef ENABLE_GRAPHIC_ON_OFF
		{ "SetEffectLevel",                systemSetEffectLevel,                METH_VARARGS },
		{ "GetEffectLevel",                systemGetEffectLevel,                METH_VARARGS },

		{ "GetPetRender",                systemGetPetRender,                METH_VARARGS },
		{ "SetPetRender",                systemSetPetRender,                METH_VARARGS },
		{ "GetMountRender",                systemGetMountRender,                METH_VARARGS },
		{ "SetMountRender",                systemSetMountRender,                METH_VARARGS },
		{ "SetShopNameStatus",                systemSetShopNameStatus,                METH_VARARGS },
		{ "GetShopNameStatus",                systemGetShopNameStatus,                METH_VARARGS },
#endif

#ifdef ENABLE_DISCORD_STUFF
		{ "SetStuffMessage",                systemSetStuffMessage,                METH_VARARGS },
		{ "GetStuffMessage",                systemGetStuffMessage,                METH_VARARGS },
#endif
#ifdef ENABLE_TRACK_WINDOW
		{ "SetDungeonTrack",			systemSetDungeonTrack,                METH_VARARGS },
		{ "GetDungeonTrack",			systemGetDungeonTrack,                METH_VARARGS },
		{ "SetBossTrack",                systemSetBossTrack,                METH_VARARGS },
		{ "GetBossTrack",                systemGetBossTrack,                METH_VARARGS },
#endif

#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
		{ "SetMapObjectDistance",	systemSetMapObjectDistance,                METH_VARARGS },
		{ "GetMapObjectDistance",	systemGetMapObjectDistance,                METH_VARARGS },
#endif
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_PRO
		{"GetShopRangeView",			systemGetShopRangeView,			METH_VARARGS },
		{"SetShopRangeView",			systemSetShopRangeView,			METH_VARARGS },
#endif
#endif

		{ "SetDamageScale",                systemSetDamageScale,                METH_VARARGS },
		{ "GetDamageScale",                systemGetDamageScale,                METH_VARARGS },

#ifdef STONE_SCALE
		{ "GetStoneScale",				systemGetScaleStone,			METH_VARARGS },
		{ "SetStoneScale",				systemSetScaleStone,			METH_VARARGS },
#endif
		{"SetInventoryChange",			systemSetInventoryChange,			METH_VARARGS },
		{"GetInventoryChange",			systemGetInventoryChange,			METH_VARARGS },

		{ "SetFogEnvironment", systemSetFogEnvironment, METH_VARARGS },
		{ "IsFogEnvironment", systemIsFogEnvironment, METH_VARARGS },

		{ "SetFogDistance", systemSetFogDistance, METH_VARARGS },
		{ "GetFogDistance", systemGetFogDistance, METH_VARARGS },

		{ "SetNightEnvironment", systemSetNightEnvironment, METH_VARARGS },
		{ "IsNightEnvironment", systemIsNightEnvironment, METH_VARARGS },

		{ "SetSnowingEnvironment", systemSetSnowingEnvironment, METH_VARARGS },
		{ "IsSnowingEnvironment", systemIsSnowingEnvironment, METH_VARARGS },

		{ "SetSnowTerrainTexture", systemSetSnowTerrainTexture, METH_VARARGS },
		{ "IsSnowTerrainTexture", systemIsSnowTerrainTexture, METH_VARARGS },

		{ "SetShadowLevel", systemSetShadowLevel, METH_VARARGS },
		{ "GetShadowLevel", systemGetShadowLevel, METH_VARARGS },

		{ NULL,							NULL,							NULL }
	};

	PyObject * poModule = Py_InitModule("systemSetting", s_methods);

	PyModule_AddIntConstant(poModule, "WINDOW_STATUS",		CPythonSystem::WINDOW_STATUS);
	PyModule_AddIntConstant(poModule, "WINDOW_INVENTORY",	CPythonSystem::WINDOW_INVENTORY);
	PyModule_AddIntConstant(poModule, "WINDOW_ABILITY",		CPythonSystem::WINDOW_ABILITY);
	PyModule_AddIntConstant(poModule, "WINDOW_SOCIETY",		CPythonSystem::WINDOW_SOCIETY);
	PyModule_AddIntConstant(poModule, "WINDOW_JOURNAL",		CPythonSystem::WINDOW_JOURNAL);
	PyModule_AddIntConstant(poModule, "WINDOW_COMMAND",		CPythonSystem::WINDOW_COMMAND);

	PyModule_AddIntConstant(poModule, "WINDOW_QUICK",		CPythonSystem::WINDOW_QUICK);
	PyModule_AddIntConstant(poModule, "WINDOW_GAUGE",		CPythonSystem::WINDOW_GAUGE);
	PyModule_AddIntConstant(poModule, "WINDOW_MINIMAP",		CPythonSystem::WINDOW_MINIMAP);
	PyModule_AddIntConstant(poModule, "WINDOW_CHAT",		CPythonSystem::WINDOW_CHAT);
}
