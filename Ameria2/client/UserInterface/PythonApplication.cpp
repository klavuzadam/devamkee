#include "StdAfx.h"
#include "../eterBase/Error.h"
#include "../eterlib/Camera.h"
#include "../eterlib/AttributeInstance.h"
#include "../gamelib/AreaTerrain.h"
#include "../EterGrnLib/Material.h"

#ifdef ENABLE_SWITCHBOT
#include "PythonSwitchbot.h"
#endif

#include "resource.h"
#include "PythonApplication.h"
#include "PythonCharacterManager.h"

//#include "ProcessScanner.h"

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
#include "CShield.h"
#include "CShieldCheckIntegrity.h"
#endif
#ifdef USE_CAPTCHA_SYSTEM
#define cimg_display 0
#include <CImg.h>
#endif

extern void GrannyCreateSharedDeformBuffer();
extern void GrannyDestroySharedDeformBuffer();

float MIN_FOG = 2400.0f;
double g_specularSpd=0.007f;

CPythonApplication * CPythonApplication::ms_pInstance;

float c_fDefaultCameraRotateSpeed = 1.5f;
float c_fDefaultCameraPitchSpeed = 1.5f;
float c_fDefaultCameraZoomSpeed = 0.05f;



CPythonApplication::CPythonApplication() :
m_bCursorVisible(TRUE),
m_bLiarCursorOn(false),
m_iCursorMode(CURSOR_MODE_HARDWARE),
m_isWindowed(false),
m_isFrameSkipDisable(false),
m_poMouseHandler(NULL),
m_dwUpdateFPS(0),
m_dwRenderFPS(0),
m_fAveRenderTime(0.0f),
m_dwFaceCount(0),
m_fGlobalTime(0.0f),
m_fGlobalElapsedTime(0.0f),
m_dwLButtonDownTime(0),
m_dwLastIdleTime(0)
{
#ifndef _DEBUG
	SetEterExceptionHandler();
#endif

#ifdef USE_CAPTCHA_SYSTEM
    _mkdir("UserData");
#endif

	CTimer::Instance().UseCustomTime();
	m_dwWidth = 800;
	m_dwHeight = 600;
#ifdef ENABLE_DISCORD_STUFF
	m_bFarmIsBlocked = false;
	oldIcon = NULL;
	ZeroMemory(&NID, sizeof(NOTIFYICONDATA));
#endif
	ms_pInstance = this;
	m_isWindowFullScreenEnable = FALSE;

	m_v3CenterPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_dwStartLocalTime = ELTimer_GetMSec();
	m_tServerTime = 0;
	m_tLocalStartTime = 0;

	m_iPort = 0;
	m_iFPS = 60;

	m_isActivateWnd = false;
	m_isMinimizedWnd = true;

	m_fRotationSpeed = 0.0f;
	m_fPitchSpeed = 0.0f;
	m_fZoomSpeed = 0.0f;

	m_fFaceSpd=0.0f;

	m_dwFaceAccCount=0;
	m_dwFaceAccTime=0;

	m_dwFaceSpdSum=0;
	m_dwFaceSpdCount=0;

	m_FlyingManager.SetMapManagerPtr(&m_pyBackground);

	m_iCursorNum = CURSOR_SHAPE_NORMAL;
	m_iContinuousCursorNum = CURSOR_SHAPE_NORMAL;

	m_isSpecialCameraMode = FALSE;
	m_fCameraRotateSpeed = c_fDefaultCameraRotateSpeed;
	m_fCameraPitchSpeed = c_fDefaultCameraPitchSpeed;
	m_fCameraZoomSpeed = c_fDefaultCameraZoomSpeed;

	m_iCameraMode = CAMERA_MODE_NORMAL;
	m_fBlendCameraStartTime = 0.0f;
	m_fBlendCameraBlendTime = 0.0f;

	m_iForceSightRange = -1;

	CCameraManager::Instance().AddCamera(EVENT_CAMERA_NUMBER);
}

CPythonApplication::~CPythonApplication()
{
}

void CPythonApplication::GetMousePosition(POINT* ppt)
{
	CMSApplication::GetMousePosition(ppt);
}

void CPythonApplication::SetMinFog(float fMinFog)
{
	MIN_FOG = fMinFog;
}

void CPythonApplication::SetFrameSkip(bool isEnable)
{
	if (isEnable)
		m_isFrameSkipDisable=false;
	else
		m_isFrameSkipDisable=true;
}

void CPythonApplication::NotifyHack(const char* c_szFormat, ...)
{
	char szBuf[1024];

	va_list args;
	va_start(args, c_szFormat);
	_vsnprintf(szBuf, sizeof(szBuf), c_szFormat, args);
	va_end(args);
	m_pyNetworkStream.NotifyHack(szBuf);
}

void CPythonApplication::GetInfo(UINT eInfo, std::string* pstInfo)
{
	switch (eInfo)
	{
	case INFO_ACTOR:
		m_kChrMgr.GetInfo(pstInfo);
		break;
	case INFO_EFFECT:
		m_kEftMgr.GetInfo(pstInfo);
		break;
	case INFO_ITEM:
		m_pyItem.GetInfo(pstInfo);
		break;
	case INFO_TEXTTAIL:
		m_pyTextTail.GetInfo(pstInfo);
		break;
	}
}

void CPythonApplication::Abort()
{
	TraceError("============================================================================================================");
	TraceError("Abort!!!!\n\n");

	PostQuitMessage(0);
}

void CPythonApplication::Exit()
{
	PostQuitMessage(0);
}

bool PERF_CHECKER_RENDER_GAME = false;

void CPythonApplication::RenderGame()
{
    m_kRenderTargetManager.RenderBackgrounds();

    const auto fAspect = m_kWndMgr.GetAspect();
    const auto fFarClip = m_pyBackground.GetFarClip();
    const auto fFOV = CPythonSystem::Instance().GetFOVLevel();
    m_pyGraphic.SetPerspective(fFOV, fAspect, 100.0, fFarClip);

    CCullingManager::Instance().Process();

    m_kChrMgr.Deform();

    m_kRenderTargetManager.DeformModels();

    CMapOutdoor *pMap = nullptr;
    if (m_pyBackground.IsMapReady())
    {
        pMap = &m_pyBackground.GetMapOutdoorRef();
    }

    if (pMap)
    {
        m_pyBackground.RenderCharacterShadowToTexture();
    }

    m_pyGraphic.SetGameRenderState();
    m_pyGraphic.PushState();

    {
        long lx, ly;
        m_kWndMgr.GetMousePosition(lx, ly);
        m_pyGraphic.SetCursorPosition(lx, ly);
    }

    if (pMap)
    {
        pMap->RenderSky();
        pMap->RenderBeforeLensFlare();
        pMap->RenderCloud();

        m_pyBackground.BeginEnvironment();
        m_pyBackground.Render();

        m_pyBackground.SetCharacterDirLight();
    }

    m_kChrMgr.Render();

    m_kRenderTargetManager.RenderModels();

    if (pMap)
    {
        m_pyBackground.SetBackgroundDirLight();
        pMap->RenderWater();
        m_pyBackground.RenderSnow();
        pMap->RenderEffect();

        m_pyBackground.EndEnvironment();
    }

    m_kEftMgr.Render();
    m_pyItem.Render();

#ifdef ENABLE_IKASHOP_RENEWAL
    m_pyOfflineshop.RenderEntities();
#endif

    m_FlyingManager.Render();

    if (pMap)
    {
        m_pyBackground.BeginEnvironment();
        pMap->RenderPCBlocker();
        m_pyBackground.EndEnvironment();

        pMap->RenderAfterLensFlare();
        pMap->RenderScreenFiltering();
    }
}

