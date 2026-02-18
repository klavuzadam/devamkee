#include "StdAfx.h"
#include "Resource.h"
#include "PythonApplication.h"
#include "../EterLib/Camera.h"
#include "Locale.h"
#include "../GameLib/GameLibDefines.h"
#include <Shellapi.h>

extern bool PERF_CHECKER_RENDER_GAME;
extern D3DXCOLOR g_fSpecularColor;
extern BOOL bVisibleNotice = true;
extern BOOL bTestServerFlag = FALSE;
extern int TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE = 0;

PyObject* appEnablePerformanceTime(PyObject* poSelf, PyObject* poArgs)
{
	char* szMode;
	if (!PyTuple_GetString(poArgs, 0, &szMode))
		return Py_BuildException();

	int nEnable;
	if (!PyTuple_GetInteger(poArgs, 1, &nEnable))
		return Py_BuildException();

	bool isEnable=nEnable ? true : false;

	if (strcmp(szMode, "RENDER_GAME")==0)
		PERF_CHECKER_RENDER_GAME = isEnable;

	return Py_BuildNone();
}

/////////////////////////////////////////////////////

extern BOOL HAIR_COLOR_ENABLE;
extern BOOL USE_ARMOR_SPECULAR;
extern BOOL USE_WEAPON_SPECULAR;
extern BOOL SKILL_EFFECT_UPGRADE_ENABLE;
extern BOOL RIDE_HORSE_ENABLE;
extern double g_specularSpd;

// TEXTTAIL_LIVINGTIME_CONTROL
extern void TextTail_SetLivingTime(long livingTime);

PyObject* appSetTextTailLivingTime(PyObject* poSelf, PyObject* poArgs)
{
	float livingTime;
	if (!PyTuple_GetFloat(poArgs, 0, &livingTime))
		return Py_BuildException();

	TextTail_SetLivingTime(livingTime*1000);

	return Py_BuildNone();
}
// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

PyObject* appHttpRequest(PyObject* poSelf, PyObject* poArgs)
{
	char* sUrl;
	if (!PyTuple_GetString(poArgs, 0, &sUrl))
		return Py_BuildException();
	const std::string result = httpGet(sUrl);
	return Py_BuildValue("s", result.c_str());
}

PyObject* appSetHairColorEnable(PyObject* poSelf, PyObject* poArgs)
{
	int nEnable;
	if (!PyTuple_GetInteger(poArgs, 0, &nEnable))
		return Py_BuildException();

	HAIR_COLOR_ENABLE=nEnable;

	return Py_BuildNone();
}

PyObject* appSetArmorSpecularEnable(PyObject* poSelf, PyObject* poArgs)
{
	int nEnable;
	if (!PyTuple_GetInteger(poArgs, 0, &nEnable))
		return Py_BuildException();

	USE_ARMOR_SPECULAR=nEnable;

	return Py_BuildNone();
}

PyObject* appSetWeaponSpecularEnable(PyObject* poSelf, PyObject* poArgs)
{
	int nEnable;
	if (!PyTuple_GetInteger(poArgs, 0, &nEnable))
		return Py_BuildException();

	USE_WEAPON_SPECULAR=nEnable;

	return Py_BuildNone();
}

PyObject* appSetSkillEffectUpgradeEnable(PyObject* poSelf, PyObject* poArgs)
{
	int nEnable;
	if (!PyTuple_GetInteger(poArgs, 0, &nEnable))
		return Py_BuildException();

	SKILL_EFFECT_UPGRADE_ENABLE=nEnable;

	return Py_BuildNone();
}

PyObject* SetTwoHandedWeaponAttSpeedDecreaseValue(PyObject* poSelf, PyObject* poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();

	TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE = iValue;

	return Py_BuildNone();
}

PyObject* appSetRideHorseEnable(PyObject* poSelf, PyObject* poArgs)
{
	int nEnable;
	if (!PyTuple_GetInteger(poArgs, 0, &nEnable))
		return Py_BuildException();

	RIDE_HORSE_ENABLE=nEnable;

	return Py_BuildNone();
}

PyObject* appSetCameraMaxDistance(PyObject* poSelf, PyObject* poArgs)
{
	float fMax;
	if (!PyTuple_GetFloat(poArgs, 0, &fMax))
		return Py_BuildException();

	CCamera::SetCameraMaxDistance(fMax);
	return Py_BuildNone();
}

PyObject* appSetControlFP(PyObject* poSelf, PyObject* poArgs)
{
	_controlfp( _PC_24, _MCW_PC );
	return Py_BuildNone();
}

PyObject* appSetSpecularSpeed(PyObject* poSelf, PyObject* poArgs)
{
	float fSpeed;
	if (!PyTuple_GetFloat(poArgs, 0, &fSpeed))
		return Py_BuildException();

	g_specularSpd = fSpeed;

	return Py_BuildNone();
}

PyObject * appSetMinFog(PyObject * poSelf, PyObject * poArgs)
{
	float fMinFog;
	if (!PyTuple_GetFloat(poArgs, 0, &fMinFog))
		return Py_BuildException();

	CPythonApplication::Instance().SetMinFog(fMinFog);
	return Py_BuildNone();
}

PyObject* appSetFrameSkip(PyObject* poSelf, PyObject* poArgs)
{
	int nFrameSkip;
	if (!PyTuple_GetInteger(poArgs, 0, &nFrameSkip))
		return Py_BuildException();

	CPythonApplication::Instance().SetFrameSkip(nFrameSkip ? true : false);
	return Py_BuildNone();
}

// LOCALE

PyObject* appForceSetLocale(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	char* szLocalePath;
	if (!PyTuple_GetString(poArgs, 1, &szLocalePath))
		return Py_BuildException();

	LocaleService_ForceSetLocale(szName, szLocalePath);

	return Py_BuildNone();
}

PyObject* appGetLocaleServiceName(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("s", LocaleService_GetName());
}

//
bool LoadLocaleData(const char* localePath);

PyObject* appSetCHEONMA(PyObject* poSelf, PyObject* poArgs)
{
	int enable;
	if (!PyTuple_GetInteger(poArgs, 0, &enable))
		return Py_BuildException();

	LocaleService_SetCHEONMA(enable ? true : false);
	return Py_BuildNone();
}

PyObject* appIsCHEONMA(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", LocaleService_IsCHEONMA());
}

#include "../eterBase/tea.h"

PyObject* appLoadLocaleAddr(PyObject* poSelf, PyObject* poArgs)
{
	char* addrPath;
	if (!PyTuple_GetString(poArgs, 0, &addrPath))
		return Py_BuildException();

	FILE* fp = fopen(addrPath, "rb");
	if (!fp)
		return Py_BuildException();

	fseek(fp, 0, SEEK_END);

	int size = ftell(fp);
	char* enc = (char*)_alloca(size);
	fseek(fp, 0, SEEK_SET);
	fread(enc, size, 1, fp);
	fclose(fp);

	static const unsigned char key[16] = {
		0x82, 0x1b, 0x34, 0xae,
		0x12, 0x3b, 0xfb, 0x17,
		0xd7, 0x2c, 0x39, 0xae,
		0x41, 0x98, 0xf1, 0x63
	};

	char* buf = (char*)_alloca(size);
	//int decSize =
	tea_decrypt((unsigned long*)buf, (const unsigned long*)enc, (const unsigned long*)key, size);
	unsigned int retSize = *(unsigned int*)buf;
	char* ret = buf + sizeof(unsigned int);
	return Py_BuildValue("s#", ret, retSize);
}

PyObject* appLoadLocaleData(PyObject* poSelf, PyObject* poArgs)
{
	char* localePath;
	if (!PyTuple_GetString(poArgs, 0, &localePath))
		return Py_BuildException();

	return Py_BuildValue("i", LoadLocaleData(localePath));
}

PyObject* appGetLocaleName(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("s", LocaleService_GetLocaleName());
}

PyObject* appGetLocalePath(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("s", LocaleService_GetLocalePath());
}
// END_OF_LOCALE

PyObject* appGetDefaultCodePage(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", LocaleService_GetCodePage());
}

#ifdef __VTUNE__

PyObject* appGetImageInfo(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	return Py_BuildValue("iii", 0, 0, 0);
}

#else

#include <il/il.h>

PyObject* appGetImageInfo(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	BOOL canLoad=FALSE;
	ILuint uWidth=0;
	ILuint uHeight=0;

	ILuint uImg;
	ilGenImages(1, &uImg);
	ilBindImage(uImg);
	if (ilLoad(IL_TYPE_UNKNOWN, szFileName))
	{
		canLoad=TRUE;
		uWidth=ilGetInteger(IL_IMAGE_WIDTH);
		uHeight=ilGetInteger(IL_IMAGE_HEIGHT);
	}

	ilDeleteImages(1, &uImg);

	return Py_BuildValue("iii", canLoad, uWidth, uHeight);
}
#endif

#include "../EterPack/EterPackManager.h"

PyObject* appIsExistFile(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	bool isExist=CEterPackManager::Instance().isExist(szFileName, __FUNCTION__, false);

	return Py_BuildValue("i", isExist);
}

PyObject* appGetFileList(PyObject* poSelf, PyObject* poArgs)
{
	char* szFilter;
	if (!PyTuple_GetString(poArgs, 0, &szFilter))
		return Py_BuildException();

	PyObject* poList=PyList_New(0);

	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(wfd));

	HANDLE hFind = FindFirstFile(szFilter, &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			PyObject* poFileName=PyString_FromString(wfd.cFileName) ;
			PyList_Append(poList, poFileName);
		}
		while (FindNextFile(hFind, &wfd));


		FindClose(hFind);
	}

	return poList;
}


PyObject* appUpdateGame(PyObject* poSelf, PyObject* poArgs)
{
	CPythonApplication::Instance().UpdateGame();
	return Py_BuildNone();
}

PyObject* appRenderGame(PyObject* poSelf, PyObject* poArgs)
{
	CPythonApplication::Instance().RenderGame();
	return Py_BuildNone();
}



PyObject* appSetMouseHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poHandler;
	if (!PyTuple_GetObject(poArgs, 0, &poHandler))
		return Py_BuildException();

	CPythonApplication::Instance().SetMouseHandler(poHandler);
	return Py_BuildNone();
}

PyObject* appCreate(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
#ifdef ENABLE_DISCORD_STUFF
	char* szMiniName;
#endif
	int width, height, Windowed;
	PyTuple_GetString(poArgs, 0, &szName);
#ifdef ENABLE_DISCORD_STUFF
	PyTuple_GetString(poArgs, 1, &szMiniName);
	PyTuple_GetInteger(poArgs, 2, &width);
	PyTuple_GetInteger(poArgs, 3, &height);
	PyTuple_GetInteger(poArgs, 4, &Windowed);
#else
	PyTuple_GetInteger(poArgs, 1, &width);
	PyTuple_GetInteger(poArgs, 2, &height);
	PyTuple_GetInteger(poArgs, 3, &Windowed);
#endif

    static auto& appMgr = CPythonApplication::Instance();
#ifdef ENABLE_DISCORD_STUFF
    if (!appMgr.Create(poSelf, szName, szMiniName, width, height, Windowed))
#else
    if (!appMgr.Create(poSelf, szName, width, height, Windowed))
#endif
    {
        return nullptr;
    }

    LoadPythonPlayerSettingsModuleLibrary("Metin2Release.exe");

    CPythonPlayerSettingsModule::Load();

    appMgr.SetVisibleMode(true);
    return Py_BuildNone();
}

PyObject* appLoop(PyObject* poSelf, PyObject* poArgs)
{
	CPythonApplication::Instance().Loop();

	return Py_BuildNone();
}

PyObject* appGetInfo(PyObject* poSelf, PyObject* poArgs)
{
	int nInfo;
	if (!PyTuple_GetInteger(poArgs, 0, &nInfo))
		return Py_BuildException();

	std::string stInfo;
	CPythonApplication::Instance().GetInfo(nInfo, &stInfo);
	return Py_BuildValue("s", stInfo.c_str());
}

PyObject* appProcess(PyObject* poSelf, PyObject* poArgs)
{
	if (CPythonApplication::Instance().Process())
		return Py_BuildValue("i", 1);

	return Py_BuildValue("i", 0);
}

PyObject* appAbort(PyObject* poSelf, PyObject* poArgs)
{
	CPythonApplication::Instance().Abort();
	return Py_BuildNone();
}

PyObject* appExit(PyObject* poSelf, PyObject* poArgs)
{
	CPythonApplication::Instance().Exit();
	return Py_BuildNone();
}

PyObject * appSetCamera(PyObject * poSelf, PyObject * poArgs)
{
	float Distance;
	if (!PyTuple_GetFloat(poArgs, 0, &Distance))
		return Py_BuildException();

	float Pitch;
	if (!PyTuple_GetFloat(poArgs, 1, &Pitch))
		return Py_BuildException();

	float Rotation;
	if (!PyTuple_GetFloat(poArgs, 2, &Rotation))
		return Py_BuildException();

	float fDestinationHeight;
	if (!PyTuple_GetFloat(poArgs, 3, &fDestinationHeight))
		return Py_BuildException();

	CPythonApplication::Instance().SetCamera(Distance, Pitch, Rotation, fDestinationHeight);
	return Py_BuildNone();
}