void CPythonApplication::UpdateGame()
{
    POINT ptMouse;
    GetMousePosition(&ptMouse);

    CGraphicTextInstance::Hyperlink_UpdateMousePos(ptMouse.x, ptMouse.y);

    //if (m_isActivateWnd)
    {
        CScreen s;
        float fAspect = UI::CWindowManager::Instance().GetAspect();
        float fFarClip = CPythonBackground::Instance().GetFarClip();

        float fFOV = CPythonSystem::Instance().GetFOVLevel();
        s.SetPerspective(fFOV,fAspect, 100.0f, fFarClip);
        s.BuildViewFrustum();
    }

    TPixelPosition kPPosMainActor;
    m_pyPlayer.NEW_GetMainActorPosition(&kPPosMainActor);

    m_kChrMgr.Update();

#ifdef ENABLE_IKASHOP_RENEWAL
    m_pyOfflineshop.UpdateEntities();
#endif

    m_kRenderTargetManager.UpdateModels();

    m_pyBackground.Update(kPPosMainActor.x, kPPosMainActor.y, kPPosMainActor.z);

    m_GameEventManager.SetCenterPosition(kPPosMainActor.x, kPPosMainActor.y, kPPosMainActor.z);
    m_GameEventManager.Update();

    m_kEftMgr.Update();

    m_FlyingManager.Update();

    m_pyItem.Update(ptMouse);

    m_pyPlayer.Update();

    m_pyPlayer.NEW_GetMainActorPosition(&kPPosMainActor);
    SetCenterPosition(kPPosMainActor.x, kPPosMainActor.y, kPPosMainActor.z);
	
#if defined(__BL_BATTLE_ROYALE__)
	PythonBattleRoyaleManager::Instance().Update();
#endif
}

void CPythonApplication::SkipRenderBuffering(DWORD dwSleepMSec)
{
	m_dwBufSleepSkipTime=ELTimer_GetMSec()+dwSleepMSec;
}

bool CPythonApplication::IsMinimizedWnd()
{
	return m_isMinimizedWnd;
}

void CPythonApplication::RenderSpecial() 
{
	m_kEftMgr.Update();
	m_kChrMgr.Render();
	m_pyItem.Render();
	m_FlyingManager.Render();
}

bool CPythonApplication::Process()
{
	ELTimer_SetFrameMSec();

	// 	m_Profiler.Clear();
	DWORD dwStart = ELTimer_GetMSec();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	static DWORD	s_dwUpdateFrameCount = 0;
	static DWORD	s_dwRenderFrameCount = 0;
	static DWORD	s_dwFaceCount = 0;
	static UINT		s_uiLoad = 0;
	static DWORD	s_dwCheckTime = ELTimer_GetMSec();

	if (ELTimer_GetMSec() - s_dwCheckTime > 1000)
	{
		m_dwUpdateFPS		= s_dwUpdateFrameCount;
		m_dwRenderFPS		= s_dwRenderFrameCount;
		m_dwLoad			= s_uiLoad;

		m_dwFaceCount		= s_dwFaceCount / max(1, s_dwRenderFrameCount);

		s_dwCheckTime		= ELTimer_GetMSec();

		s_uiLoad = s_dwFaceCount = s_dwUpdateFrameCount = s_dwRenderFrameCount = 0;
	}
	
#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
/*	static DWORD time = ELTimer_GetMSec();
	if (ELTimer_GetMSec() - time > 10000)
	{
		if (!ValidateCShield())
		{
#ifdef ENABLE_CSHIELD_DEBUG
			ErrorMessage("Couldnt validate CShield");
#endif
			Close();
		}
		time = ELTimer_GetMSec();
	}
	*/
#endif

	// Update Time
	static BOOL s_bFrameSkip = false;
	static UINT s_uiNextFrameTime = ELTimer_GetMSec();

#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime1=ELTimer_GetMSec();
#endif
	CTimer& rkTimer=CTimer::Instance();
	rkTimer.Advance();

	m_fGlobalTime = rkTimer.GetCurrentSecond();
	m_fGlobalElapsedTime = rkTimer.GetElapsedSecond();

	UINT uiFrameTime = rkTimer.GetElapsedMilliecond();
	s_uiNextFrameTime += uiFrameTime;	//17 - 1초당 60fps기준.

	DWORD updatestart = ELTimer_GetMSec();
#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime2=ELTimer_GetMSec();
#endif
	// Network I/O
	m_pyNetworkStream.Process();
	//m_pyNetworkDatagram.Process();

	m_kGuildMarkUploader.Process();

	m_kGuildMarkDownloader.Process();
	m_kAccountConnector.Process();
#if defined(ENABLE_VOICE_CHAT)
	m_VoiceChat.Process();
#endif
#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime3=ELTimer_GetMSec();
#endif
	//////////////////////
	// Input Process
	// Keyboard
	UpdateKeyboard();
#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime4=ELTimer_GetMSec();
#endif
	// Mouse
	POINT Point;
	if (GetCursorPos(&Point))
	{
		ScreenToClient(m_hWnd, &Point);
		OnMouseMove(Point.x, Point.y);
	}
	//////////////////////
#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime5=ELTimer_GetMSec();
#endif
	//!@# Alt+Tab 중 SetTransfor 에서 튕김 현상 해결을 위해 - [levites]
	//if (m_isActivateWnd)
	__UpdateCamera();
#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime6=ELTimer_GetMSec();
#endif
	// Update Game Playing
	CResourceManager::Instance().Update();
#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime7=ELTimer_GetMSec();
#endif
	OnCameraUpdate();
#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime8=ELTimer_GetMSec();
#endif
	OnMouseUpdate();
#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime9=ELTimer_GetMSec();
#endif
	OnUIUpdate();

#ifdef __PERFORMANCE_CHECK__
	DWORD dwUpdateTime10=ELTimer_GetMSec();

	if (dwUpdateTime10-dwUpdateTime1>10)
	{
		static FILE* fp=fopen("perf_app_update.txt", "w");

		fprintf(fp, "AU.Total %d (Time %d)\n", dwUpdateTime9-dwUpdateTime1, ELTimer_GetMSec());
		fprintf(fp, "AU.TU %d\n", dwUpdateTime2-dwUpdateTime1);
		fprintf(fp, "AU.NU %d\n", dwUpdateTime3-dwUpdateTime2);
		fprintf(fp, "AU.KU %d\n", dwUpdateTime4-dwUpdateTime3);
		fprintf(fp, "AU.MP %d\n", dwUpdateTime5-dwUpdateTime4);
		fprintf(fp, "AU.CP %d\n", dwUpdateTime6-dwUpdateTime5);
		fprintf(fp, "AU.RU %d\n", dwUpdateTime7-dwUpdateTime6);
		fprintf(fp, "AU.CU %d\n", dwUpdateTime8-dwUpdateTime7);
		fprintf(fp, "AU.MU %d\n", dwUpdateTime9-dwUpdateTime8);
		fprintf(fp, "AU.UU %d\n", dwUpdateTime10-dwUpdateTime9);
		fprintf(fp, "----------------------------------\n");
		fflush(fp);
	}
#endif

	//Update하는데 걸린시간.delta값
	m_dwCurUpdateTime = ELTimer_GetMSec() - updatestart;

	DWORD dwCurrentTime = ELTimer_GetMSec();
	BOOL  bCurrentLateUpdate = FALSE;

	s_bFrameSkip = false;

	if (dwCurrentTime > s_uiNextFrameTime)
	{
		int dt = dwCurrentTime - s_uiNextFrameTime;
		int nAdjustTime = ((float)dt / (float)uiFrameTime) * uiFrameTime;

		if ( dt >= 500 )
		{
			s_uiNextFrameTime += nAdjustTime;
			printf("FrameSkip Adjusting... %d\n",nAdjustTime);
			CTimer::Instance().Adjust(nAdjustTime);
		}

		s_bFrameSkip = true;
		bCurrentLateUpdate = TRUE;
	}

	//s_bFrameSkip = false;

	//if (dwCurrentTime > s_uiNextFrameTime)
	//{
	//	int dt = dwCurrentTime - s_uiNextFrameTime;

	//	//너무 늦었을 경우 따라잡는다.
	//	//그리고 m_dwCurUpdateTime는 delta인데 delta랑 absolute time이랑 비교하면 어쩌자는겨?
	//	//if (dt >= 500 || m_dwCurUpdateTime > s_uiNextFrameTime)

	//	//기존코드대로 하면 0.5초 이하 차이난 상태로 update가 지속되면 계속 rendering frame skip발생
	//	if (dt >= 500 || m_dwCurUpdateTime > s_uiNextFrameTime)
	//	{
	//		s_uiNextFrameTime += dt / uiFrameTime * uiFrameTime;
	//		printf("FrameSkip 보정 %d\n", dt / uiFrameTime * uiFrameTime);
	//		CTimer::Instance().Adjust((dt / uiFrameTime) * uiFrameTime);
	//		s_bFrameSkip = true;
	//	}
	//}

	if (m_isFrameSkipDisable)
		s_bFrameSkip = false;

#ifdef __VTUNE__
	s_bFrameSkip = false;
#endif
	/*
	static bool s_isPrevFrameSkip=false;
	static DWORD s_dwFrameSkipCount=0;
	static DWORD s_dwFrameSkipEndTime=0;

	static DWORD ERROR_FRAME_SKIP_COUNT = 60*5;
	static DWORD ERROR_FRAME_SKIP_TIME = ERROR_FRAME_SKIP_COUNT*18;

	//static DWORD MAX_FRAME_SKIP=0;

	if (IsActive())
	{
	DWORD dwFrameSkipCurTime=ELTimer_GetMSec();

	if (s_bFrameSkip)
	{
	// 이전 프레임도 스킵이라면..
	if (s_isPrevFrameSkip)
	{
	if (s_dwFrameSkipEndTime==0)
	{
	s_dwFrameSkipCount=0; // 프레임 체크는 로딩 대비
	s_dwFrameSkipEndTime=dwFrameSkipCurTime+ERROR_FRAME_SKIP_TIME; // 시간 체크는 로딩후 프레임 스킵 체크

	//printf("FrameSkipCheck Start\n");
	}
	++s_dwFrameSkipCount;

	//if (MAX_FRAME_SKIP<s_dwFrameSkipCount)
	//	MAX_FRAME_SKIP=s_dwFrameSkipCount;

	//printf("u %d c %d/%d t %d\n",
	//	dwUpdateTime9-dwUpdateTime1,
	//	s_dwFrameSkipCount,
	//	MAX_FRAME_SKIP,
	//	s_dwFrameSkipEndTime);

	//#ifndef _DEBUG
	// 일정 시간동안 계속 프레임 스킵만 한다면...
	if (s_dwFrameSkipCount>ERROR_FRAME_SKIP_COUNT && s_dwFrameSkipEndTime<dwFrameSkipCurTime)
	{
	s_isPrevFrameSkip=false;
	s_dwFrameSkipEndTime=0;
	s_dwFrameSkipCount=0;

	//m_pyNetworkStream.AbsoluteExitGame();

	/*
	TraceError("무한 프레임 스킵으로 접속을 종료합니다");

	{
	FILE* fp=fopen("errorlog.txt", "w");
	if (fp)
	{
	fprintf(fp, "FRAMESKIP\n");
	fprintf(fp, "Total %d\n",		dwUpdateTime9-dwUpdateTime1);
	fprintf(fp, "Timer %d\n",		dwUpdateTime2-dwUpdateTime1);
	fprintf(fp, "Network %d\n",		dwUpdateTime3-dwUpdateTime2);
	fprintf(fp, "Keyboard %d\n",	dwUpdateTime4-dwUpdateTime3);
	fprintf(fp, "Controll %d\n",	dwUpdateTime5-dwUpdateTime4);
	fprintf(fp, "Resource %d\n",	dwUpdateTime6-dwUpdateTime5);
	fprintf(fp, "Camera %d\n",		dwUpdateTime7-dwUpdateTime6);
	fprintf(fp, "Mouse %d\n",		dwUpdateTime8-dwUpdateTime7);
	fprintf(fp, "UI %d\n",			dwUpdateTime9-dwUpdateTime8);
	fclose(fp);

	WinExec("errorlog.exe", SW_SHOW);
	}
	}
	}
	}

	s_isPrevFrameSkip=true;
	}
	else
	{
	s_isPrevFrameSkip=false;
	s_dwFrameSkipCount=0;
	s_dwFrameSkipEndTime=0;
	}
	}
	else
	{
	s_isPrevFrameSkip=false;
	s_dwFrameSkipCount=0;
	s_dwFrameSkipEndTime=0;
	}
	*/
	if (!s_bFrameSkip)
	{
		CGrannyMaterial::TranslateSpecularMatrix(g_specularSpd, g_specularSpd, 0.0f);
		DWORD dwRenderStartTime = ELTimer_GetMSec();
		bool canRender = true;

        if (m_isMinimizedWnd)
        {
            canRender = false;
        }
        else
        {
            if (m_pyGraphic.IsLostDevice())
            {
                m_pyBackground.ReleaseCharacterShadowTexture();
                m_kRenderTargetManager.ReleaseRenderTargetTextures();

                if (m_pyGraphic.RestoreDevice())
                {
                    m_pyBackground.CreateCharacterShadowTexture();
                    m_kRenderTargetManager.CreateRenderTargetTextures();
                }
                else
                {
                    canRender = false;
                }
            }
        }

		if (!IsActive())
		{
			SkipRenderBuffering(3000);
		}

		// 리스토어 처리때를 고려해 일정 시간동안은 버퍼링을 하지 않는다
		if (!canRender)
		{
			RenderSpecial();
			SkipRenderBuffering(3000);
		}
		else
		{
//#ifdef USE_D3D9
//            m_grpDevice.Clear(true, true, false);
//#endif

			// RestoreLostDevice
			CCullingManager::Instance().Update();
			if (m_pyGraphic.Begin())
			{

				m_pyGraphic.ClearDepthBuffer();

#ifdef _DEBUG
				m_pyGraphic.SetClearColor(0.3f, 0.3f, 0.3f);
				m_pyGraphic.Clear();
#endif

				/////////////////////
				// Interface
				m_pyGraphic.SetInterfaceRenderState();

				OnUIRender();
				OnMouseRender();
				/////////////////////

				m_pyGraphic.End();

				//DWORD t1 = ELTimer_GetMSec();
				m_pyGraphic.Show();
				//DWORD t2 = ELTimer_GetMSec();

				DWORD dwRenderEndTime = ELTimer_GetMSec();

				static DWORD s_dwRenderCheckTime = dwRenderEndTime;
				static DWORD s_dwRenderRangeTime = 0;
				static DWORD s_dwRenderRangeFrame = 0;

				m_dwCurRenderTime = dwRenderEndTime - dwRenderStartTime;
				s_dwRenderRangeTime += m_dwCurRenderTime;
				++s_dwRenderRangeFrame;

				if (dwRenderEndTime-s_dwRenderCheckTime>1000)
				{
					m_fAveRenderTime=float(double(s_dwRenderRangeTime)/double(s_dwRenderRangeFrame));

					s_dwRenderCheckTime=ELTimer_GetMSec();
					s_dwRenderRangeTime=0;
					s_dwRenderRangeFrame=0;
				}

				DWORD dwCurFaceCount=m_pyGraphic.GetFaceCount();
				m_pyGraphic.ResetFaceCount();
				s_dwFaceCount += dwCurFaceCount;

				if (dwCurFaceCount > 5000)
				{
					// 프레임 완충 처리
					if (dwRenderEndTime > m_dwBufSleepSkipTime)
					{
						static float s_fBufRenderTime = 0.0f;

						float fCurRenderTime = m_dwCurRenderTime;

						if (fCurRenderTime > s_fBufRenderTime)
						{
							float fRatio = fMAX(0.5f, (fCurRenderTime - s_fBufRenderTime) / 30.0f);
							s_fBufRenderTime = (s_fBufRenderTime * (100.0f - fRatio) + (fCurRenderTime + 5) * fRatio) / 100.0f;
						}
						else
						{
							float fRatio = 0.5f;
							s_fBufRenderTime = (s_fBufRenderTime * (100.0f - fRatio) + fCurRenderTime * fRatio) / 100.0f;
						}

						// 한계치를 정한다
						if (s_fBufRenderTime > 100.0f)
							s_fBufRenderTime = 100.0f;

						DWORD dwBufRenderTime = s_fBufRenderTime;

						if (m_isWindowed)
						{
							if (dwBufRenderTime>58)
								dwBufRenderTime=64;
							else if (dwBufRenderTime>42)
								dwBufRenderTime=48;
							else if (dwBufRenderTime>26)
								dwBufRenderTime=32;
							else if (dwBufRenderTime>10)
								dwBufRenderTime=16;
							else
								dwBufRenderTime=8;
						}

						// 일정 프레임 속도에 맞추어주는쪽에 눈에 편하다
						// 아래에서 한번 하면 됬�?
						//if (m_dwCurRenderTime<dwBufRenderTime)
						//	Sleep(dwBufRenderTime-m_dwCurRenderTime);

						m_fAveRenderTime=s_fBufRenderTime;
					}

					m_dwFaceAccCount += dwCurFaceCount;
					m_dwFaceAccTime += m_dwCurRenderTime;

					m_fFaceSpd=(m_dwFaceAccCount/m_dwFaceAccTime);

					// 거리 자동 조절
					if (-1 == m_iForceSightRange)
					{
						static float s_fAveRenderTime = 16.0f;
						float fRatio=0.3f;
						s_fAveRenderTime=(s_fAveRenderTime*(100.0f-fRatio)+max(16.0f, m_dwCurRenderTime)*fRatio)/100.0f;


						float fFar=25600.0f;
						float fNear=MIN_FOG;
						double dbAvePow=double(1000.0f/s_fAveRenderTime);
						double dbMaxPow=60.0;
						float fDistance=max(fNear+(fFar-fNear)*(dbAvePow)/dbMaxPow, fNear);
						m_pyBackground.SetViewDistanceSet(0, fDistance);
					}
					// 거리 강제 설정시
					else
					{
						m_pyBackground.SetViewDistanceSet(0, float(m_iForceSightRange));
					}
				}
				else
				{
					// 10000 폴리곤 보다 적을때는 가장 멀리 보이게 한다
					m_pyBackground.SetViewDistanceSet(0, 25600.0f);
				}

				++s_dwRenderFrameCount;
			}
		}
	}

	int rest = s_uiNextFrameTime - ELTimer_GetMSec();

	if (rest > 0 && !bCurrentLateUpdate )
	{
		s_uiLoad -= rest;	// 쉰 시간은 로드에서 뺀다..
		Sleep(rest);
	}

	++s_dwUpdateFrameCount;

	s_uiLoad += ELTimer_GetMSec() - dwStart;
	//m_Profiler.ProfileByScreen();
	return true;
}