PyObject * appGetCamera(PyObject * poSelf, PyObject * poArgs)
{
	float Distance, Pitch, Rotation, DestinationHeight;
    CPythonApplication::Instance().GetCamera(&Distance, &Pitch, &Rotation, &DestinationHeight);

	return Py_BuildValue("ffff", Distance, Pitch, Rotation, DestinationHeight);
}

PyObject * appGetCameraPitch(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("f", CPythonApplication::Instance().GetPitch());
}

PyObject * appGetCameraRotation(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("f", CPythonApplication::Instance().GetRotation());
}

PyObject * appGetTime(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("f", CPythonApplication::Instance().GetGlobalTime());
}

PyObject * appGetGlobalTime(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().GetServerTime());
}

PyObject * appGetGlobalTimeStamp(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().GetServerTimeStamp());
}

PyObject * appGetUpdateFPS(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().GetUpdateFPS());
}

PyObject * appGetRenderFPS(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().GetRenderFPS());
}

PyObject * appRotateCamera(PyObject * poSelf, PyObject * poArgs)
{
	int iDirection;
	if (!PyTuple_GetInteger(poArgs, 0, &iDirection))
		return Py_BuildException();
	CPythonApplication::Instance().RotateCamera(iDirection);
	return Py_BuildNone();
}

PyObject * appPitchCamera(PyObject * poSelf, PyObject * poArgs)
{
	int iDirection;
	if (!PyTuple_GetInteger(poArgs, 0, &iDirection))
		return Py_BuildException();
	CPythonApplication::Instance().PitchCamera(iDirection);
	return Py_BuildNone();
}

PyObject * appZoomCamera(PyObject * poSelf, PyObject * poArgs)
{
	int iDirection;
	if (!PyTuple_GetInteger(poArgs, 0, &iDirection))
		return Py_BuildException();
	CPythonApplication::Instance().ZoomCamera(iDirection);
	return Py_BuildNone();
}

PyObject * appMovieRotateCamera(PyObject * poSelf, PyObject * poArgs)
{
	int iDirection;
	if (!PyTuple_GetInteger(poArgs, 0, &iDirection))
		return Py_BuildException();
	CPythonApplication::Instance().MovieRotateCamera(iDirection);
	return Py_BuildNone();
}

PyObject * appMoviePitchCamera(PyObject * poSelf, PyObject * poArgs)
{
	int iDirection;
	if (!PyTuple_GetInteger(poArgs, 0, &iDirection))
		return Py_BuildException();
	CPythonApplication::Instance().MoviePitchCamera(iDirection);
	return Py_BuildNone();
}

PyObject * appMovieZoomCamera(PyObject * poSelf, PyObject * poArgs)
{
	int iDirection;
	if (!PyTuple_GetInteger(poArgs, 0, &iDirection))
		return Py_BuildException();
	CPythonApplication::Instance().MovieZoomCamera(iDirection);
	return Py_BuildNone();
}

PyObject * appMovieResetCamera(PyObject * poSelf, PyObject * poArgs)
{
	CPythonApplication::Instance().MovieResetCamera();
	return Py_BuildNone();
}

PyObject * appGetFaceSpeed(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("f", CPythonApplication::Instance().GetFaceSpeed());
}

PyObject * appGetRenderTime(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("fi",
		CPythonApplication::Instance().GetAveRenderTime(),
		CPythonApplication::Instance().GetCurRenderTime());
}

PyObject * appGetUpdateTime(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().GetCurUpdateTime());
}

PyObject * appGetLoad(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().GetLoad());
}
PyObject * appGetFaceCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().GetFaceCount());
}

PyObject * appGetAvaiableTextureMememory(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CGraphicBase::GetAvailableTextureMemory());
}

PyObject * appSetFPS(PyObject * poSelf, PyObject * poArgs)
{
	int	iFPS;
	if (!PyTuple_GetInteger(poArgs, 0, &iFPS))
		return Py_BuildException();

	CPythonApplication::Instance().SetFPS(iFPS);

	return Py_BuildNone();
}

PyObject * appSetGlobalCenterPosition(PyObject * poSelf, PyObject * poArgs)
{
	int x;
	if (!PyTuple_GetInteger(poArgs, 0, &x))
		return Py_BuildException();

	int y;
	if (!PyTuple_GetInteger(poArgs, 1, &y))
		return Py_BuildException();

	CPythonApplication::Instance().SetGlobalCenterPosition(x, y);
	return Py_BuildNone();
}


PyObject * appSetCenterPosition(PyObject * poSelf, PyObject * poArgs)
{
	float fx;
	if (!PyTuple_GetFloat(poArgs, 0, &fx))
		return Py_BuildException();

	float fy;
	if (!PyTuple_GetFloat(poArgs, 1, &fy))
		return Py_BuildException();

	float fz;
	if (!PyTuple_GetFloat(poArgs, 2, &fz))
		return Py_BuildException();

	CPythonApplication::Instance().SetCenterPosition(fx, -fy, fz);
	return Py_BuildNone();
}

PyObject * appGetCursorPosition(PyObject * poSelf, PyObject * poArgs)
{
	long lx, ly;
	UI::CWindowManager& rkWndMgr=UI::CWindowManager::Instance();
	rkWndMgr.GetMousePosition(lx, ly);

	return Py_BuildValue("ii", lx, ly);
}

PyObject * appRunPythonFile(PyObject * poSelf, PyObject * poArgs)
{
	char *szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	bool ret = CPythonLauncher::Instance().RunFile(szFileName);
	return Py_BuildValue("i", ret);
}

PyObject * appIsPressed(PyObject * poSelf, PyObject * poArgs)
{
	int iKey;
	if (!PyTuple_GetInteger(poArgs, 0, &iKey))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonApplication::Instance().IsPressed(iKey));
}

PyObject * appSetCursor(PyObject * poSelf, PyObject * poArgs)
{
/*
	char * szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	if (!CPythonApplication::Instance().SetHardwareCursor(szName))
		return Py_BuildException("Wrong Cursor Name [%s]", szName);
*/
	int iCursorNum;
	if (!PyTuple_GetInteger(poArgs, 0, &iCursorNum))
		return Py_BuildException();

	if (!CPythonApplication::Instance().SetCursorNum(iCursorNum))
		return Py_BuildException("Wrong Cursor Name [%d]", iCursorNum);

	return Py_BuildNone();
}

PyObject * appGetCursor(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().GetCursorNum());
}

PyObject * appShowCursor(PyObject * poSelf, PyObject * poArgs)
{
	CPythonApplication::Instance().SetCursorVisible(TRUE);

	return Py_BuildNone();
}

PyObject * appHideCursor(PyObject * poSelf, PyObject * poArgs)
{
	CPythonApplication::Instance().SetCursorVisible(FALSE);

	return Py_BuildNone();
}

PyObject * appIsShowCursor(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", TRUE == CPythonApplication::Instance().GetCursorVisible());
}

PyObject * appIsLiarCursorOn(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", TRUE == CPythonApplication::Instance().GetLiarCursorOn());
}

PyObject * appSetSoftwareCursor(PyObject * poSelf, PyObject * poArgs)
{
	CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_SOFTWARE);
	return Py_BuildNone();
}

PyObject * appSetHardwareCursor(PyObject * poSelf, PyObject * poArgs)
{
	CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_HARDWARE);
	return Py_BuildNone();
}

PyObject * appSetConnectData(PyObject * poSelf, PyObject * poArgs)
{
	char * szIP;
	if (!PyTuple_GetString(poArgs, 0, &szIP))
		return Py_BuildException();

	int	iPort;
	if (!PyTuple_GetInteger(poArgs, 1, &iPort))
		return Py_BuildException();

	CPythonApplication::Instance().SetConnectData(szIP, iPort);

	return Py_BuildNone();
}

PyObject * appGetConnectData(PyObject * poSelf, PyObject * poArgs)
{
	std::string strIP;
	int iPort;

	CPythonApplication::Instance().GetConnectData(strIP, iPort);

	return Py_BuildValue("si", strIP.c_str(), iPort);
}

PyObject * appGetRandom(PyObject * poSelf, PyObject * poArgs)
{
	int from;
	if (!PyTuple_GetInteger(poArgs, 0, &from))
		return Py_BuildException();

	int	to;
	if (!PyTuple_GetInteger(poArgs, 1, &to))
		return Py_BuildException();

	if (from > to)
	{
		int tmp = from;
		from = to;
		to = tmp;
	}

	return Py_BuildValue("i", random_range(from, to));
}

PyObject * appGetRotatingDirection(PyObject * poSelf, PyObject * poArgs)
{
	float fSource;
	if (!PyTuple_GetFloat(poArgs, 0, &fSource))
		return Py_BuildException();
	float fTarget;
	if (!PyTuple_GetFloat(poArgs, 1, &fTarget))
		return Py_BuildException();

	return Py_BuildValue("i", GetRotatingDirection(fSource, fTarget));
}

PyObject * appGetDegreeDifference(PyObject * poSelf, PyObject * poArgs)
{
	float fSource;
	if (!PyTuple_GetFloat(poArgs, 0, &fSource))
		return Py_BuildException();
	float fTarget;
	if (!PyTuple_GetFloat(poArgs, 1, &fTarget))
		return Py_BuildException();

	return Py_BuildValue("f", GetDegreeDifference(fSource, fTarget));
}

PyObject * appSleep(PyObject * poSelf, PyObject * poArgs)
{
	int	iTime;
	if (!PyTuple_GetInteger(poArgs, 0, &iTime))
		return Py_BuildException();

	Sleep(iTime);

	return Py_BuildNone();
}

PyObject * appSetDefaultFontName(PyObject * poSelf, PyObject * poArgs)
{
	char * szFontName;
	if (!PyTuple_GetString(poArgs, 0, &szFontName))
		return Py_BuildException();

	// DEFAULT_FONT
	DefaultFont_SetName(szFontName);
	// END_OF_DEFAULT_FONT

	return Py_BuildNone();
}

PyObject * appSetGuildSymbolPath(PyObject * poSelf, PyObject * poArgs)
{
	char * szPathName;
	if (!PyTuple_GetString(poArgs, 0, &szPathName))
		return Py_BuildException();

	SetGuildSymbolPath(szPathName);

	return Py_BuildNone();
}

PyObject * appEnableSpecialCameraMode(PyObject * poSelf, PyObject * poArgs)
{
	CPythonApplication::Instance().EnableSpecialCameraMode();
	return Py_BuildNone();
}

PyObject * appSetCameraSpeed(PyObject * poSelf, PyObject * poArgs)
{
	int iPercentage;
	if (!PyTuple_GetInteger(poArgs, 0, &iPercentage))
		return Py_BuildException();

	CPythonApplication::Instance().SetCameraSpeed(iPercentage);

	CCamera * pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (pCamera)
		pCamera->SetResistance(float(iPercentage) / 100.0f);
	return Py_BuildNone();
}

PyObject * appIsFileExist(PyObject * poSelf, PyObject * poArgs)
{
	char * szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	return Py_BuildValue("i", -1 != _access(szFileName, 0));
}

PyObject * appSetCameraSetting(PyObject * poSelf, PyObject * poArgs)
{
	int ix;
	if (!PyTuple_GetInteger(poArgs, 0, &ix))
		return Py_BuildException();
	int iy;
	if (!PyTuple_GetInteger(poArgs, 1, &iy))
		return Py_BuildException();
	int iz;
	if (!PyTuple_GetInteger(poArgs, 2, &iz))
		return Py_BuildException();

	int iZoom;
	if (!PyTuple_GetInteger(poArgs, 3, &iZoom))
		return Py_BuildException();
	int iRotation;
	if (!PyTuple_GetInteger(poArgs, 4, &iRotation))
		return Py_BuildException();
	int iPitch;
	if (!PyTuple_GetInteger(poArgs, 5, &iPitch))
		return Py_BuildException();

	CPythonApplication::SCameraSetting CameraSetting;
	ZeroMemory(&CameraSetting, sizeof(CameraSetting));
	CameraSetting.v3CenterPosition.x = float(ix);
	CameraSetting.v3CenterPosition.y = float(iy);
	CameraSetting.v3CenterPosition.z = float(iz);
	CameraSetting.fZoom = float(iZoom);
	CameraSetting.fRotation = float(iRotation);
	CameraSetting.fPitch = float(iPitch);
	CPythonApplication::Instance().SetEventCamera(CameraSetting);
	return Py_BuildNone();
}

PyObject * appSaveCameraSetting(PyObject * poSelf, PyObject * poArgs)
{
	char * szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	CPythonApplication::Instance().SaveCameraSetting(szFileName);
	return Py_BuildNone();
}