void CPythonApplication::UpdateClientRect()
{
	RECT rcApp;
	GetClientRect(&rcApp);
	OnSizeChange(rcApp.right - rcApp.left, rcApp.bottom - rcApp.top);
}

void CPythonApplication::SetMouseHandler(PyObject* poMouseHandler)
{
	m_poMouseHandler = poMouseHandler;
}

int CPythonApplication::CheckDeviceState()
{
    CGraphicDevice::EDeviceState e_deviceState = m_grpDevice.GetDeviceState();
    switch (e_deviceState)
    {
        case CGraphicDevice::DEVICESTATE_NULL:
        {
            return DEVICE_STATE_FALSE;
        }
        case CGraphicDevice::DEVICESTATE_BROKEN:
        {
            return DEVICE_STATE_SKIP;
        }
        case CGraphicDevice::DEVICESTATE_NEEDS_RESET:
        {
            if (!m_grpDevice.Reset())
            {
                return DEVICE_STATE_SKIP;
            }

            break;
        }
        default:
        {
            break;
        }
    }

    return DEVICE_STATE_OK;
}

#ifdef ENABLE_MORE_FPS
std::tuple<int, int> GetTimer(int frequency)
{
	if (frequency < 60)
		return { 32, 1 };
	else if (frequency < 90)
		return { 16, 1 };
	else if (frequency < 120)
		return { 11, 0 };
	else if (frequency < 150)
		return { 8, 1 };
	else if (frequency < 180)
		return { 6, 1 };
	else if (frequency < 220)
		return { 5, 1 };
	else if (frequency < 240)
		return { 4, 1 };
	else if (frequency < 300)
		return { 3, 1 };
	else if (frequency < 360)
		return { 2, 1 };
	else if (frequency >= 360)
		return { 1, 1 };

	return  { 16, 1 };
}
#endif

bool CPythonApplication::CreateDevice(int width, int height, int Windowed, int bit /* = 32*/, int frequency /* = 0*/)
{
	int iRet;

#ifdef USE_D3D9
    m_grpDevice.InitBackBufferCount(Windowed ? 2 : 1);
#else
    m_grpDevice.InitBackBufferCount(2);
#endif

	m_grpDevice.RegisterWarningString(CGraphicDevice::CREATE_BAD_DRIVER, ApplicationStringTable_GetStringz(IDS_WARN_BAD_DRIVER, "WARN_BAD_DRIVER"));
	m_grpDevice.RegisterWarningString(CGraphicDevice::CREATE_NO_TNL, ApplicationStringTable_GetStringz(IDS_WARN_NO_TNL, "WARN_NO_TNL"));

#ifdef ENABLE_MORE_FPS
	g_fGameFPS = static_cast<float>(frequency);

	const auto diff = 0.42f;
	g_specularSpd = (diff / frequency);

	auto [first, sec] = GetTimer(frequency);
	CTimer::Instance().SetUpdateTime(frequency, first, sec);
	//TraceError("Device - Frequency %d g_fGameFPS %.3f (first %d sec %d) val %d specular %.3f", frequency, g_fGameFPS, first, sec, value, g_specularSpd);
#endif

	iRet = m_grpDevice.Create(GetWindowHandle(), width, height, Windowed ? true : false, bit,frequency);

	switch (iRet)
	{
	case CGraphicDevice::CREATE_OK:
		return true;

	case CGraphicDevice::CREATE_REFRESHRATE:
		return true;

	case CGraphicDevice::CREATE_ENUM:
	case CGraphicDevice::CREATE_DETECT:
		SET_EXCEPTION(CREATE_NO_APPROPRIATE_DEVICE);
		TraceError("CreateDevice: Enum & Detect failed");
		return false;

	case CGraphicDevice::CREATE_NO_DIRECTX:
		//PyErr_SetString(PyExc_RuntimeError, "DirectX 8.1 or greater required to run game");
		SET_EXCEPTION(CREATE_NO_DIRECTX);
		TraceError("CreateDevice: DirectX 8.1 or greater required to run game");
		return false;

	case CGraphicDevice::CREATE_DEVICE:
		//PyErr_SetString(PyExc_RuntimeError, "GraphicDevice create failed");
		SET_EXCEPTION(CREATE_DEVICE);
		TraceError("CreateDevice: GraphicDevice create failed");
		return false;

	case CGraphicDevice::CREATE_FORMAT:
		SET_EXCEPTION(CREATE_FORMAT);
		TraceError("CreateDevice: Change the screen format");
		return false;

		/*case CGraphicDevice::CREATE_GET_ADAPTER_DISPLAY_MODE:
		//PyErr_SetString(PyExc_RuntimeError, "GetAdapterDisplayMode failed");
		SET_EXCEPTION(CREATE_GET_ADAPTER_DISPLAY_MODE);
		TraceError("CreateDevice: GetAdapterDisplayMode failed");
		return false;*/

	case CGraphicDevice::CREATE_GET_DEVICE_CAPS:
		PyErr_SetString(PyExc_RuntimeError, "GetDevCaps failed");
		TraceError("CreateDevice: GetDevCaps failed");
		return false;

	case CGraphicDevice::CREATE_GET_DEVICE_CAPS2:
		PyErr_SetString(PyExc_RuntimeError, "GetDevCaps2 failed");
		TraceError("CreateDevice: GetDevCaps2 failed");
		return false;

	default:
		if (iRet & CGraphicDevice::CREATE_OK)
		{
			//if (iRet & CGraphicDevice::CREATE_BAD_DRIVER)
			//{
			//	LogBox(ApplicationStringTable_GetStringz(IDS_WARN_BAD_DRIVER), NULL, GetWindowHandle());
			//}
			if (iRet & CGraphicDevice::CREATE_NO_TNL)
			{
				CGrannyLODController::SetMinLODMode(true);
				//LogBox(ApplicationStringTable_GetStringz(IDS_WARN_NO_TNL), NULL, GetWindowHandle());
			}
			return true;
		}

		//PyErr_SetString(PyExc_RuntimeError, "Unknown Error!");
		SET_EXCEPTION(UNKNOWN_ERROR);
		TraceError("CreateDevice: Unknown Error!");
		return false;
	}
}

void CPythonApplication::Loop()
{
	while (1)
	{
		// CPythonNetworkStream::Instance().ProcessDelayedChats();

		if (IsMessage())
		{
			if (!MessageProcess())
				break;
		}
		else
		{
			if (!Process())
				break;

			m_dwLastIdleTime=ELTimer_GetMSec();
		}
	}
}