PyObject * appLoadCameraSetting(PyObject * poSelf, PyObject * poArgs)
{
	char * szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	bool bResult = CPythonApplication::Instance().LoadCameraSetting(szFileName);
	return Py_BuildValue("i", bResult);
}

PyObject * appSetDefaultCamera(PyObject * poSelf, PyObject * poArgs)
{
	CPythonApplication::Instance().SetDefaultCamera();
	return Py_BuildNone();
}

PyObject * appSetSightRange(PyObject * poSelf, PyObject * poArgs)
{
	int iRange;
	if (!PyTuple_GetInteger(poArgs, 0, &iRange))
		return Py_BuildException();

	CPythonApplication::Instance().SetForceSightRange(iRange);
	return Py_BuildNone();
}

extern int g_iAccumulationTime;

PyObject * apptestGetAccumulationTime(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", g_iAccumulationTime);
}

PyObject * apptestResetAccumulationTime(PyObject * poSelf, PyObject * poArgs)
{
	g_iAccumulationTime = 0;
	return Py_BuildNone();
}

PyObject * apptestSetSpecularColor(PyObject * poSelf, PyObject * poArgs)
{
	float fr;
	if (!PyTuple_GetFloat(poArgs, 0, &fr))
		return Py_BuildException();
	float fg;
	if (!PyTuple_GetFloat(poArgs, 1, &fg))
		return Py_BuildException();
	float fb;
	if (!PyTuple_GetFloat(poArgs, 2, &fb))
		return Py_BuildException();
	g_fSpecularColor = D3DXCOLOR(fr, fg, fb, 1.0f);
	return Py_BuildNone();
}

PyObject * appSetVisibleNotice(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	bVisibleNotice = iFlag;
	return Py_BuildNone();
}

PyObject * appIsVisibleNotice(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", bVisibleNotice);
}

PyObject * appEnableTestServerFlag(PyObject * poSelf, PyObject * poArgs)
{
	bTestServerFlag = TRUE;
	return Py_BuildNone();
}

PyObject * appIsEnableTestServerFlag(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", bTestServerFlag);
}

class CTextLineLoader
{
	public:
		CTextLineLoader(const char * c_szFileName)
		{
			const VOID* pvData;
			CMappedFile kFile;
			if (!CEterPackManager::Instance().Get(kFile, c_szFileName, &pvData, __FUNCTION__))
				return;

			m_kTextFileLoader.Bind(kFile.Size(), pvData);
		}

		DWORD GetLineCount()
		{
			return m_kTextFileLoader.GetLineCount();
		}

		const char * GetLine(DWORD dwIndex)
		{
			if (dwIndex >= GetLineCount())
				return "";

			return m_kTextFileLoader.GetLineString(dwIndex).c_str();
		}

	protected:
		CMemoryTextFileLoader m_kTextFileLoader;
};

PyObject * appOpenTextFile(PyObject * poSelf, PyObject * poArgs)
{
	char * szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	CTextLineLoader * pTextLineLoader = new CTextLineLoader(szFileName);

	return Py_BuildValue("i", (int)pTextLineLoader);
}

PyObject * appCloseTextFile(PyObject * poSelf, PyObject * poArgs)
{
	int iHandle;
	if (!PyTuple_GetInteger(poArgs, 0, &iHandle))
		return Py_BuildException();

	CTextLineLoader * pTextFileLoader = (CTextLineLoader *)iHandle;
	delete pTextFileLoader;

	return Py_BuildNone();
}

PyObject * appGetTextFileLineCount(PyObject * poSelf, PyObject * poArgs)
{
	int iHandle;
	if (!PyTuple_GetInteger(poArgs, 0, &iHandle))
		return Py_BuildException();

	CTextLineLoader * pTextFileLoader = (CTextLineLoader *)iHandle;
	return Py_BuildValue("i", pTextFileLoader->GetLineCount());
}

PyObject * appGetTextFileLine(PyObject * poSelf, PyObject * poArgs)
{
	int iHandle;
	if (!PyTuple_GetInteger(poArgs, 0, &iHandle))
		return Py_BuildException();
	int iLineIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iLineIndex))
		return Py_BuildException();

	CTextLineLoader * pTextFileLoader = (CTextLineLoader *)iHandle;
	return Py_BuildValue("s", pTextFileLoader->GetLine(iLineIndex));
}

PyObject * appSetGuildMarkPath(PyObject * poSelf, PyObject * poArgs)
{
	char * path;
	if (!PyTuple_GetString(poArgs, 0, &path))
		return Py_BuildException();

    char newPath[256];
    char * ext = strstr(path, ".tga");

    if (ext)
    {
		int extPos = ext - path;
        strncpy(newPath, path, extPos);
        newPath[extPos] = '\0';
    }
    else
        strncpy(newPath, path, sizeof(newPath)-1);

	CGuildMarkManager::Instance().SetMarkPathPrefix(newPath);
	return Py_BuildNone();
}

PyObject* appIsDevStage(PyObject* poSelf, PyObject* poArgs)
{
	int nIsDevelopmentStage = 0;
#if defined(LOCALE_SERVICE_STAGE_DEVELOPMENT)
	nIsDevelopmentStage = 1;
#endif
	return Py_BuildValue("i", nIsDevelopmentStage);
}

PyObject* appIsTestStage(PyObject* poSelf, PyObject* poArgs)
{
	int nIsTestStage = 0;
#if defined(LOCALE_SERVICE_STAGE_TEST)
	nIsTestStage = 1;
#endif
	return Py_BuildValue("i", nIsTestStage);
}

PyObject* appIsLiveStage(PyObject* poSelf, PyObject* poArgs)
{
	int nIsLiveStage = 0;
#if !defined(LOCALE_SERVICE_STAGE_TEST) && !defined(LOCALE_SERVICE_STAGE_DEVELOPMENT)
	nIsLiveStage = 1;
#endif
	return Py_BuildValue("i", nIsLiveStage);
}

PyObject* appLogoOpen(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	int nIsSuccess = 1; //CPythonApplication::Instance().OnLogoOpen(szName);
	CMovieMan::Instance().PlayLogo(szName);

	return Py_BuildValue("i", nIsSuccess);
}

PyObject* appLogoUpdate(PyObject* poSelf, PyObject* poArgs)
{
	int nIsRun = 0; //CPythonApplication::Instance().OnLogoUpdate();
	return Py_BuildValue("i", nIsRun);
}

PyObject* appLogoRender(PyObject* poSelf, PyObject* poArgs)
{
	//CPythonApplication::Instance().OnLogoRender();
	return Py_BuildNone();
}

PyObject* appLogoClose(PyObject* poSelf, PyObject* poArgs)
{
	//CPythonApplication::Instance().OnLogoClose();
	return Py_BuildNone();
}
#ifdef ENABLE_WHISPER_FLASHING
PyObject* appFlashApplication(PyObject* poSelf, PyObject* poArgs)
{
	CPythonApplication::Instance().FlashApplication();
	return Py_BuildNone();
}
#endif
#ifdef ENABLE_MULTI_FARM_BLOCK
PyObject* appSetMultiFarmExeIcon(PyObject* poSelf, PyObject* poArgs)
{
	int iStatus;
	if (!PyTuple_GetInteger(poArgs, 0, &iStatus))
		return Py_BuildException();
	CPythonApplication::Instance().MultiFarmBlockIcon(iStatus);
	return Py_BuildNone();
}
#endif

#ifdef ENABLE_DISCORD_STUFF
PyObject* appSetTitle(PyObject* poSelf, PyObject* poArgs)
{
	char* szTitle;
	if (!PyTuple_GetString(poArgs, 0, &szTitle))
		return Py_BuildException();
	char* szMiniTitle;
	if (!PyTuple_GetString(poArgs, 1, &szMiniTitle))
		return Py_BuildException();
	CPythonApplication::Instance().SetTitle(szTitle, szMiniTitle);
	return Py_BuildNone();
}
PyObject* appSetMessage(PyObject* poSelf, PyObject* poArgs)
{
	char* szTitle;
	if (!PyTuple_GetString(poArgs, 0, &szTitle))
		return Py_BuildException();
	char* szMessage;
	if (!PyTuple_GetString(poArgs, 1, &szMessage))
		return Py_BuildException();
	CPythonApplication::Instance().SetMessage(szTitle, szMessage);
	return Py_BuildNone();
}
PyObject* appWindowStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iStatus;
	if (!PyTuple_GetInteger(poArgs, 0, &iStatus))
		return Py_BuildException();
	CPythonApplication::Instance().SetVisibleMode(iStatus == 1 ? true : false);
	return Py_BuildNone();
}

PyObject* appIsVisible(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonApplication::Instance().IsVisible());
}
#endif

#ifdef ENABLE_RENDER_TARGET
PyObject* appIsCanOpenRender(PyObject* poSelf, PyObject* poArgs)
{
	if(CPythonApplication::Instance().IsPressed(DIK_LSHIFT) && CPythonApplication::Instance().IsPressed(DIK_X))
		return Py_BuildValue("i", 1);
	return Py_BuildValue("i", 0);
}
#endif

PyObject* appSetDefaultCodePage(PyObject* poSelf, PyObject* poArgs)
{
	int codePage;
	if (!PyTuple_GetInteger(poArgs, 0, &codePage))
		return Py_BuildException();

	SetDefaultCodePage(codePage);
	return Py_BuildNone();
}

PyObject* appShellExecute(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

#ifdef NEEDED_COMMAND_ARGUMENT
#ifdef UNICODE
	ShellExecuteW(0, "open", szName, NEEDED_COMMAND_ARGUMENT, 0, SW_SHOWNORMAL);
#else
	ShellExecuteA(0, "open", szName, NEEDED_COMMAND_ARGUMENT, 0, SW_SHOWNORMAL);
#endif
#else
#ifdef UNICODE
	ShellExecuteW(0, "open", szName, NULL, 0, SW_SHOWNORMAL);
#else
	ShellExecuteA(0, "open", szName, NULL, 0, SW_SHOWNORMAL);
#endif
#endif

	CPythonApplication::Instance().Exit();
	return Py_BuildNone();
}

#if defined(ENABLE_QUEST_RENEWAL)
PyObject* appGetTextLength(PyObject* poSelf, PyObject* poArgs)
{
	int iLength = 0;

	char* szString;
	if (PyTuple_GetString(poArgs, 0, &szString))
		iLength = MultiByteToWideChar(GetDefaultCodePage(), 0, szString, -1, nullptr, 0);

	return Py_BuildValue("i", iLength);
}

PyObject* appGetTextWidth(PyObject* poSelf, PyObject* poArgs)
{
	char* szString;
	if (!PyTuple_GetString(poArgs, 0, &szString))
		return Py_BuildValue("i", 0);

	const DWORD dwDefaultCodePage = GetDefaultCodePage();
	int iLength = MultiByteToWideChar(dwDefaultCodePage, 0, szString, -1, nullptr, 0);

	wchar_t* wText = (wchar_t*)_alloca(2 * iLength);

	iLength = MultiByteToWideChar(dwDefaultCodePage, 0, szString, -1, wText, iLength);

	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());
	if (!pkDefaultFont)
		return Py_BuildValue("i", 0);

	CGraphicFontTexture* pFont = pkDefaultFont->GetFontTexturePointer();
	if (!pFont)
		return Py_BuildValue("i", 0);

	float fWidth = 0.0f;
	for (int i = 0; i < iLength; ++i)
	{
		if (!wText[i])
			continue;

		CGraphicFontTexture::TCharacterInfomation* pCharacterInfomation = pFont->GetCharacterInfomation(dwDefaultCodePage, wText[i]);
		if (!pCharacterInfomation)
			continue;

		fWidth += pCharacterInfomation->advance;
	}

	return Py_BuildValue("i", (int)ceilf(fWidth));
}
#endif

#if defined(ENABLE_VOICE_CHAT)
PyObject* appVoiceChatSetChatType(PyObject* poSelf, PyObject* poArgs)
{
	uint8_t voiceChatType = 0;
	if (!PyTuple_GetByte(poArgs, 0, &voiceChatType))
		return Py_BuildException();

	VoiceChat::Instance().SetChatType(voiceChatType);
	return Py_BuildValue("i", 0);
}

PyObject* appVoiceChatStartTalking(PyObject* poSelf, PyObject* poArgs)
{
	VoiceChat::Instance().StartTalking();
	return Py_BuildNone();
}

PyObject* appVoiceChatStopTalking(PyObject* poSelf, PyObject* poArgs)
{
	VoiceChat::Instance().StopTalking();
	return Py_BuildNone();
}

PyObject* appVoiceChatSetDisabled(PyObject* poSelf, PyObject* poArgs)
{
	uint8_t disabled = 0;
	if (!PyTuple_GetByte(poArgs, 0, &disabled))
		return Py_BuildException();

	VoiceChat::Instance().SetDisabled(disabled);
	return Py_BuildNone();
}
PyObject* appVoiceChatSetVolumeForSpeaker(PyObject* poSelf, PyObject* poArgs)
{
	char* name;
	if (!PyTuple_GetString(poArgs, 0, &name))
		return Py_BuildException();

	float volume;
	if (!PyTuple_GetFloat(poArgs, 1, &volume))
		return Py_BuildException();

	VoiceChat::Instance().SetSpeakerVolume(name, static_cast<uint8_t>(volume * 100.0f));
	return Py_BuildNone();
}