// SUPPORT_NEW_KOREA_SERVER
bool LoadLocaleData(const char* localePath)
{
	NANOBEGIN
		CPythonNonPlayer&	rkNPCMgr	= CPythonNonPlayer::Instance();
	CItemManager&		rkItemMgr	= CItemManager::Instance();
	CPythonSkill&		rkSkillMgr	= CPythonSkill::Instance();
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
#ifdef NEW_PET_SYSTEM
	CPythonSkillPet&    rkSkillPetMgr = CPythonSkillPet::Instance();
#endif

	char szItemList[256];
	char szItemProto[256];
	char szItemDesc[256];
#ifdef NEW_PET_SYSTEM
	char szSkillPetFileName[256];
#endif

	char szMobProto[256];
	char szSkillDescFileName[256];
	char szSkillTableFileName[256];
	char szInsultList[256];
#ifdef ENABLE_MOB_SCALE_SYSTEM
	char szMobScale[256];
#endif

	const char* localePathCommon = "locale/common";

	snprintf(szItemList,	sizeof(szItemList) ,	"%s/item_list.txt", localePathCommon);
	snprintf(szItemProto,	sizeof(szItemProto),	"%s/item_proto",	localePath);
	snprintf(szItemDesc,	sizeof(szItemDesc),	"%s/itemdesc.txt",	localePath);
	snprintf(szMobProto,	sizeof(szMobProto),	"%s/mob_proto",		localePath);
	snprintf(szSkillDescFileName, sizeof(szSkillDescFileName),	"%s/SkillDesc.txt", localePath);
	snprintf(szSkillTableFileName, sizeof(szSkillTableFileName),	"%s/SkillTable.txt", localePath);
	snprintf(szInsultList,	sizeof(szInsultList),	"%s/insult.txt", localePath);
#ifdef NEW_PET_SYSTEM
	snprintf(szSkillPetFileName, sizeof(szSkillPetFileName), "%s/pet_skill.txt", localePath);
#endif
#ifdef ENABLE_MOB_SCALE_SYSTEM
	snprintf(szMobScale, sizeof(szMobScale), "%s/mob_scale.txt", localePath);
#endif
#ifdef ENABLE_SHINING_TABLE_SYSTEM
	char szShiningTable[256];
	snprintf(szShiningTable, sizeof(szShiningTable), "%s/shining_table.txt", localePathCommon);
	if (!rkItemMgr.LoadShiningTable(szShiningTable))
	{
		Tracenf("LoadLocaleData - LoadShiningTable(%s) Error", szShiningTable);
	}
#endif

	rkNPCMgr.Destroy();
	rkItemMgr.Destroy();
	rkSkillMgr.Destroy();
#ifdef NEW_PET_SYSTEM
	rkSkillPetMgr.Destroy();
#endif
#ifdef ENABLE_AURA_SYSTEM
	char szAuraScale[256];
	snprintf(szAuraScale, sizeof(szAuraScale), "%s/aura_scale.txt", localePathCommon);
	if (!rkItemMgr.LoadAuraScale(szAuraScale))
	{
		TraceError("LoadLocaleData - LoadAuraScale(%s) Error", szAuraScale);
	}
#endif

	if (!rkItemMgr.LoadItemList(szItemList))
	{
		TraceError("LoadLocaleData - LoadItemList(%s) Error", szItemList);
	}

	if (!rkItemMgr.LoadItemTable(szItemProto))
	{
		TraceError("LoadLocaleData - LoadItemProto(%s) Error", szItemProto);
		return false;
	}

	if (!rkItemMgr.LoadItemDesc(szItemDesc))
	{
		Tracenf("LoadLocaleData - LoadItemDesc(%s) Error", szItemDesc);
	}

	if (!rkNPCMgr.LoadNonPlayerData(szMobProto))
	{
		TraceError("LoadLocaleData - LoadMobProto(%s) Error", szMobProto);
		return false;
	}

	if (!rkSkillMgr.RegisterSkillDesc(szSkillDescFileName))
	{
		TraceError("LoadLocaleData - RegisterSkillDesc(%s) Error", szSkillDescFileName);
		return false;
	}


	if (!rkSkillMgr.RegisterSkillTable(szSkillTableFileName))
	{
		TraceError("LoadLocaleData - RegisterSkillTable(%s) Error", szSkillTableFileName);
		return false;
	}
#ifdef NEW_PET_SYSTEM
	if (!rkSkillPetMgr.RegisterSkillPet(szSkillPetFileName))
	{
		TraceError("LoadLocaleData - RegisterSkillPet(%s) Error", szSkillPetFileName);
		return false;
	}
#endif
	if (!rkNetStream.LoadInsultList(szInsultList))
	{
		Tracenf("CPythonApplication - CPythonNetworkStream::LoadInsultList(%s)", szInsultList);
	}
#ifdef ENABLE_NPC_WEAR_ITEM
	if (!rkNPCMgr.LoadNPCWear("locale/common/npc_wear.txt"))
		Tracenf("CPythonApplication - CPythonNonPlayer::LoadNPCWear()");
#endif

	if (LocaleService_IsYMIR())
	{
		char szEmpireTextConvFile[256];
		for (DWORD dwEmpireID=1; dwEmpireID<=3; ++dwEmpireID)
		{
			sprintf(szEmpireTextConvFile, "%s/lang%d.cvt", localePath, dwEmpireID);
			if (!rkNetStream.LoadConvertTable(dwEmpireID, szEmpireTextConvFile))
			{
				TraceError("LoadLocaleData - CPythonNetworkStream::LoadConvertTable(%d, %s) FAILURE", dwEmpireID, szEmpireTextConvFile);
			}
		}
	}

#ifdef ENABLE_MOB_SCALE_SYSTEM
	if (!rkNPCMgr.LoadMobScale(szMobScale))
	{
		TraceError("LoadMobScale  Error(%s)", szMobScale);
		return false;
	}
#endif
	
	#ifdef ENABLE_SASH_SYSTEM
	char szItemScale[256];
	snprintf(szItemScale, sizeof(szItemScale), "%s/item_scale.txt", localePath);
	if (!rkItemMgr.LoadItemScale(szItemScale))
		Tracenf("LoadLocaleData: error while loading %s.", szItemScale);
	#endif
	/*
#if defined(ENABLE_SHOW_CHEST_DROP)
	char szChestDrop[256];
	snprintf(szChestDrop, sizeof(szChestDrop), "locale/en/chest_drop", localePath);
	if (!rkItemMgr.LoadChestDropInfo(szChestDrop))
	{
		TraceError("LoadLocaleData - LoadChestDropInfo(%s) Error", szChestDrop);
		return false;
	}
#endif
	*/

#ifdef ENABLE_WIKI
	CPythonWiki::Instance().ReadData(localePath);
#endif
#ifdef ENABLE_BATTLEPASS
	BattlePassManager::Instance().LoadBattlePass();
#endif
    NANOEND
    return true;
}
// END_OF_SUPPORT_NEW_KOREA_SERVER

unsigned __GetWindowMode(bool windowed)
{
	if (windowed)
		return WS_OVERLAPPED | WS_CAPTION |   WS_SYSMENU | WS_MINIMIZEBOX;

	return WS_POPUP;
}

#ifdef ENABLE_DISCORD_STUFF
bool CPythonApplication::Create(PyObject* poSelf, const char* c_szName, const char* c_szMiniName, int width, int height, int Windowed)
#else
bool CPythonApplication::Create(PyObject* poSelf, const char* c_szName, int width, int height, int Windowed)
#endif
{
	NANOBEGIN
		Windowed = CPythonSystem::Instance().IsWindowed() ? 1 : 0;

	bool bAnotherWindow = false;

	if (FindWindow(NULL, c_szName))
		bAnotherWindow = true;

	m_dwWidth = width;
	m_dwHeight = height;

	// Window
	UINT WindowMode = __GetWindowMode(Windowed ? true : false);

#ifdef ENABLE_DISCORD_STUFF
	oldIcon = LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_METIN2));
	if (!CMSWindow::Create(c_szName, 4, 0, WindowMode, oldIcon, IDC_CURSOR_NORMAL))
#else
	if (!CMSWindow::Create(c_szName, 4, 0, WindowMode, ::LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_METIN2)), IDC_CURSOR_NORMAL))
#endif
	{
		//PyErr_SetString(PyExc_RuntimeError, "CMSWindow::Create failed");
		TraceError("CMSWindow::Create failed");
		SET_EXCEPTION(CREATE_WINDOW);
		return false;
	}

	if (m_pySystem.IsUseDefaultIME())
	{
		CPythonIME::Instance().UseDefaultIME();
	}
#if defined(ENABLE_DISCORD_RPC)
	m_pyNetworkStream.Discord_Start();
#endif

	// 풀스크린 모드이고
	// 디폴트 IME 를 사용하거나 유럽 버전이면
	// 윈도우 풀스크린 모드를 사용한다
	if (!m_pySystem.IsWindowed() && (m_pySystem.IsUseDefaultIME() || LocaleService_IsEUROPE()))
	{
		m_isWindowed = false;
		m_isWindowFullScreenEnable = TRUE;
		__SetFullScreenWindow(GetWindowHandle(), width, height, m_pySystem.GetBPP());

		Windowed = true;
	}
	else
	{
		AdjustSize(m_pySystem.GetWidth(), m_pySystem.GetHeight());

		if (Windowed)
		{
			m_isWindowed = true;

			if (bAnotherWindow)
			{
				RECT rc;

				GetClientRect(&rc);

				int windowWidth = rc.right - rc.left;
				int windowHeight = (rc.bottom - rc.top);

				CMSApplication::SetPosition(GetScreenWidth() - windowWidth, GetScreenHeight() - 60 - windowHeight);
			}
		}
		else
		{
			m_isWindowed = false;
			SetPosition(0, 0);
		}
	}

	NANOEND
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Cursor
		if (!CreateCursors())
		{
			//PyErr_SetString(PyExc_RuntimeError, "CMSWindow::Cursors Create Error");
			TraceError("CMSWindow::Cursors Create Error");
			SET_EXCEPTION("CREATE_CURSOR");
			return false;
		}

		if (!m_pySystem.IsNoSoundCard())
		{
			// Sound
			if (!m_SoundManager.Create())
			{
				// NOTE : 중국측의 요청으로 생략
				//		LogBox(ApplicationStringTable_GetStringz(IDS_WARN_NO_SOUND_DEVICE));
			}
		}

		extern bool GRAPHICS_CAPS_SOFTWARE_TILING;

		if (!m_pySystem.IsAutoTiling())
			GRAPHICS_CAPS_SOFTWARE_TILING = m_pySystem.IsSoftwareTiling();

		// Device
		if (!CreateDevice(m_pySystem.GetWidth(), m_pySystem.GetHeight(), Windowed, m_pySystem.GetBPP(), m_pySystem.GetFrequency()))
			return false;

		GrannyCreateSharedDeformBuffer();

		if (m_pySystem.IsAutoTiling())
		{
			if (m_grpDevice.IsFastTNL())
			{
				m_pyBackground.ReserveSoftwareTilingEnable(false);
			}
			else
			{
				m_pyBackground.ReserveSoftwareTilingEnable(true);
			}
		}
		else
		{
			m_pyBackground.ReserveSoftwareTilingEnable(m_pySystem.IsSoftwareTiling());
		}

        SetVisibleMode(false);

		if (m_isWindowFullScreenEnable) //m_pySystem.IsUseDefaultIME() && !m_pySystem.IsWindowed())
		{
			SetWindowPos(GetWindowHandle(), HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
		}

		if (!InitializeKeyboard(GetWindowHandle()))
			return false;

		m_pySystem.GetDisplaySettings();

		// Mouse
		if (m_pySystem.IsSoftwareCursor())
			SetCursorMode(CURSOR_MODE_SOFTWARE);
		else
			SetCursorMode(CURSOR_MODE_HARDWARE);

		// Network
		if (!m_netDevice.Create())
		{
			//PyErr_SetString(PyExc_RuntimeError, "NetDevice::Create failed");
			TraceError("NetDevice::Create failed");
			SET_EXCEPTION("CREATE_NETWORK");
			return false;
		}

		//CAnticheatManager::Instance().InitializeAnticheatRoutines(GetInstance(), GetWindowHandle());

		if (!m_grpDevice.IsFastTNL())
			CGrannyLODController::SetMinLODMode(true);

		m_pyItem.Create();

		// Other Modules
		DefaultFont_Startup();

		CPythonIME::Instance().Create(GetWindowHandle());
		CPythonIME::Instance().SetText("", 0);
		CPythonTextTail::Instance().Initialize();
#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
		if (!m_BadgeNotificationManager.Create())
		{
			TraceError("CBadgeNotificationManager::Create - FAILURE");
			return false;
		}
#endif
		// Light Manager
		m_LightManager.Initialize();

		CGraphicImageInstance::CreateSystem(32);
#ifdef ENABLE_DISCORD_STUFF
		NID.cbSize = sizeof(NID);
		NID.hIcon = oldIcon;
		NID.hWnd = GetWindowHandle();
		NID.uFlags = NIF_ICON | NIF_TIP;
		strcpy(NID.szTip, c_szMiniName);
		NID.uCallbackMessage = WM_APP + 1;
		NID.hBalloonIcon = NID.hIcon;
		Shell_NotifyIcon(NIM_ADD, &NID);
#endif
		// 백업
		STICKYKEYS sStickKeys;
		memset(&sStickKeys, 0, sizeof(sStickKeys));
		sStickKeys.cbSize = sizeof(sStickKeys);
		SystemParametersInfo( SPI_GETSTICKYKEYS, sizeof(sStickKeys), &sStickKeys, 0 );
		m_dwStickyKeysFlag = sStickKeys.dwFlags;

		// 설정
		sStickKeys.dwFlags &= ~(SKF_AVAILABLE|SKF_HOTKEYACTIVE);
		SystemParametersInfo( SPI_SETSTICKYKEYS, sizeof(sStickKeys), &sStickKeys, 0 );

		// SphereMap
		CGrannyMaterial::CreateSphereMap(0, "d:/ymir work/special/spheremap.jpg");
		CGrannyMaterial::CreateSphereMap(1, "d:/ymir work/special/spheremap01.jpg");
		return true;
}

void CPythonApplication::SetGlobalCenterPosition(long x, long y)
{
	CPythonBackground& rkBG=CPythonBackground::Instance();
	rkBG.GlobalPositionToLocalPosition(x, y);

	float z = CPythonBackground::Instance().GetHeight(x, y);

	CPythonApplication::Instance().SetCenterPosition(x, y, z);
}

void CPythonApplication::SetCenterPosition(float fx, float fy, float fz)
{
	m_v3CenterPosition.x = +fx;
	m_v3CenterPosition.y = -fy;
	m_v3CenterPosition.z = +fz;
}

void CPythonApplication::GetCenterPosition(TPixelPosition * pPixelPosition)
{
	pPixelPosition->x = +m_v3CenterPosition.x;
	pPixelPosition->y = -m_v3CenterPosition.y;
	pPixelPosition->z = +m_v3CenterPosition.z;
}


void CPythonApplication::SetServerTime(time_t tTime)
{
	m_dwStartLocalTime	= ELTimer_GetMSec();
	m_tServerTime		= tTime;
	m_tLocalStartTime	= time(0);
}

time_t CPythonApplication::GetServerTime()
{
	return (ELTimer_GetMSec() - m_dwStartLocalTime) + m_tServerTime;
}

// 2005.03.28 - MALL 아이템에 들어있는 시간의 단위가 서버에서 time(0) 으로 만들어지는
//              값이기 때문에 단위를 맞추기 위해 시간 관련 처리를 별도로 추가
time_t CPythonApplication::GetServerTimeStamp()
{
	return (time(0) - m_tLocalStartTime) + m_tServerTime;
}

float CPythonApplication::GetGlobalTime()
{
	return m_fGlobalTime;
}

float CPythonApplication::GetGlobalElapsedTime()
{
	return m_fGlobalElapsedTime;
}

void CPythonApplication::SetFPS(int iFPS)
{
	m_iFPS = iFPS;
}

int CPythonApplication::GetWidth()
{
	return m_dwWidth;
}

int CPythonApplication::GetHeight()
{
	return m_dwHeight;
}

void CPythonApplication::SetConnectData(const char * c_szIP, int iPort)
{
	m_strIP = c_szIP;
	m_iPort = iPort;
}

void CPythonApplication::GetConnectData(std::string & rstIP, int & riPort)
{
	rstIP	= m_strIP;
	riPort	= m_iPort;
}

void CPythonApplication::EnableSpecialCameraMode()
{
	m_isSpecialCameraMode = TRUE;
}

void CPythonApplication::SetCameraSpeed(int iPercentage)
{
	m_fCameraRotateSpeed = c_fDefaultCameraRotateSpeed * float(iPercentage) / 100.0f;
	m_fCameraPitchSpeed = c_fDefaultCameraPitchSpeed * float(iPercentage) / 100.0f;
	m_fCameraZoomSpeed = c_fDefaultCameraZoomSpeed * float(iPercentage) / 100.0f;
}

void CPythonApplication::SetForceSightRange(int iRange)
{
	m_iForceSightRange = iRange;
}

#ifdef USE_CAPTCHA_SYSTEM
void CPythonApplication::SetWindowInvisible(bool invisible)
{
    if (invisible)
    {
        SetWindowDisplayAffinity(GetWindowHandle(), WDA_MONITOR);
    }
    else
    {
        SetWindowDisplayAffinity(GetWindowHandle(), WDA_NONE);
    }
}