PyObject* appVoiceChatSetRecorderState(PyObject* poSelf, PyObject* poArgs)
{
	uint8_t state;
	if (!PyTuple_GetInteger(poArgs, 0, &state))
		return Py_BuildException();

	VoiceChat::Instance().SetRecorderState(state);
	return Py_BuildNone();
}

PyObject* appVoiceChatSetVolume(PyObject* poSelf, PyObject* poArgs)
{
	float volume;
	if (!PyTuple_GetFloat(poArgs, 0, &volume))
		return Py_BuildException();

	VoiceChat::Instance().SetVolume(static_cast<uint8_t>(volume * 100.0f));
	return Py_BuildNone();
}
#endif

void initapp()
{
	static PyMethodDef s_methods[] =
	{
		{ "IsDevStage",					appIsDevStage,					METH_VARARGS },
		{ "IsTestStage",				appIsTestStage,					METH_VARARGS },
		{ "IsLiveStage",				appIsLiveStage,					METH_VARARGS },

		// TEXTTAIL_LIVINGTIME_CONTROL
		{ "SetTextTailLivingTime",		appSetTextTailLivingTime,		METH_VARARGS },
		// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

		{ "EnablePerformanceTime",		appEnablePerformanceTime,		METH_VARARGS },
		{ "SetHairColorEnable",			appSetHairColorEnable,			METH_VARARGS },

		{ "SetArmorSpecularEnable",		appSetArmorSpecularEnable,		METH_VARARGS },
		{ "SetWeaponSpecularEnable",	appSetWeaponSpecularEnable,		METH_VARARGS },
		{ "SetSkillEffectUpgradeEnable",appSetSkillEffectUpgradeEnable,	METH_VARARGS },
		{ "SetTwoHandedWeaponAttSpeedDecreaseValue", SetTwoHandedWeaponAttSpeedDecreaseValue, METH_VARARGS },
		{ "SetRideHorseEnable",			appSetRideHorseEnable,			METH_VARARGS },

		{ "SetCameraMaxDistance",		appSetCameraMaxDistance,		METH_VARARGS },
		{ "SetMinFog",					appSetMinFog,					METH_VARARGS },
		{ "SetFrameSkip",				appSetFrameSkip,				METH_VARARGS },
		{ "GetImageInfo",				appGetImageInfo,				METH_VARARGS },
		{ "GetInfo",					appGetInfo,						METH_VARARGS },
		{ "UpdateGame",					appUpdateGame,					METH_VARARGS },
		{ "RenderGame",					appRenderGame,					METH_VARARGS },
		{ "Loop",						appLoop,						METH_VARARGS },
		{ "Create",						appCreate,						METH_VARARGS },
		{ "Process",					appProcess,						METH_VARARGS },
		{ "Exit",						appExit,						METH_VARARGS },
		{ "Abort",						appAbort,						METH_VARARGS },
		{ "SetMouseHandler",			appSetMouseHandler,				METH_VARARGS },
		{ "IsExistFile",				appIsExistFile,					METH_VARARGS },
		{ "GetFileList",				appGetFileList,					METH_VARARGS },

		{ "SetCamera",					appSetCamera,					METH_VARARGS },
		{ "GetCamera",					appGetCamera,					METH_VARARGS },
		{ "GetCameraPitch",				appGetCameraPitch,				METH_VARARGS },
		{ "GetCameraRotation",			appGetCameraRotation,			METH_VARARGS },
		{ "GetTime",					appGetTime,						METH_VARARGS },
		{ "GetGlobalTime",				appGetGlobalTime,				METH_VARARGS },
		{ "GetGlobalTimeStamp",			appGetGlobalTimeStamp,			METH_VARARGS },
		{ "GetUpdateFPS",				appGetUpdateFPS,				METH_VARARGS },
		{ "GetRenderFPS",				appGetRenderFPS,				METH_VARARGS },
		{ "RotateCamera",				appRotateCamera,				METH_VARARGS },
		{ "PitchCamera",				appPitchCamera,					METH_VARARGS },
		{ "ZoomCamera",					appZoomCamera,					METH_VARARGS },
		{ "MovieRotateCamera",			appMovieRotateCamera,			METH_VARARGS },
		{ "MoviePitchCamera",			appMoviePitchCamera,			METH_VARARGS },
		{ "MovieZoomCamera",			appMovieZoomCamera,				METH_VARARGS },
		{ "MovieResetCamera",			appMovieResetCamera,			METH_VARARGS },

		{ "GetAvailableTextureMemory",	appGetAvaiableTextureMememory,	METH_VARARGS },
		{ "GetRenderTime",				appGetRenderTime,				METH_VARARGS },
		{ "GetUpdateTime",				appGetUpdateTime,				METH_VARARGS },
		{ "GetLoad",					appGetLoad,						METH_VARARGS },
		{ "GetFaceSpeed",				appGetFaceSpeed,				METH_VARARGS },
		{ "GetFaceCount",				appGetFaceCount,				METH_VARARGS },
		{ "SetFPS",						appSetFPS,						METH_VARARGS },
		{ "SetGlobalCenterPosition",	appSetGlobalCenterPosition,		METH_VARARGS },
		{ "SetCenterPosition",			appSetCenterPosition,			METH_VARARGS },
		{ "GetCursorPosition",			appGetCursorPosition,			METH_VARARGS },

		{ "GetRandom",					appGetRandom,					METH_VARARGS },
		{ "RunPythonFile",				appRunPythonFile,				METH_VARARGS },
		{ "IsPressed",					appIsPressed,					METH_VARARGS },
		{ "SetCursor",					appSetCursor,					METH_VARARGS },
		{ "GetCursor",					appGetCursor,					METH_VARARGS },
		{ "ShowCursor",					appShowCursor,					METH_VARARGS },
		{ "HideCursor",					appHideCursor,					METH_VARARGS },
		{ "IsShowCursor",				appIsShowCursor,				METH_VARARGS },
		{ "IsLiarCursorOn",				appIsLiarCursorOn,				METH_VARARGS },
		{ "SetSoftwareCursor",			appSetSoftwareCursor,			METH_VARARGS },
		{ "SetHardwareCursor",			appSetHardwareCursor,			METH_VARARGS },

		{ "SetConnectData",				appSetConnectData,				METH_VARARGS },
		{ "GetConnectData",				appGetConnectData,				METH_VARARGS },

		{ "GetRotatingDirection",		appGetRotatingDirection,		METH_VARARGS },
		{ "GetDegreeDifference",		appGetDegreeDifference,			METH_VARARGS },
		{ "Sleep",						appSleep,						METH_VARARGS },
		{ "SetDefaultFontName",			appSetDefaultFontName,			METH_VARARGS },
		{ "SetGuildSymbolPath",			appSetGuildSymbolPath,			METH_VARARGS },

		{ "EnableSpecialCameraMode",	appEnableSpecialCameraMode,		METH_VARARGS },
		{ "SetCameraSpeed",				appSetCameraSpeed,				METH_VARARGS },

		{ "SaveCameraSetting",			appSaveCameraSetting,			METH_VARARGS },
		{ "LoadCameraSetting",			appLoadCameraSetting,			METH_VARARGS },
		{ "SetDefaultCamera",			appSetDefaultCamera,			METH_VARARGS },
		{ "SetCameraSetting",			appSetCameraSetting,			METH_VARARGS },

		{ "SetSightRange",				appSetSightRange,				METH_VARARGS },

		{ "IsFileExist",				appIsFileExist,					METH_VARARGS },
		{ "OpenTextFile",				appOpenTextFile,				METH_VARARGS },
		{ "CloseTextFile",				appCloseTextFile,				METH_VARARGS },
		{ "GetTextFileLineCount",		appGetTextFileLineCount,		METH_VARARGS },
		{ "GetTextFileLine",			appGetTextFileLine,				METH_VARARGS },

		// LOCALE
		{ "GetLocaleServiceName",		appGetLocaleServiceName,		METH_VARARGS },
		{ "GetLocaleName",				appGetLocaleName,				METH_VARARGS },
		{ "GetLocalePath",				appGetLocalePath,				METH_VARARGS },
		{ "ForceSetLocale",				appForceSetLocale,				METH_VARARGS },
		// END_OF_LOCALE

		// CHEONMA
		{ "LoadLocaleAddr",				appLoadLocaleAddr,				METH_VARARGS },
		{ "LoadLocaleData",				appLoadLocaleData,				METH_VARARGS },
		{ "SetCHEONMA",					appSetCHEONMA,					METH_VARARGS },
		{ "IsCHEONMA",					appIsCHEONMA,					METH_VARARGS },
		// END_OF_CHEONMA

		{ "GetDefaultCodePage",			appGetDefaultCodePage,			METH_VARARGS },
		{ "SetControlFP",				appSetControlFP,				METH_VARARGS },
		{ "SetSpecularSpeed",			appSetSpecularSpeed,			METH_VARARGS },

		{ "testGetAccumulationTime",	apptestGetAccumulationTime,		METH_VARARGS },
		{ "testResetAccumulationTime",	apptestResetAccumulationTime,	METH_VARARGS },
		{ "testSetSpecularColor",		apptestSetSpecularColor,		METH_VARARGS },

		{ "SetVisibleNotice",			appSetVisibleNotice,			METH_VARARGS },
		{ "IsVisibleNotice",			appIsVisibleNotice,				METH_VARARGS },
		{ "EnableTestServerFlag",		appEnableTestServerFlag,		METH_VARARGS },
		{ "IsEnableTestServerFlag",		appIsEnableTestServerFlag,		METH_VARARGS },

		{ "SetGuildMarkPath",			appSetGuildMarkPath,			METH_VARARGS },

		{ "OnLogoUpdate",				appLogoUpdate,					METH_VARARGS },
		{ "OnLogoRender",				appLogoRender,					METH_VARARGS },
		{ "OnLogoOpen",					appLogoOpen,					METH_VARARGS },
		{ "OnLogoClose",				appLogoClose,					METH_VARARGS },
		{ "HttpRequest",		appHttpRequest,		METH_VARARGS },

#ifdef ENABLE_WHISPER_FLASHING
		{ "FlashApplication",			appFlashApplication,			METH_VARARGS },
#endif

#ifdef ENABLE_MULTI_FARM_BLOCK
		{ "SetMultiFarmExeIcon", 				appSetMultiFarmExeIcon,					METH_VARARGS },
#endif
#ifdef ENABLE_DISCORD_STUFF
		{ "WindowStatus",					appWindowStatus,			METH_VARARGS },
		{ "IsVisible",					appIsVisible,			METH_VARARGS },
		{ "SetTitle", 					appSetTitle,					METH_VARARGS },		
		{ "SetMessage", 				appSetMessage,					METH_VARARGS },
#endif
#ifdef ENABLE_RENDER_TARGET
		{ "IsCanOpenRender", 				appIsCanOpenRender,					METH_VARARGS },
#endif
		{ "SetDefaultCodePage",			appSetDefaultCodePage,			METH_VARARGS },
		{ "ShellExecute", 				appShellExecute,				METH_VARARGS },
#if defined(ENABLE_QUEST_RENEWAL)
		{ "GetTextLength", appGetTextLength, METH_VARARGS },
		{ "GetTextWidth", appGetTextWidth, METH_VARARGS },
#endif
#if defined(ENABLE_VOICE_CHAT)
		{ "VoiceChatSetDisabled",			appVoiceChatSetDisabled,				METH_VARARGS },
		{ "VoiceChatStartTalking",			appVoiceChatStartTalking,				METH_VARARGS },
		{ "VoiceChatStopTalking",			appVoiceChatStopTalking,				METH_VARARGS },
		{ "VoiceChatSetChatType",			appVoiceChatSetChatType,					METH_VARARGS },
		{ "VoiceChatSetVolumeForSpeaker",	appVoiceChatSetVolumeForSpeaker,		METH_VARARGS },
		{ "VoiceChatSetRecorderState",		appVoiceChatSetRecorderState,				METH_VARARGS },
		{ "VoiceChatSetVolume",				appVoiceChatSetVolume,				METH_VARARGS },
#endif

		{ NULL, NULL },
	};

	PyObject * poModule = Py_InitModule("app", s_methods);

	PyModule_AddIntConstant(poModule, "INFO_ITEM",		CPythonApplication::INFO_ITEM);
	PyModule_AddIntConstant(poModule, "INFO_ACTOR",		CPythonApplication::INFO_ACTOR);
	PyModule_AddIntConstant(poModule, "INFO_EFFECT",	CPythonApplication::INFO_EFFECT);
	PyModule_AddIntConstant(poModule, "INFO_TEXTTAIL",	CPythonApplication::INFO_TEXTTAIL);

	PyModule_AddIntConstant(poModule, "DEGREE_DIRECTION_SAME",		DEGREE_DIRECTION_SAME);
	PyModule_AddIntConstant(poModule, "DEGREE_DIRECTION_RIGHT",		DEGREE_DIRECTION_RIGHT);
	PyModule_AddIntConstant(poModule, "DEGREE_DIRECTION_LEFT",		DEGREE_DIRECTION_LEFT);

	PyModule_AddIntConstant(poModule, "VK_LEFT",	     VK_LEFT);
	PyModule_AddIntConstant(poModule, "VK_RIGHT",	     VK_RIGHT);
	PyModule_AddIntConstant(poModule, "VK_UP",		     VK_UP);
	PyModule_AddIntConstant(poModule, "VK_DOWN",	     VK_DOWN);
	PyModule_AddIntConstant(poModule, "VK_HOME",	     VK_HOME);
	PyModule_AddIntConstant(poModule, "VK_END",          VK_END);
	PyModule_AddIntConstant(poModule, "VK_DELETE",	     VK_DELETE);

	PyModule_AddIntConstant(poModule, "DIK_ESCAPE",      DIK_ESCAPE);
	PyModule_AddIntConstant(poModule, "DIK_ESC",         DIK_ESCAPE);	// 편의를 위해
	PyModule_AddIntConstant(poModule, "DIK_1",           DIK_1);
	PyModule_AddIntConstant(poModule, "DIK_2",           DIK_2);
	PyModule_AddIntConstant(poModule, "DIK_3",           DIK_3);
	PyModule_AddIntConstant(poModule, "DIK_4",           DIK_4);
	PyModule_AddIntConstant(poModule, "DIK_5",           DIK_5);
	PyModule_AddIntConstant(poModule, "DIK_6",           DIK_6);
	PyModule_AddIntConstant(poModule, "DIK_7",           DIK_7);
	PyModule_AddIntConstant(poModule, "DIK_8",           DIK_8);
	PyModule_AddIntConstant(poModule, "DIK_9",           DIK_9);
	PyModule_AddIntConstant(poModule, "DIK_0",           DIK_0);
	PyModule_AddIntConstant(poModule, "DIK_MINUS",       DIK_MINUS);        /* - on main keyboard */
	PyModule_AddIntConstant(poModule, "DIK_EQUALS",      DIK_EQUALS);
	PyModule_AddIntConstant(poModule, "DIK_BACK",        DIK_BACK);           /* backspace */
	PyModule_AddIntConstant(poModule, "DIK_TAB",         DIK_TAB);
	PyModule_AddIntConstant(poModule, "DIK_Q",           DIK_Q);
	PyModule_AddIntConstant(poModule, "DIK_W",           DIK_W);
	PyModule_AddIntConstant(poModule, "DIK_E",           DIK_E);
	PyModule_AddIntConstant(poModule, "DIK_R",           DIK_R);
	PyModule_AddIntConstant(poModule, "DIK_T",           DIK_T);
	PyModule_AddIntConstant(poModule, "DIK_Y",           DIK_Y);
	PyModule_AddIntConstant(poModule, "DIK_U",           DIK_U);
	PyModule_AddIntConstant(poModule, "DIK_I",           DIK_I);
	PyModule_AddIntConstant(poModule, "DIK_O",           DIK_O);
	PyModule_AddIntConstant(poModule, "DIK_P",           DIK_P);
	PyModule_AddIntConstant(poModule, "DIK_LBRACKET",    DIK_LBRACKET);
	PyModule_AddIntConstant(poModule, "DIK_RBRACKET",    DIK_RBRACKET);
	PyModule_AddIntConstant(poModule, "DIK_RETURN",      DIK_RETURN);         /* Enter on main keyboard */
	PyModule_AddIntConstant(poModule, "DIK_LCONTROL",    DIK_LCONTROL);
	PyModule_AddIntConstant(poModule, "DIK_A",           DIK_A);
	PyModule_AddIntConstant(poModule, "DIK_S",           DIK_S);
	PyModule_AddIntConstant(poModule, "DIK_D",           DIK_D);
	PyModule_AddIntConstant(poModule, "DIK_F",           DIK_F);
	PyModule_AddIntConstant(poModule, "DIK_G",           DIK_G);
	PyModule_AddIntConstant(poModule, "DIK_H",           DIK_H);
	PyModule_AddIntConstant(poModule, "DIK_J",           DIK_J);
	PyModule_AddIntConstant(poModule, "DIK_K",           DIK_K);
	PyModule_AddIntConstant(poModule, "DIK_L",           DIK_L);
	PyModule_AddIntConstant(poModule, "DIK_SEMICOLON",   DIK_SEMICOLON);
	PyModule_AddIntConstant(poModule, "DIK_APOSTROPHE",  DIK_APOSTROPHE);
	PyModule_AddIntConstant(poModule, "DIK_GRAVE",       DIK_GRAVE);          /* accent grave */
	PyModule_AddIntConstant(poModule, "DIK_LSHIFT",      DIK_LSHIFT);
	PyModule_AddIntConstant(poModule, "DIK_BACKSLASH",   DIK_BACKSLASH);
	PyModule_AddIntConstant(poModule, "DIK_Z",           DIK_Z);
	PyModule_AddIntConstant(poModule, "DIK_X",           DIK_X);
	PyModule_AddIntConstant(poModule, "DIK_C",           DIK_C);
	PyModule_AddIntConstant(poModule, "DIK_V",           DIK_V);
	PyModule_AddIntConstant(poModule, "DIK_B",           DIK_B);
	PyModule_AddIntConstant(poModule, "DIK_N",           DIK_N);
	PyModule_AddIntConstant(poModule, "DIK_M",           DIK_M);
	PyModule_AddIntConstant(poModule, "DIK_COMMA",       DIK_COMMA);
	PyModule_AddIntConstant(poModule, "DIK_PERIOD",      DIK_PERIOD);         /* . on main keyboard */
	PyModule_AddIntConstant(poModule, "DIK_SLASH",       DIK_SLASH);          /* / on main keyboard */
	PyModule_AddIntConstant(poModule, "DIK_RSHIFT",      DIK_RSHIFT);
	PyModule_AddIntConstant(poModule, "DIK_MULTIPLY",    DIK_MULTIPLY);       /* * on numeric keypad */
	PyModule_AddIntConstant(poModule, "DIK_LALT",        DIK_LMENU);          /* left Alt */
	PyModule_AddIntConstant(poModule, "DIK_SPACE",       DIK_SPACE);
	PyModule_AddIntConstant(poModule, "DIK_CAPITAL",     DIK_CAPITAL);
	PyModule_AddIntConstant(poModule, "DIK_F1",          DIK_F1);
	PyModule_AddIntConstant(poModule, "DIK_F2",          DIK_F2);
	PyModule_AddIntConstant(poModule, "DIK_F3",          DIK_F3);
	PyModule_AddIntConstant(poModule, "DIK_F4",          DIK_F4);
	PyModule_AddIntConstant(poModule, "DIK_F5",          DIK_F5);
	PyModule_AddIntConstant(poModule, "DIK_F6",          DIK_F6);
	PyModule_AddIntConstant(poModule, "DIK_F7",          DIK_F7);
	PyModule_AddIntConstant(poModule, "DIK_F8",          DIK_F8);
	PyModule_AddIntConstant(poModule, "DIK_F9",          DIK_F9);
	PyModule_AddIntConstant(poModule, "DIK_F10",         DIK_F10);
	PyModule_AddIntConstant(poModule, "DIK_NUMLOCK",     DIK_NUMLOCK);
	PyModule_AddIntConstant(poModule, "DIK_SCROLL",      DIK_SCROLL);         /* Scroll Lock */
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD7",     DIK_NUMPAD7);
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD8",     DIK_NUMPAD8);
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD9",     DIK_NUMPAD9);
	PyModule_AddIntConstant(poModule, "DIK_SUBTRACT",    DIK_SUBTRACT);       /* - on numeric keypad */
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD4",     DIK_NUMPAD4);
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD5",     DIK_NUMPAD5);
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD6",     DIK_NUMPAD6);
	PyModule_AddIntConstant(poModule, "DIK_ADD",         DIK_ADD);            /* + on numeric keypad */
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD1",     DIK_NUMPAD1);
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD2",     DIK_NUMPAD2);
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD3",     DIK_NUMPAD3);
	PyModule_AddIntConstant(poModule, "DIK_NUMPAD0",     DIK_NUMPAD0);
	PyModule_AddIntConstant(poModule, "DIK_DECIMAL",     DIK_DECIMAL);        /* . on numeric keypad */
	PyModule_AddIntConstant(poModule, "DIK_F11",         DIK_F11);
	PyModule_AddIntConstant(poModule, "DIK_F12",         DIK_F12);
	PyModule_AddIntConstant(poModule, "DIK_NEXTTRACK",   DIK_NEXTTRACK);      /* Next Track */
	PyModule_AddIntConstant(poModule, "DIK_NUMPADENTER", DIK_NUMPADENTER);    /* Enter on numeric keypad */
	PyModule_AddIntConstant(poModule, "DIK_RCONTROL",    DIK_RCONTROL);
	PyModule_AddIntConstant(poModule, "DIK_MUTE",        DIK_MUTE);           /* Mute */
	PyModule_AddIntConstant(poModule, "DIK_CALCULATOR",  DIK_CALCULATOR);     /* Calculator */
	PyModule_AddIntConstant(poModule, "DIK_PLAYPAUSE",   DIK_PLAYPAUSE);      /* Play / Pause */
	PyModule_AddIntConstant(poModule, "DIK_MEDIASTOP",   DIK_MEDIASTOP);      /* Media Stop */
	PyModule_AddIntConstant(poModule, "DIK_VOLUMEDOWN",  DIK_VOLUMEDOWN);     /* Volume - */
	PyModule_AddIntConstant(poModule, "DIK_VOLUMEUP",    DIK_VOLUMEUP);       /* Volume + */
	PyModule_AddIntConstant(poModule, "DIK_WEBHOME",     DIK_WEBHOME);        /* Web home */
	PyModule_AddIntConstant(poModule, "DIK_NUMPADCOMMA", DIK_NUMPADCOMMA);    /* , on numeric keypad (NEC PC98) */
	PyModule_AddIntConstant(poModule, "DIK_DIVIDE",      DIK_DIVIDE);         /* / on numeric keypad */
	PyModule_AddIntConstant(poModule, "DIK_SYSRQ",       DIK_SYSRQ);
	PyModule_AddIntConstant(poModule, "DIK_RALT",        DIK_RMENU);          /* right Alt */
	PyModule_AddIntConstant(poModule, "DIK_PAUSE",       DIK_PAUSE);          /* Pause */
	PyModule_AddIntConstant(poModule, "DIK_HOME",        DIK_HOME);           /* Home on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_UP",          DIK_UP);             /* UpArrow on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_PGUP",        DIK_PRIOR);          /* PgUp on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_LEFT",        DIK_LEFT);           /* LeftArrow on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_RIGHT",       DIK_RIGHT);          /* RightArrow on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_END",         DIK_END);            /* End on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_DOWN",        DIK_DOWN);           /* DownArrow on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_PGDN",        DIK_NEXT);           /* PgDn on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_INSERT",      DIK_INSERT);         /* Insert on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_DELETE",      DIK_DELETE);         /* Delete on arrow keypad */
	PyModule_AddIntConstant(poModule, "DIK_LWIN",        DIK_LWIN);           /* Left Windows key */
	PyModule_AddIntConstant(poModule, "DIK_RWIN",        DIK_RWIN);           /* Right Windows key */
	PyModule_AddIntConstant(poModule, "DIK_APPS",        DIK_APPS);           /* AppMenu key */

	// Cursor
	PyModule_AddIntConstant(poModule, "NORMAL",			CPythonApplication::CURSOR_SHAPE_NORMAL);
	PyModule_AddIntConstant(poModule, "ATTACK",			CPythonApplication::CURSOR_SHAPE_ATTACK);
	PyModule_AddIntConstant(poModule, "TARGET",			CPythonApplication::CURSOR_SHAPE_TARGET);
	PyModule_AddIntConstant(poModule, "TALK",			CPythonApplication::CURSOR_SHAPE_TALK);
	PyModule_AddIntConstant(poModule, "CANT_GO",		CPythonApplication::CURSOR_SHAPE_CANT_GO);
	PyModule_AddIntConstant(poModule, "PICK",			CPythonApplication::CURSOR_SHAPE_PICK);

	PyModule_AddIntConstant(poModule, "DOOR",			CPythonApplication::CURSOR_SHAPE_DOOR);
	PyModule_AddIntConstant(poModule, "CHAIR",			CPythonApplication::CURSOR_SHAPE_CHAIR);
	PyModule_AddIntConstant(poModule, "MAGIC",			CPythonApplication::CURSOR_SHAPE_MAGIC);
	PyModule_AddIntConstant(poModule, "BUY",			CPythonApplication::CURSOR_SHAPE_BUY);
	PyModule_AddIntConstant(poModule, "SELL",			CPythonApplication::CURSOR_SHAPE_SELL);
#ifdef ENABLE_SWITCHBOT
	PyModule_AddIntConstant(poModule, "ENABLE_SWITCHBOT", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SWITCHBOT", 0);
#endif
	PyModule_AddIntConstant(poModule, "CAMERA_ROTATE",	CPythonApplication::CURSOR_SHAPE_CAMERA_ROTATE);
	PyModule_AddIntConstant(poModule, "HSIZE",			CPythonApplication::CURSOR_SHAPE_HSIZE);
	PyModule_AddIntConstant(poModule, "VSIZE",			CPythonApplication::CURSOR_SHAPE_VSIZE);
	PyModule_AddIntConstant(poModule, "HVSIZE",			CPythonApplication::CURSOR_SHAPE_HVSIZE);
#if defined(ENABLE_FISHING_GAME)
	PyModule_AddIntConstant(poModule, "FISH", CPythonApplication::CURSOR_SHAPE_FISH);
#endif
	PyModule_AddIntConstant(poModule, "CAMERA_TO_POSITIVE",		CPythonApplication::CAMERA_TO_POSITIVE);
	PyModule_AddIntConstant(poModule, "CAMERA_TO_NEGATIVE",		CPythonApplication::CAMERA_TO_NEGITIVE);
	PyModule_AddIntConstant(poModule, "CAMERA_STOP",			CPythonApplication::CAMERA_STOP);
#if defined(ENABLE_VOICE_CHAT)
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_NONE", VOICE_CHAT_TYPE_NONE);
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_LOCAL", VOICE_CHAT_TYPE_LOCAL);
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_PARTY", VOICE_CHAT_TYPE_PARTY);
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_GUILD", VOICE_CHAT_TYPE_GUILD);
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_MAX_NUM", VOICE_CHAT_TYPE_MAX_NUM);
#endif

#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
	PyModule_AddIntConstant(poModule, "ENABLE_AUTOMATIC_HUNTING_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_AUTOMATIC_HUNTING_SYSTEM", 0);
#endif
#ifdef ENABLE_SWITCHBOT_WORLDARD
	PyModule_AddIntConstant(poModule, "ENABLE_SWITCHBOT_WORLDARD", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SWITCHBOT_WORLDARD", 0);
#endif
#ifdef ENABLE_DISCORD_STUFF
	PyModule_AddIntConstant(poModule, "ENABLE_DISCORD_STUFF", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DISCORD_STUFF", 0);
#endif

#ifdef ENABLE_SHOW_CHEST_DROP
	PyModule_AddIntConstant(poModule, "ENABLE_SHOW_CHEST_DROP",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SHOW_CHEST_DROP",	0);
#endif
#ifdef ENABLE_MANAGER_BANK_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_MANAGER_BANK_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MANAGER_BANK_SYSTEM",	0);
#endif
#ifdef ENABLE_REFINE_RENEWAL
	PyModule_AddIntConstant(poModule, "ENABLE_REFINE_RENEWAL",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_REFINE_RENEWAL",	0);
#endif

#ifdef ENABLE_WHISPER_FLASHING
	PyModule_AddIntConstant(poModule, "ENABLE_WHISPER_FLASHING", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_WHISPER_FLASHING", 0);
#endif

#ifdef RACE_HEIGHT
	PyModule_AddIntConstant(poModule, "RACE_HEIGHT", 1);
#else
	PyModule_AddIntConstant(poModule, "RACE_HEIGHT", 0);
#endif
#ifdef ENABLE_HIGHLIGHT_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_HIGHLIGHT_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_HIGHLIGHT_SYSTEM", 0);
#endif
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_MULTILANGUAGE_SYSTEM", 1);
	PyModule_AddIntConstant(poModule, "LOCALE_DEFAULT", CPythonApplication::LOCALE_DEFAULT);
	PyModule_AddIntConstant(poModule, "LOCALE_EN", CPythonApplication::LOCALE_EN);
	PyModule_AddIntConstant(poModule, "LOCALE_DE", CPythonApplication::LOCALE_DE);
	PyModule_AddIntConstant(poModule, "LOCALE_TR", CPythonApplication::LOCALE_TR);
	PyModule_AddIntConstant(poModule, "LOCALE_PT", CPythonApplication::LOCALE_PT);
	PyModule_AddIntConstant(poModule, "LOCALE_ES", CPythonApplication::LOCALE_ES);
	PyModule_AddIntConstant(poModule, "LOCALE_FR", CPythonApplication::LOCALE_FR);
	PyModule_AddIntConstant(poModule, "LOCALE_RO", CPythonApplication::LOCALE_RO);
	PyModule_AddIntConstant(poModule, "LOCALE_PL", CPythonApplication::LOCALE_PL);
	PyModule_AddIntConstant(poModule, "LOCALE_IT", CPythonApplication::LOCALE_IT);
	PyModule_AddIntConstant(poModule, "LOCALE_CZ", CPythonApplication::LOCALE_CZ);
	PyModule_AddIntConstant(poModule, "LOCALE_HU", CPythonApplication::LOCALE_HU);
	PyModule_AddIntConstant(poModule, "LOCALE_MAX_NUM", CPythonApplication::LOCALE_MAX_NUM);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MULTILANGUAGE_SYSTEM", 0);
#endif

#if defined(WJ_COMBAT_ZONE)
	PyModule_AddIntConstant(poModule, "WJ_COMBAT_ZONE", 1);
#else
	PyModule_AddIntConstant(poModule, "WJ_COMBAT_ZONE", 0);
#endif
#ifdef ENABLE_SEND_TARGET_INFO
	PyModule_AddIntConstant(poModule, "ENABLE_SEND_TARGET_INFO",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SEND_TARGET_INFO",	0);
#endif
#ifdef NEW_PET_SYSTEM
	PyModule_AddIntConstant(poModule, "NEW_PET_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "NEW_PET_SYSTEM",	0);
#endif
#ifdef ENABLE_SASH_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_SASH_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SASH_SYSTEM",	0);
#endif

#ifdef ENABLE_ACCUMULATE_DAMAGE_DISPLAY
	PyModule_AddIntConstant(poModule, "ENABLE_ACCUMULATE_DAMAGE_DISPLAY", true);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ACCUMULATE_DAMAGE_DISPLAY", false);
#endif

#ifdef HIDE_OBJECTS
	PyModule_AddIntConstant(poModule, "ENABLE_HIDE_OBJECTS",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_HIDE_OBJECTS",	0);
#endif

#ifdef ENABLE_DROP_ITEM_WORLDARD
	PyModule_AddIntConstant(poModule, "ENABLE_DROP_ITEM_WORLDARD",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DROP_ITEM_WORLDARD",	0);
#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	PyModule_AddIntConstant(poModule, "ENABLE_CUBE_RENEWAL_WORLDARD",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_CUBE_RENEWAL_WORLDARD",	0);
#endif

#ifdef ENABLE_MAINTENANCE_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_MAINTENANCE_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MAINTENANCE_SYSTEM",	0);
#endif
#ifdef ENABLE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_COSTUME_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_COSTUME_SYSTEM",	0);
#endif

#if defined(__BL_BATTLE_ROYALE__)
	PyModule_AddIntConstant(poModule, "__BL_BATTLE_ROYALE__", true);
#else
	PyModule_AddIntConstant(poModule, "__BL_BATTLE_ROYALE__", false);
#endif
#ifdef TOURNAMENT_PVP_SYSTEM
	PyModule_AddIntConstant(poModule, "TOURNAMENT_PVP_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "TOURNAMENT_PVP_SYSTEM",	0);
#endif
#ifdef CHANGE_EQUIP_WORLDARD
	PyModule_AddIntConstant(poModule, "CHANGE_EQUIP_WORLDARD",	1);
#else
	PyModule_AddIntConstant(poModule, "CHANGE_EQUIP_WORLDARD",	0);
#endif

#ifdef ENABLE_HIDE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_HIDE_COSTUME_SYSTEM",	1);
	#ifdef ENABLE_HIDE_COSTUME_SYSTEM_ACCE
		PyModule_AddIntConstant(poModule, "ENABLE_HIDE_COSTUME_SYSTEM_ACCE", 1);
	#else
		PyModule_AddIntConstant(poModule, "ENABLE_HIDE_COSTUME_SYSTEM_ACCE", 0);
	#endif
	#ifdef ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME
		PyModule_AddIntConstant(poModule, "ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME", 1);
	#else
		PyModule_AddIntConstant(poModule, "ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME", 0);
	#endif
#else
	PyModule_AddIntConstant(poModule, "ENABLE_HIDE_COSTUME_SYSTEM",	0);
	PyModule_AddIntConstant(poModule, "ENABLE_HIDE_COSTUME_SYSTEM_ACCE", 0);
	PyModule_AddIntConstant(poModule, "ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME", 0);
#endif
#ifdef ENABLE_AUTOMATIC_PICK_UP_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_AUTOMATIC_PICK_UP_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_AUTOMATIC_PICK_UP_SYSTEM", 0);
#endif


#ifdef ENABLE_DRAGON_SOUL_CHANGE_BONUS_WORLDARD
	PyModule_AddIntConstant(poModule, "ENABLE_DRAGON_SOUL_CHANGE_BONUS_WORLDARD",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DRAGON_SOUL_CHANGE_BONUS_WORLDARD",	0);
#endif
	
#ifdef LINK_IN_CHAT
	PyModule_AddIntConstant(poModule, "LINK_IN_CHAT",	1);
#else
	PyModule_AddIntConstant(poModule, "LINK_IN_CHAT",	0);
#endif

#ifdef ENABLE_BIOLOG_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_BIOLOG_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_BIOLOG_SYSTEM",	0);
#endif
#ifdef ENABLE_ENERGY_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_ENERGY_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ENERGY_SYSTEM",	0);
#endif

#if defined(__BL_67_ATTR__)
	PyModule_AddIntConstant(poModule, "BL_67_ATTR", 1);
#else
	PyModule_AddIntConstant(poModule, "BL_67_ATTR", 0);
#endif

#ifdef ENABLE_DRAGON_SOUL_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_DRAGON_SOUL_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DRAGON_SOUL_SYSTEM",	0);
#endif

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_EQUIPMENT_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_EQUIPMENT_SYSTEM",	0);
#endif

#ifdef ENABLE_SOULBIND_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_SOULBIND_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SOULBIND_SYSTEM",	0);
#endif

#ifdef ENABLE_PLAYER_PER_ACCOUNT5
	PyModule_AddIntConstant(poModule, "ENABLE_PLAYER_PER_ACCOUNT5",	1);
	PyModule_AddIntConstant(poModule, "PLAYER_PER_ACCOUNT",	PLAYER_PER_ACCOUNT5);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_PLAYER_PER_ACCOUNT5",	0);
	PyModule_AddIntConstant(poModule, "PLAYER_PER_ACCOUNT",	PLAYER_PER_ACCOUNT4);
#endif

#ifdef ENABLE_WOLFMAN_CHARACTER
	PyModule_AddIntConstant(poModule, "ENABLE_WOLFMAN_CHARACTER",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_WOLFMAN_CHARACTER",	0);
#endif

#ifdef ENABLE_QUIVER_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_QUIVER_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_QUIVER_SYSTEM",	0);
#endif

#ifdef ENABLE_LEVEL_IN_TRADE
	PyModule_AddIntConstant(poModule, "ENABLE_LEVEL_IN_TRADE",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_LEVEL_IN_TRADE",	0);
#endif

#ifdef ENABLE_678TH_SKILL
	PyModule_AddIntConstant(poModule, "ENABLE_678TH_SKILL",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_678TH_SKILL",	0);
#endif

#ifdef LOGIN_COUNT_DOWN_UI_MODIFY
	PyModule_AddIntConstant(poModule, "LOGIN_COUNT_DOWN_UI_MODIFY",	1);
#else
	PyModule_AddIntConstant(poModule, "LOGIN_COUNT_DOWN_UI_MODIFY",	0);
#endif

#ifdef ENABLE_SPECIAL_STORAGE
	PyModule_AddIntConstant(poModule, "ENABLE_SPECIAL_STORAGE",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SPECIAL_STORAGE",	0);
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_MOUNT_COSTUME_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MOUNT_COSTUME_SYSTEM",	0);
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_ACCE_COSTUME_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ACCE_COSTUME_SYSTEM",	0);
#endif

#ifdef ENABLE_ENVIRONMENT_EFFECT_OPTION
	PyModule_AddIntConstant(poModule, "ENABLE_ENVIRONMENT_EFFECT_OPTION",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ENVIRONMENT_EFFECT_OPTION",	0);
#endif

#ifdef WJ_SHOW_MOB_INFO
	PyModule_AddIntConstant(poModule, "WJ_SHOW_MOB_INFO",	1);
#else
	PyModule_AddIntConstant(poModule, "WJ_SHOW_MOB_INFO",	0);
#endif

#ifdef NEW_SELECT_CHARACTER
	PyModule_AddIntConstant(poModule, "NEW_SELECT_CHARACTER",	1);
#else
	PyModule_AddIntConstant(poModule, "NEW_SELECT_CHARACTER",	0);
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_GROWTH_PET_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_GROWTH_PET_SYSTEM",	0);
#endif

#ifdef ENABLE_AUTO_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_AUTO_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_AUTO_SYSTEM",	0);
#endif

#ifdef ENABLE_MONSTER_CARD
	PyModule_AddIntConstant(poModule, "ENABLE_MONSTER_CARD",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MONSTER_CARD",	0);
#endif

#ifdef ENABLE_HELP_RENEWAL
	PyModule_AddIntConstant(poModule, "ENABLE_HELP_RENEWAL",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_HELP_RENEWAL",	0);
#endif

#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_MAGIC_REDUCTION_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MAGIC_REDUCTION_SYSTEM",	0);
#endif

#ifdef ENABLE_DICE_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_DICE_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DICE_SYSTEM",	0);
#endif

#ifdef ENABLE_CHEQUE_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_CHEQUE_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_CHEQUE_SYSTEM",	0);
#endif

#ifdef ENABLE_EXTEND_INVEN_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_EXTEND_INVEN_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_EXTEND_INVEN_SYSTEM",	0);
#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_WEAPON_COSTUME_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_WEAPON_COSTUME_SYSTEM",	0);
#endif

#ifdef ENABLE_LVL115_ARMOR_EFFECT
	PyModule_AddIntConstant(poModule, "ENABLE_LVL115_ARMOR_EFFECT",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_LVL115_ARMOR_EFFECT",	0);
#endif

#ifdef WJ_ENABLE_TRADABLE_ICON
	PyModule_AddIntConstant(poModule, "WJ_ENABLE_TRADABLE_ICON", 1);
#else
	PyModule_AddIntConstant(poModule, "WJ_ENABLE_TRADABLE_ICON", 0);
#endif

#ifdef ENABLE_CALENDAR_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_CALENDAR_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_CALENDAR_SYSTEM",	0);
#endif

#ifdef ENABLE_DUNGEON_INFO
	PyModule_AddIntConstant(poModule, "ENABLE_DUNGEON_INFO", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DUNGEON_INFO", 0);
#endif
#ifdef VERSION_162_ENABLED
	PyModule_AddIntConstant(poModule, "VERSION_162_ENABLED", 1);
#else
	PyModule_AddIntConstant(poModule, "VERSION_162_ENABLED", 0);
#endif
#ifdef ENABLE_KINGDOMS_WAR
	PyModule_AddIntConstant(poModule, "ENABLE_KINGDOMS_WAR", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_KINGDOMS_WAR", 0);
#endif
#ifdef RENEWAL_DEAD_PACKET
	PyModule_AddIntConstant(poModule, "RENEWAL_DEAD_PACKET",	1);
#else
	PyModule_AddIntConstant(poModule, "RENEWAL_DEAD_PACKET",	0);
#endif

#ifdef ENABLE_DSS_ACTIVE_EFFECT_BUTTON
	PyModule_AddIntConstant(poModule, "ENABLE_DSS_ACTIVE_EFFECT_BUTTON", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DSS_ACTIVE_EFFECT_BUTTON", 0);
#endif
#ifdef ENABLE_YOHARA_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_YOHARA_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_YOHARA_SYSTEM", 0);
#endif
#ifdef NEW_SELECT_CHARACTER
	PyModule_AddIntConstant(poModule, "NEW_SELECT_CHARACTER", 1);
#else
	PyModule_AddIntConstant(poModule, "NEW_SELECT_CHARACTER", 0);
#endif
#ifdef ENABLE_QUEEN_NETHIS
	PyModule_AddIntConstant(poModule, "ENABLE_QUEEN_NETHIS", true);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_QUEEN_NETHIS", false);
#endif
#ifdef ELEMENT_TARGET
	PyModule_AddIntConstant(poModule, "ENABLE_VIEW_ELEMENT", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_VIEW_ELEMENT", 0);
#endif

#ifdef ENABLE_MULTI_TEXTLINE
	PyModule_AddIntConstant(poModule, "ENABLE_MULTI_TEXTLINE", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MULTI_TEXTLINE", 0);
#endif

#ifdef ENABLE_MOUSEWHEEL_EVENT
	PyModule_AddIntConstant(poModule, "ENABLE_MOUSEWHEEL_EVENT", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MOUSEWHEEL_EVENT", 0);
#endif

#ifdef ENABLE_STONEMINIMAP
	PyModule_AddIntConstant(poModule, "ENABLE_STONEMINIMAP", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_STONEMINIMAP", 0);
#endif

#ifdef ENABLE_NEW_NAME_ITEM
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_NAME_ITEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_NAME_ITEM", 0);
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_PET_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_PET_SYSTEM", 0);
#endif
#ifdef NEW_BONUS
	PyModule_AddIntConstant(poModule, "NEW_BONUS", 1);
#else
	PyModule_AddIntConstant(poModule, "NEW_BONUS", 0);
#endif
#ifdef ENABLE_BIYOLOG
	PyModule_AddIntConstant(poModule, "ENABLE_BIYOLOG", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_BIYOLOG", 0);
#endif

#ifdef __CHAT_SETTINGS__
	PyModule_AddIntConstant(poModule, "__CHAT_SETTINGS__", 1);
#else
	PyModule_AddIntConstant(poModule, "__CHAT_SETTINGS__", 0);
#endif
#ifdef ENABLE_ANIMATED_WINDOW
	PyModule_AddIntConstant(poModule, "ENABLE_ANIMATED_WINDOW", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ANIMATED_WINDOW", 0);
#endif
#ifdef DISABLE_OLD_MINUS
	PyModule_AddIntConstant(poModule, "DISABLE_OLD_MINUS", 1);
#else
	PyModule_AddIntConstant(poModule, "DISABLE_OLD_MINUS", 0);
#endif
#ifdef ENABLE_CHANNEL_CHANGE
	PyModule_AddIntConstant(poModule, "ENABLE_CHANNEL_CHANGE", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_CHANNEL_CHANGE", 0);
#endif
#ifdef dracaryS_DUNGEON_LIB
	PyModule_AddIntConstant(poModule, "dracaryS_DUNGEON_LIB", 1);
#else
	PyModule_AddIntConstant(poModule, "dracaryS_DUNGEON_LIB", 0);
#endif
#ifdef ENABLE_DS_GRADE_MYTH
	PyModule_AddIntConstant(poModule, "ENABLE_DS_GRADE_MYTH", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DS_GRADE_MYTH", 0);
#endif
#ifdef ENABLE_EMOTICONS
	PyModule_AddIntConstant(poModule, "ENABLE_EMOTICONS", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_EMOTICONS", 0);
#endif

#ifdef ENABLE_ITEMSHOP
	PyModule_AddIntConstant(poModule, "ENABLE_ITEMSHOP", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ITEMSHOP", 0);
#endif

#ifdef USE_ITEMSHOP_RENEWED
	PyModule_AddIntConstant(poModule, "USE_ITEMSHOP_RENEWED", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_ITEMSHOP_RENEWED", 0);
#endif

#ifdef ENABLE_EVENT_MANAGER
	PyModule_AddIntConstant(poModule, "ENABLE_EVENT_MANAGER", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_EVENT_MANAGER", 0);
#endif

#ifdef ENABLE_DRAGONSOUL_THINGS
	PyModule_AddIntConstant(poModule, "ENABLE_DRAGONSOUL_THINGS", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DRAGONSOUL_THINGS", 0);
#endif
#ifdef ENABLE_AUTO_SELECT_SKILL
	PyModule_AddIntConstant(poModule, "ENABLE_AUTO_SELECT_SKILL", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_AUTO_SELECT_SKILL", 0);
#endif
#ifdef ENABLE_DEFENSAWESHIP
	PyModule_AddIntConstant(poModule, "ENABLE_DEFENSAWESHIP", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DEFENSAWESHIP", 0);
#endif
#ifdef CLOSE_ATTACK_DEFENSE_DECREASE_VALUE
	PyModule_AddIntConstant(poModule, "CLOSE_ATTACK_DEFENSE_DECREASE_VALUE", 1);
#else
	PyModule_AddIntConstant(poModule, "CLOSE_ATTACK_DEFENSE_DECREASE_VALUE", 0);
#endif
#ifdef ENABLE_NEW_GAMEOPTION
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_GAMEOPTION", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_GAMEOPTION", 0);
#endif
#ifdef ENABLE_NEW_DETAILS_GUI
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_DETAILS_GUI", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_DETAILS_GUI", 0);
#endif
#ifdef ENABLE_SPECIAL_COSTUME_ATTR
	PyModule_AddIntConstant(poModule, "ENABLE_SPECIAL_COSTUME_ATTR", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SPECIAL_COSTUME_ATTR", 0);
#endif
#ifdef ENABLE_ANTI_EXP
	PyModule_AddIntConstant(poModule, "ENABLE_ANTI_EXP", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ANTI_EXP", 0);
#endif
#ifdef ENABLE_MULTI_FARM_BLOCK
	PyModule_AddIntConstant(poModule, "ENABLE_MULTI_FARM_BLOCK", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MULTI_FARM_BLOCK", 0);
#endif
#ifdef ENABLE_VERSION_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_VERSION_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_VERSION_SYSTEM", 0);
#endif
#ifdef ENABLE_WIKI
	PyModule_AddIntConstant(poModule, "ENABLE_WIKI", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_WIKI", 0);
#endif
#ifdef ENABLE_RENDER_TARGET
	PyModule_AddIntConstant(poModule, "ENABLE_RENDER_TARGET", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_RENDER_TARGET", 0);
#endif
#ifdef ENABLE_MOUNT_SKIN
	PyModule_AddIntConstant(poModule, "ENABLE_MOUNT_SKIN", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MOUNT_SKIN", 0);
#endif
#ifdef ENABLE_ZODIAC_MISSION
	PyModule_AddIntConstant(poModule, "ENABLE_ZODIAC_MISSION", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ZODIAC_MISSION", 0);
#endif
#ifdef ENABLE_MELEY_LAIR_DUNGEON
	PyModule_AddIntConstant(poModule, "ENABLE_MELEY_LAIR_DUNGEON", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MELEY_LAIR_DUNGEON", 0);
#endif
#ifdef ENABLE_ULTIMATE_REGEN
	PyModule_AddIntConstant(poModule, "ENABLE_ULTIMATE_REGEN", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ULTIMATE_REGEN", 0);
#endif

#ifdef ENABLE_TRACK_WINDOW
	PyModule_AddIntConstant(poModule, "ENABLE_TRACK_WINDOW", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_TRACK_WINDOW", 0);
#endif
#ifdef ENABLE_FISH_GAME
	PyModule_AddIntConstant(poModule, "ENABLE_FISH_GAME", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_FISH_GAME", 0);
#endif
#ifdef ENABLE_EXCHANGE_LOG
	PyModule_AddIntConstant(poModule, "ENABLE_EXCHANGE_LOG", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_EXCHANGE_LOG", 0);
#endif
#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
	PyModule_AddIntConstant(poModule, "ENABLE_MAP_OBJECT_OPTIMIZATION", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MAP_OBJECT_OPTIMIZATION", 0);
#endif

#ifdef ENABLE_GEM_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_GEM_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_GEM_SYSTEM", 0);
#endif

#ifdef ENABLE_VOTE4BUFF
	PyModule_AddIntConstant(poModule, "ENABLE_VOTE4BUFF", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_VOTE4BUFF", 0);
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
	PyModule_AddIntConstant(poModule, "ENABLE_MINI_GAME_CATCH_KING", true);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MINI_GAME_CATCH_KING", false);
#endif
#ifdef RENEWAL_MISSION_BOOKS
	PyModule_AddIntConstant(poModule, "RENEWAL_MISSION_BOOKS", 1);
#else
	PyModule_AddIntConstant(poModule, "RENEWAL_MISSION_BOOKS", 0);
#endif
#ifdef ENABLE_COPY_ATTR_ITEM
	PyModule_AddIntConstant(poModule, "ENABLE_COPY_ATTR_ITEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_COPY_ATTR_ITEM", 0);
#endif
#ifdef ENABLE_LOCK_ATTR
	PyModule_AddIntConstant(poModule, "ENABLE_LOCK_ATTR", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_LOCK_ATTR", 0);
#endif

#ifdef __RENEWAL_BRAVE_CAPE__
	PyModule_AddIntConstant(poModule, "__RENEWAL_BRAVE_CAPE__", 1);
#else
	PyModule_AddIntConstant(poModule, "__RENEWAL_BRAVE_CAPE__", 0);
#endif

#ifdef ENABLE_SELL_ITEM
	PyModule_AddIntConstant(poModule, "ENABLE_SELL_ITEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SELL_ITEM", 0);
#endif
#ifdef ENABLE_VIEW_TARGET_PLAYER_HP
	PyModule_AddIntConstant(poModule, "ENABLE_VIEW_TARGET_PLAYER_HP", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_VIEW_TARGET_PLAYER_HP", 0);
#endif

#ifdef ENABLE_VIEW_TARGET_DECIMAL_HP
	PyModule_AddIntConstant(poModule, "ENABLE_VIEW_TARGET_DECIMAL_HP", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_VIEW_TARGET_DECIMAL_HP", 0);
#endif
#ifdef ENABLE_RANKING
	PyModule_AddIntConstant(poModule, "ENABLE_RANKING", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_RANKING", 0);
#endif

#ifdef __BUFFI_SUPPORT__
	PyModule_AddIntConstant(poModule, "__BUFFI_SUPPORT__", 1);
#else
	PyModule_AddIntConstant(poModule, "__BUFFI_SUPPORT__", 0);
#endif

#ifdef __RENEWAL_NOTICE__
	PyModule_AddIntConstant(poModule, "__RENEWAL_NOTICE__", 1);
#else
	PyModule_AddIntConstant(poModule, "__RENEWAL_NOTICE__", 0);
#endif

#ifdef ENABLE_DS_SET_RENEWAL
	PyModule_AddIntConstant(poModule, "ENABLE_DS_SET_RENEWAL", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_DS_SET_RENEWAL", 0);
#endif

#ifdef ENABLE_REWARD_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_REWARD_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_REWARD_SYSTEM", 0);
#endif

#define AUTO_SHOUT
#if defined(AUTO_SHOUT)
	PyModule_AddIntConstant(poModule, "AUTO_SHOUT", TRUE);
#else
	PyModule_AddIntConstant(poModule, "AUTO_SHOUT", FALSE);
#endif
#ifdef ENABLE_AURA_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_AURA_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_AURA_SYSTEM", 0);
#endif

#ifdef ENABLE_NEW_EXCHANGE_WINDOW
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_EXCHANGE_WINDOW", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_EXCHANGE_WINDOW", 0);
#endif

#ifdef __DUNGEON_INFO__
	PyModule_AddIntConstant(poModule, "__DUNGEON_INFO__", 1);
#else
	PyModule_AddIntConstant(poModule, "__DUNGEON_INFO__", 0);
#endif

#ifdef __AUTO_SKILL_READER__
	PyModule_AddIntConstant(poModule, "__AUTO_SKILL_READER__", 1);
#else
	PyModule_AddIntConstant(poModule, "__AUTO_SKILL_READER__", 0);
#endif

#ifdef ENABLE_AFFECT_BUFF_REMOVE
	PyModule_AddIntConstant(poModule, "ENABLE_AFFECT_BUFF_REMOVE", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_AFFECT_BUFF_REMOVE", 0);
#endif

#ifdef ENABLE_PERFECT_BUFF
	PyModule_AddIntConstant(poModule, "ENABLE_PERFECT_BUFF",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_PERFECT_BUFF",	0);
#endif
#ifdef __AUTO_HUNT__
	PyModule_AddIntConstant(poModule, "__AUTO_HUNT__", 1);
#else
	PyModule_AddIntConstant(poModule, "__AUTO_HUNT__", 0);
#endif
#ifdef __ROULETTE__
	PyModule_AddIntConstant(poModule, "__ROULETTE__", 1);
#else
	PyModule_AddIntConstant(poModule, "__ROULETTE__", 0);
#endif
#ifdef __SPIN_WHEEL__
	PyModule_AddIntConstant(poModule, "__SPIN_WHEEL__", 1);
#else
	PyModule_AddIntConstant(poModule, "__SPIN_WHEEL__", 0);
#endif

#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef USE_IKASHOP_UI_RENEWED
	PyModule_AddIntConstant(poModule, "USE_IKASHOP_UI_RENEWED", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_IKASHOP_UI_RENEWED", 0);
#endif

#ifdef EXTEND_IKASHOP_PRO
	PyModule_AddIntConstant(poModule, "EXTEND_IKASHOP_PRO", 1);
#else
	PyModule_AddIntConstant(poModule, "EXTEND_IKASHOP_PRO", 0);
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
	PyModule_AddIntConstant(poModule, "EXTEND_IKASHOP_ULTIMATE", 1);
#else
	PyModule_AddIntConstant(poModule, "EXTEND_IKASHOP_ULTIMATE", 0);
#endif
	PyModule_AddIntConstant(poModule, "ENABLE_IKASHOP_RENEWAL", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_IKASHOP_RENEWAL", 0);
#endif
#ifdef __AUTO_QUQUE_ATTACK__
	PyModule_AddIntConstant(poModule, "__AUTO_QUQUE_ATTACK__", 1);
#else
	PyModule_AddIntConstant(poModule, "__AUTO_QUQUE_ATTACK__", 0);
#endif
#ifdef VOTE_BUFF_RENEWAL
	PyModule_AddIntConstant(poModule, "VOTE_BUFF_RENEWAL", 1);
#else
	PyModule_AddIntConstant(poModule, "VOTE_BUFF_RENEWAL", 0);
#endif

#ifdef NAMECOLOR_BOSS_CLIENT
    PyModule_AddIntConstant(poModule, "NAMECOLOR_BOSS_CLIENT", 1);
#else
    PyModule_AddIntConstant(poModule, "NAMECOLOR_BOSS_CLIENT", 0);
#endif

#ifdef USE_EXPANDED_ITEM_REFRESH
    PyModule_AddIntConstant(poModule, "USE_EXPANDED_ITEM_REFRESH", 1);
#else
    PyModule_AddIntConstant(poModule, "USE_EXPANDED_ITEM_REFRESH", 0);
#endif

#ifdef USE_CAPTCHA_SYSTEM
    PyModule_AddIntConstant(poModule, "USE_CAPTCHA_SYSTEM", 1);
#else
    PyModule_AddIntConstant(poModule, "USE_CAPTCHA_SYSTEM", 0);
#endif

#ifdef ITEM_TALISMAN_EQUIPMENT
    PyModule_AddIntConstant(poModule, "ITEM_TALISMAN_EQUIPMENT", 1);
#else
    PyModule_AddIntConstant(poModule, "ITEM_TALISMAN_EQUIPMENT", 0);
#endif

#ifdef ENABLE_SASH_COSTUME_SYSTEM
    PyModule_AddIntConstant(poModule, "ENABLE_SASH_COSTUME_SYSTEM", 1);
#else
    PyModule_AddIntConstant(poModule, "ENABLE_SASH_COSTUME_SYSTEM", 0);
#endif

#ifdef USE_CLIP_MASK
    PyModule_AddIntConstant(poModule, "USE_CLIP_MASK", 1);
#else
    PyModule_AddIntConstant(poModule, "USE_CLIP_MASK", 0);
#endif

#ifdef USE_OFFLINESHOP_SEARCH_V2
    PyModule_AddIntConstant(poModule, "USE_OFFLINESHOP_SEARCH_V2", 1);
#else
    PyModule_AddIntConstant(poModule, "USE_OFFLINESHOP_SEARCH_V2", 0);
#endif

#ifdef USE_OFFLINESHOP_FIXES
    PyModule_AddIntConstant(poModule, "USE_OFFLINESHOP_FIXES", 1);
#else
    PyModule_AddIntConstant(poModule, "USE_OFFLINESHOP_FIXES", 0);
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
	PyModule_AddIntConstant(poModule, "ENABLE_SUNG_MAHI_TOWER",   1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SUNG_MAHI_TOWER",   0);
#endif
#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
	PyModule_AddIntConstant(poModule, "ENABLE_BADGE_NOTIFICATION_MANAGER", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_BADGE_NOTIFICATION_MANAGER", 0);
#endif
#if defined(ENABLE_FISHING_GAME)
	PyModule_AddIntConstant(poModule, "ENABLE_FISHING_GAME", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_FISHING_GAME", 0);
#endif
#ifdef ENABLE_MESSENGER_BLOCK
	PyModule_AddIntConstant(poModule, "ENABLE_MESSENGER_BLOCK", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_MESSENGER_BLOCK", 0);
#endif
#if defined(__BL_HYPERLINK_ITEM_ICON__)
	PyModule_AddIntConstant(poModule, "__BL_HYPERLINK_ITEM_ICON__", true);
#else
	PyModule_AddIntConstant(poModule, "__BL_HYPERLINK_ITEM_ICON__", false);
#endif
#if defined(ENABLE_GAME_OPTION_ESCAPE)
	PyModule_AddIntConstant(poModule, "ENABLE_GAME_OPTION_ESCAPE", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_GAME_OPTION_ESCAPE", 0);
#endif

#ifdef USE_CHEQUE_CURRENCY
	PyModule_AddIntConstant(poModule, "USE_CHEQUE_CURRENCY", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_CHEQUE_CURRENCY", 0);
#endif

#ifdef USE_CURRENCY_EXCHANGE
	PyModule_AddIntConstant(poModule, "USE_CURRENCY_EXCHANGE", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_CURRENCY_EXCHANGE", 0);
#endif

#ifdef ENABLE_BUY_WITH_ITEM
	PyModule_AddIntConstant(poModule, "ENABLE_BUY_WITH_ITEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_BUY_WITH_ITEM", 0);
#endif

#ifdef ENABLE_NEW_TARGET_UI
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_TARGET_UI", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_NEW_TARGET_UI", 0);
#endif

#ifdef USE_BOOST_POTIONS
	PyModule_AddIntConstant(poModule, "USE_BOOST_POTIONS", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_BOOST_POTIONS", 0);
#endif

#ifdef __NEW_SET_BONUS__
	PyModule_AddIntConstant(poModule, "__NEW_SET_BONUS__", 1);
#else
	PyModule_AddIntConstant(poModule, "__NEW_SET_BONUS__", 0);
#endif
#ifdef ENABLE_QUEST_PAGE_SCROLLING
    PyModule_AddIntConstant(poModule, "ENABLE_QUEST_PAGE_SCROLLING", 1);
#else
    PyModule_AddIntConstant(poModule, "ENABLE_QUEST_PAGE_SCROLLING", 0);
#endif
#ifdef ENABLE_BATTLEPASS
	PyModule_AddIntConstant(poModule, "ENABLE_BATTLEPASS", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_BATTLEPASS", 0);
#endif

#if defined(ENABLE_QUEST_RENEWAL)
	PyModule_AddIntConstant(poModule, "ENABLE_QUEST_RENEWAL", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_QUEST_RENEWAL", 0);
#endif
#ifdef ENABLE_ACCE_ABSORBED_ITEM_PREVIEW
    PyModule_AddIntConstant(poModule, "ENABLE_ACCE_ABSORBED_ITEM_PREVIEW", true);
#else
    PyModule_AddIntConstant(poModule, "ENABLE_ACCE_ABSORBED_ITEM_PREVIEW", false);
#endif
#ifdef ENABLE_WHISPER_CLOSE_SHORTCUT
    PyModule_AddIntConstant(poModule, "ENABLE_WHISPER_CLOSE_SHORTCUT", true);
#else
    PyModule_AddIntConstant(poModule, "ENABLE_WHISPER_CLOSE_SHORTCUT", false);
#endif
#if defined(ENABLE_VOICE_CHAT)
PyModule_AddIntConstant(poModule, "ENABLE_VOICE_CHAT", 1);
#else
PyModule_AddIntConstant(poModule, "ENABLE_VOICE_CHAT", 0);
#endif

#ifdef USE_PASSIVE_ABILITY_TAMER
	PyModule_AddIntConstant(poModule, "USE_PASSIVE_ABILITY_TAMER", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_PASSIVE_ABILITY_TAMER", 0);
#endif

#ifdef ENABLE_PITTY_REFINE
	PyModule_AddIntConstant(poModule, "ENABLE_PITTY_REFINE", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_PITTY_REFINE", 0);
#endif

#ifdef USE_AURA_SYSTEM_RENEWED
	PyModule_AddIntConstant(poModule, "USE_AURA_SYSTEM_RENEWED", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_AURA_SYSTEM_RENEWED", 0);
#endif

#ifdef USE_CRYSTAL_SYSTEM
	PyModule_AddIntConstant(poModule, "USE_CRYSTAL_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_CRYSTAL_SYSTEM", 0);
#endif

#ifdef USE_DROP_AFFECT_BONUSES
	PyModule_AddIntConstant(poModule, "USE_DROP_AFFECT_BONUSES", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_DROP_AFFECT_BONUSES", 0);
#endif

#ifdef USE_PREMIUM_AFFECT
	PyModule_AddIntConstant(poModule, "USE_PREMIUM_AFFECT", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_PREMIUM_AFFECT", 0);
#endif

#ifdef USE_AUTOMATIC_HUNTING_RENEWED
	PyModule_AddIntConstant(poModule, "USE_AUTOMATIC_HUNTING_RENEWED", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_AUTOMATIC_HUNTING_RENEWED", 0);
#endif

#ifdef USE_BL_67_ATTR_RENEWED
	PyModule_AddIntConstant(poModule, "USE_BL_67_ATTR_RENEWED", 1);
#else
	PyModule_AddIntConstant(poModule, "USE_BL_67_ATTR_RENEWED", 0);
#endif
}