bool CPythonApplication::IsWindowVisisble()
{
    DWORD affinity = 0;
    if (GetWindowDisplayAffinity(GetWindowHandle(), &affinity))
    {
        if (affinity & WDA_NONE)
        {
            return true;
        }
    }

    return false;
}

bool CPythonApplication::SaveImage(const char* c_szFileName, uint16_t wPin)
{
    cimg_library::CImg<unsigned char> image(62, 27, 1, 3);

    cimg_forXY(image, x, y)
    {
        image(x, y, 0, 0) = 0;
        image(x, y, 0, 1) = 0;
        image(x, y, 0, 2) = 0;
    }

    unsigned char static s_blackColor[]    = { 0,      0,      0   };
    unsigned char static s_whiteColor[]    = { 255,    255,    255 };

    image.draw_text(5, 1, std::to_string(wPin).c_str(), s_whiteColor, s_blackColor, 1, 26);

    char szPath[256];
    snprintf(szPath, sizeof(szPath), "UserData//%s", c_szFileName);

    image.save(szPath);

    return true;
}
#endif

void CPythonApplication::Clear()
{
	m_pySystem.Clear();
}
#ifdef ENABLE_WHISPER_FLASHING
#include <Windows.h>
void CPythonApplication::FlashApplication()
{
	HWND hWnd = GetWindowHandle();
	FLASHWINFO fi;
	fi.cbSize = sizeof(FLASHWINFO);
	fi.hwnd = hWnd;
	fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	fi.uCount = 0;
	fi.dwTimeout = 0;
	FlashWindowEx(&fi);
}
#endif
void CPythonApplication::Destroy()
{
#ifdef ENABLE_IKASHOP_RENEWAL
	CPythonIkarusShop::instance().DeleteEntities();
#endif
	// SphereMap
	CGrannyMaterial::DestroySphereMap();
#ifdef ENABLE_DISCORD_STUFF
	if (oldIcon != NULL)
		DestroyIcon(oldIcon);
	oldIcon = NULL;

	if (NID.hIcon != NULL)
		DestroyIcon(NID.hIcon);
	NID.hIcon = NULL;
	Shell_NotifyIcon(NIM_DELETE, &NID);
	ZeroMemory(&NID, sizeof(NOTIFYICONDATA));
#endif
	m_kWndMgr.Destroy();

	CPythonSystem::Instance().SaveConfig();

	m_kRenderTargetManager.Destroy();
	
	DestroyCollisionInstanceSystem();

	m_pySystem.SaveInterfaceStatus();

	m_pyEventManager.Destroy();
	m_FlyingManager.Destroy();

	m_pyMiniMap.Destroy();

	m_pyTextTail.Destroy();
	m_pyChat.Destroy();
	m_kChrMgr.Destroy();
	m_RaceManager.Destroy();

	m_pyItem.Destroy();
	m_kItemMgr.Destroy();

	m_pyBackground.Destroy();

	m_kEftMgr.Destroy();
	m_LightManager.Destroy();
#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
	m_BadgeNotificationManager.Destroy();
#endif
	// DEFAULT_FONT
	DefaultFont_Cleanup();
	// END_OF_DEFAULT_FONT

	GrannyDestroySharedDeformBuffer();

	m_pyGraphic.Destroy();
	//m_pyNetworkDatagram.Destroy();

#if defined(ENABLE_DISCORD_RPC)
	m_pyNetworkStream.Discord_Close();
#endif
	
	m_pyRes.Destroy();

	m_kGuildMarkDownloader.Disconnect();

	CGrannyModelInstance::DestroySystem();
	CGraphicImageInstance::DestroySystem();


	m_SoundManager.Destroy();
	m_grpDevice.Destroy();

	// FIXME : 만들어져 있지 않음 - [levites]
	//CSpeedTreeForestDirectX8::Instance().Clear();

	CAttributeInstance::DestroySystem();
	CTextFileLoader::DestroySystem();
	DestroyCursors();

	CMSApplication::Destroy();

	STICKYKEYS sStickKeys;
	memset(&sStickKeys, 0, sizeof(sStickKeys));
	sStickKeys.cbSize = sizeof(sStickKeys);
	sStickKeys.dwFlags = m_dwStickyKeysFlag;
	SystemParametersInfo( SPI_SETSTICKYKEYS, sizeof(sStickKeys), &sStickKeys, 0 );
}

#ifdef ENABLE_MULTI_FARM_BLOCK
void CPythonApplication::MultiFarmBlockIcon(BYTE bStatus)
{
#ifdef ENABLE_DISCORD_STUFF
	m_bFarmIsBlocked = !bStatus;
	ChangeIcon();
#else
	HICON exeIcon = bStatus ? LoadIcon(ms_hInstance, MAKEINTRESOURCE(IDI_METIN2)) : LoadIcon(ms_hInstance, MAKEINTRESOURCE(BLOCK_METIN2));
	SendMessage(GetWindowHandle(), WM_SETICON, ICON_BIG, (LPARAM)exeIcon);
#endif
}
#endif

#ifdef ENABLE_DISCORD_STUFF
void CPythonApplication::ChangeIcon()
{
	if (oldIcon != NULL)
		DestroyIcon(oldIcon);
	oldIcon = NULL;

	const DWORD count = CPythonChat::Instance().GetNewChatCount();
	if (count == 0)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2 : IDI_METIN2));
	else if (count == 1)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_1 : IDI_METIN2_1));
	else if (count == 2)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_2 :IDI_METIN2_2));
	else if (count == 3)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_3 :IDI_METIN2_3));
	else if (count == 4)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_4 :IDI_METIN2_4));
	else if (count == 5)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_5 :IDI_METIN2_5));
	else if (count == 6)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_6 :IDI_METIN2_6));
	else if (count == 7)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_7 :IDI_METIN2_7));
	else if (count == 8)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_8 :IDI_METIN2_8));
	else if (count == 9)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_9 :IDI_METIN2_9));
	else if (count > 9)
		oldIcon = LoadIcon(ms_hInstance, MAKEINTRESOURCE(m_bFarmIsBlocked ? BLOCK_METIN2_10 :IDI_METIN2_10));
	if (oldIcon == NULL)
		return;
	NID.uFlags = NIF_ICON;
	NID.hIcon = oldIcon;
	Shell_NotifyIcon(NIM_MODIFY, &NID);

	SendMessage(GetWindowHandle(), WM_SETICON, ICON_BIG, (LPARAM)oldIcon);
}
void CPythonApplication::SetTitle(const char* szTitle, const char* szMiniTitle)
{
	CMSWindow::SetText(szTitle);
	NID.uFlags = NIF_ICON | NIF_TIP;
	strcpy(NID.szTip, szMiniTitle);
	Shell_NotifyIcon(NIM_MODIFY, &NID);
}
void CPythonApplication::SetMessage(const char* szMsgTitle, const char* szMsgText)
{
	if (!CPythonSystem::Instance().GetStuffMessage())
		return;
	NID.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
	strcpy(NID.szInfoTitle, szMsgTitle);
	strcpy(NID.szInfo, szMsgText);
	Shell_NotifyIconA(NIM_MODIFY, &NID);
}
#endif
