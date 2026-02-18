#pragma once

#include "../eterBase/Utils.h"
#include <cstdint>

#ifdef ENABLE_ANIMATED_WINDOW
#include <optional>
#include <tweeny/include/tweeny.h>
#endif

#ifdef INSIDE_RENDER
#include <cstdint>
#endif

namespace UI
{
	class CWindow
	{
		public:
			typedef std::list<CWindow *> TWindowContainer;

			static DWORD Type();
			BOOL IsType(DWORD dwType);

			enum EHorizontalAlign
			{
				HORIZONTAL_ALIGN_LEFT = 0,
				HORIZONTAL_ALIGN_CENTER = 1,
				HORIZONTAL_ALIGN_RIGHT = 2,
			};

			enum EVerticalAlign
			{
				VERTICAL_ALIGN_TOP = 0,
				VERTICAL_ALIGN_CENTER = 1,
				VERTICAL_ALIGN_BOTTOM = 2,
			};

			enum EFlags
			{
				FLAG_MOVABLE			= (1 <<  0),	// 움직일 수 있는 창
				FLAG_LIMIT				= (1 <<  1),	// 창이 화면을 벗어나지 않음
				FLAG_SNAP				= (1 <<  2),	// 스냅 될 수 있는 창
				FLAG_DRAGABLE			= (1 <<  3),
				FLAG_ATTACH				= (1 <<  4),	// 완전히 부모에 붙어 있는 창 (For Drag / ex. ScriptWindow)
				FLAG_RESTRICT_X			= (1 <<  5),	// 좌우 이동 제한
				FLAG_RESTRICT_Y			= (1 <<  6),	// 상하 이동 제한
				FLAG_NOT_CAPTURE		= (1 <<  7),
				FLAG_FLOAT				= (1 <<  8),	// 공중에 떠있어서 순서 재배치가 되는 창
				FLAG_NOT_PICK			= (1 <<  9),	// 마우스에 의해 Pick되지 않는 창
				FLAG_IGNORE_SIZE		= (1 << 10),
				FLAG_RTL				= (1 << 11),	// Right-to-left
#ifdef ENABLE_ANIMATED_WINDOW
				FLAG_ANIMATED_BOARD = (1 << 12),
#endif
			};

		public:
			CWindow(PyObject * ppyObject);
			virtual ~CWindow();

			void			AddChild(CWindow * pWin);

			void			Clear();
			void			DestroyHandle();
			void			Update();
			void			Render();
#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
			void SetWhisperTargetName(const std::string& target_name)
			{
				m_whisperTargetName = target_name;
			};

			const std::string& GetWhisperTargetName() const
			{
				return m_whisperTargetName;
			}
#endif
			void			SetName(const char * c_szName);
			const char *	GetName()		{ return m_strName.c_str(); }
			void			SetSize(long width, long height);
			long			GetWidth()		{ return m_lWidth; }
			long			GetHeight()		{ return m_lHeight; }
#ifdef ENABLE_ANIMATED_WINDOW
			void			SetScale(float x, float y);
#endif
			void			SetHorizontalAlign(DWORD dwAlign);
			void			SetVerticalAlign(DWORD dwAlign);
			void			SetPosition(long x, long y);
			void			GetPosition(long * plx, long * ply);
			long			GetPositionX( void ) const		{ return m_x; }
			long			GetPositionY( void ) const		{ return m_y; }
			RECT &			GetRect()		{ return m_rect; }
			void			GetLocalPosition(long & rlx, long & rly);
			void			GetMouseLocalPosition(long & rlx, long & rly);
			long			UpdateRect();

			RECT &			GetLimitBias()	{ return m_limitBiasRect; }
			void			SetLimitBias(long l, long r, long t, long b) { m_limitBiasRect.left = l, m_limitBiasRect.right = r, m_limitBiasRect.top = t, m_limitBiasRect.bottom = b; }

			void			Show(bool bForceShow = false);
			void			Hide(bool bForceHide = false);
#ifdef INSIDE_RENDER
			virtual	bool	IsShow();
			void			OnHideWithChilds();
			void			OnHide();
#else
			bool			IsShow() { return m_bShow; }
#endif
			bool			IsRendering();

			bool			HasParent()		{ return m_pParent ? true : false; }
			bool			HasChild()		{ return m_pChildList.empty() ? false : true; }
			int				GetChildCount()	{ return m_pChildList.size(); }

			CWindow *		GetRoot();
			CWindow *		GetParent();
#ifdef INSIDE_RENDER
			bool			IsChild(CWindow* pWin, bool bCheckRecursive = false);
#else
			bool			IsChild(CWindow * pWin);
#endif
			void			DeleteChild(CWindow * pWin);
			void			SetTop(CWindow * pWin);

			bool			IsIn(long x, long y);
			bool			IsIn();
			CWindow *		PickWindow(long x, long y);
			CWindow *		PickTopWindow(long x, long y);	// NOTE : Children으로 내려가지 않고 상위에서만
															//        체크 하는 특화된 함수

			void			__RemoveReserveChildren();

			void			AddFlag(DWORD flag)		{ SET_BIT(m_dwFlag, flag);		}
			void			RemoveFlag(DWORD flag)	{ REMOVE_BIT(m_dwFlag, flag);	}
			bool			IsFlag(DWORD flag)		{ return (m_dwFlag & flag) ? true : false;	}

#ifdef INSIDE_RENDER
			void			SetInsideRender(BOOL flag);
			void			GetRenderBox(RECT* box);
			void			UpdateTextLineRenderBox();
			void			UpdateRenderBox();
			void			UpdateRenderBoxRecursive();
#endif
			/////////////////////////////////////

			virtual void	OnRender();
			virtual void	OnUpdate();
			virtual void	OnChangePosition(){}

#ifdef INSIDE_RENDER
			virtual void	OnAfterRender();
			virtual void	OnUpdateRenderBox() {}
#endif

			virtual void	OnSetFocus();
			virtual void	OnKillFocus();

			virtual void	OnMouseDrag(long lx, long ly);
			virtual void	OnMouseOverIn();
			virtual void	OnMouseOverOut();
			virtual void	OnMouseOver();
			virtual void	OnDrop();
			virtual void	OnTop();
			virtual void	OnIMEUpdate();

			virtual void	OnMoveWindow(long x, long y);

			///////////////////////////////////////

			BOOL			RunIMETabEvent();
			BOOL			RunIMEReturnEvent();
			BOOL			RunIMEKeyDownEvent(int ikey);

			CWindow *		RunKeyDownEvent(int ikey);
			BOOL			RunKeyUpEvent(int ikey);
			BOOL			RunPressEscapeKeyEvent();
			BOOL			RunPressExitKeyEvent();

			virtual BOOL	OnIMETabEvent();
			virtual BOOL	OnIMEReturnEvent();
			virtual BOOL	OnIMEKeyDownEvent(int ikey);

			virtual BOOL	OnIMEChangeCodePage();
			virtual BOOL	OnIMEOpenCandidateListEvent();
			virtual BOOL	OnIMECloseCandidateListEvent();
			virtual BOOL	OnIMEOpenReadingWndEvent();
			virtual BOOL	OnIMECloseReadingWndEvent();

			virtual BOOL	OnMouseLeftButtonDown();
			virtual BOOL	OnMouseLeftButtonUp();
			virtual BOOL	OnMouseLeftButtonDoubleClick();
			virtual BOOL	OnMouseRightButtonDown();
			virtual BOOL	OnMouseRightButtonUp();
			virtual BOOL	OnMouseRightButtonDoubleClick();
			virtual BOOL	OnMouseMiddleButtonDown();
			virtual BOOL	OnMouseMiddleButtonUp();
#ifdef ENABLE_MOUSEWHEEL_EVENT
			virtual BOOL	OnMouseWheelScroll(short wDelta);
			virtual void	SetScrollable();
			virtual BOOL	OnMouseWheel(int nLen);
#endif

			virtual BOOL    RunMouseWheelEvent(long nLen);

			virtual BOOL	OnKeyDown(int ikey);
			virtual BOOL	OnKeyUp(int ikey);
			virtual BOOL	OnPressEscapeKey();
			virtual BOOL	OnPressExitKey();
			///////////////////////////////////////

			virtual void	SetColor(DWORD dwColor){}
			virtual BOOL	OnIsType(DWORD dwType);
			/////////////////////////////////////

			virtual BOOL	IsWindow() { return TRUE; }
			/////////////////////////////////////

#ifdef USE_CLIP_MASK
            virtual void SetClippingMaskRect(const RECT& rMask);
            virtual void SetClippingMaskWindow(CWindow* pMaskWindow);
#endif

#ifdef INSIDE_RENDER
		public:
			virtual void	iSetRenderingRect(int iLeft, int iTop, int iRight, int iBottom);
			virtual void	SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
			virtual int		GetRenderingWidth();
			virtual int		GetRenderingHeight();
			void			ResetRenderingRect(bool bCallEvent = true);

		private:
			virtual void	OnSetRenderingRect();
#endif

		protected:
			std::string			m_strName;
#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
			std::string			m_whisperTargetName;
#endif
			EHorizontalAlign	m_HorizontalAlign;
			EVerticalAlign		m_VerticalAlign;
			long				m_x, m_y;				// X,Y 상대좌표
			long				m_lWidth, m_lHeight;	// 크기
			RECT				m_rect;					// Global 좌표
			RECT				m_limitBiasRect;		// limit bias 값

			bool				m_bMovable;
			bool				m_bShow;
#ifdef INSIDE_RENDER
			RECT				m_renderingRect;
			BOOL				m_isInsideRender;
			RECT				m_renderBox;
#endif
			bool				m_bAnimation;

			DWORD				m_dwFlag;

			PyObject *			m_poHandler;

			CWindow	*			m_pParent;
			TWindowContainer	m_pChildList;

			BOOL				m_isUpdatingChildren;
			TWindowContainer	m_pReserveChildList;
#ifdef ENABLE_ANIMATED_WINDOW
			D3DXVECTOR2			m_v2Scale;
			D3DXMATRIX			m_matScaling;
			std::optional<tweeny::tween<float>> m_sizeAnimation;
#endif
#ifdef ENABLE_MOUSEWHEEL_EVENT
			bool				m_bIsScrollable;
#endif

#ifdef USE_CLIP_MASK
            bool m_bEnableMask;
            CWindow* m_pMaskWindow;
            RECT m_rMaskRect;
#endif

#ifdef _DEBUG
		public:
			DWORD				DEBUG_dwCounter;
#endif
	};

	class CLayer : public CWindow
	{
		public:
			CLayer(PyObject * ppyObject) : CWindow(ppyObject) {}
			virtual ~CLayer() {}

			BOOL IsWindow() { return FALSE; }
	};


	class CUiRenderTarget : public CWindow
	{
	public:
		CUiRenderTarget(PyObject* ppyObject);
		virtual ~CUiRenderTarget();

		static DWORD Type();
		bool SetRenderTarget(int index);
#ifdef ENABLE_WIKI
		void SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
		RECT rect_ex;
#endif

	protected:
		DWORD m_dwIndex;
		void OnRender();
		BOOL OnIsType(DWORD dwType);
	};
	
	class CBox : public CWindow
	{
		public:
			CBox(PyObject * ppyObject);
			virtual ~CBox();

			void SetColor(DWORD dwColor);

		protected:
			void OnRender();

		protected:
			DWORD m_dwColor;
	};

	class CBar : public CWindow
	{
		public:
			CBar(PyObject * ppyObject);
			virtual ~CBar();

			void SetColor(DWORD dwColor);

		protected:
			void OnRender();

		protected:
			DWORD m_dwColor;
	};

	class CLine : public CWindow
	{
		public:
			CLine(PyObject * ppyObject);
			virtual ~CLine();

			void SetColor(DWORD dwColor);

		protected:
			void OnRender();

		protected:
			DWORD m_dwColor;
	};

	class CBar3D : public CWindow
	{
		public:
			static DWORD Type();

		public:
			CBar3D(PyObject * ppyObject);
			virtual ~CBar3D();

			void SetColor(DWORD dwLeft, DWORD dwRight, DWORD dwCenter);

		protected:
			void OnRender();

		protected:
			DWORD m_dwLeftColor;
			DWORD m_dwRightColor;
			DWORD m_dwCenterColor;
	};

	// Text
	class CTextLine : public CWindow
	{
		public:
			CTextLine(PyObject * ppyObject);
			virtual ~CTextLine();

#ifdef INSIDE_RENDER
			static DWORD Type();
#endif

			void SetMax(int iMax);
			void SetHorizontalAlign(int iType);
			void SetVerticalAlign(int iType);
			void SetSecret(BOOL bFlag);
			void SetOutline(BOOL bFlag);
			void SetFeather(BOOL bFlag);
			void SetMultiLine(BOOL bFlag);
			void SetFontName(const char * c_szFontName);
			void SetFontColor(DWORD dwColor);
			void SetLimitWidth(float fWidth);

#ifdef INSIDE_RENDER
			void SetFixedRenderPos(WORD startPos, WORD endPos) { m_TextInstance.SetFixedRenderPos(startPos, endPos); }
			void GetRenderPositions(WORD& startPos, WORD& endPos) { m_TextInstance.GetRenderPositions(startPos, endPos); }
			bool IsShowCursor();
			bool IsShow();
			int GetRenderingWidth();
			int GetRenderingHeight();
			void OnSetRenderingRect();
#endif

			void ShowCursor();
			void HideCursor();
			int GetCursorPosition();

			void SetText(const char * c_szText);
			const char * GetText();

			void GetTextSize(int* pnWidth, int* pnHeight);
#ifdef ENABLE_MULTI_TEXTLINE
			int GetTextLineCount();
			void DisableEnterToken();
			void SetLineHeight(int iHeight);
			int GetLineHeight();
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
			void GetCharSize(short* sWidth);
#endif
		protected:
			void OnUpdate();
			void OnRender();
			void OnChangePosition();

			virtual void OnSetText(const char * c_szText);

#ifdef INSIDE_RENDER
			void OnUpdateRenderBox() {
				UpdateTextLineRenderBox();
				m_TextInstance.SetRenderBox(m_renderBox);
			}
#endif

		protected:
			CGraphicTextInstance m_TextInstance;
	};

	class CNumberLine : public CWindow
	{
		public:
			CNumberLine(PyObject * ppyObject);
			CNumberLine(CWindow * pParent);
			virtual ~CNumberLine();

			void SetPath(const char * c_szPath);
			void SetHorizontalAlign(int iType);
			void SetNumber(const char * c_szNumber);
			const std::vector<CGraphicImageInstance*>& GetImageVector() const { return m_ImageInstanceVector; };

		protected:
			void ClearNumber();
			void OnRender();
			void OnChangePosition();

		protected:
			std::string m_strPath;
			std::string m_strNumber;
			std::vector<CGraphicImageInstance *> m_ImageInstanceVector;

			int m_iHorizontalAlign;
			DWORD m_dwWidthSummary;
	};

	// Image
	class CImageBox : public CWindow
	{
		public:
			CImageBox(PyObject * ppyObject);
			virtual ~CImageBox();

#ifdef INSIDE_RENDER
			void UnloadImage()
			{
				OnDestroyInstance();
				SetSize(GetWidth(), GetHeight());
				UpdateRect();
			}
#endif

			void LeftRightReverse();
			BOOL LoadImage(CGraphicImageInstance* imageInstance);
			BOOL LoadImage(const char * c_szFileName);
			void SetDiffuseColor(float fr, float fg, float fb, float fa);

			void Clear();
			int GetWidth();
			int GetHeight();
			void SetScale(float sx, float sy);
#ifdef ENABLE_NEW_TARGET_UI
			void ShowImage() { m_bIsShowImage = true; }
			void HideImage() { m_bIsShowImage = false; }
			void DisplayImageProcent(float procent) { m_fDisplayProcent = procent; }
#endif
#ifdef dracaryS_DUNGEON_LIB
			void SetCoolTime(float fCoolTime);
			void SetCoolTimeStart(float fCoolTimeStart);
			float m_fCoolTime;
			float m_fCoolTimeStart;
#endif
		protected:
			virtual void OnCreateInstance();
			virtual void OnDestroyInstance();

			virtual void OnUpdate();
			virtual void OnRender();
			void OnChangePosition();

		protected:
			CGraphicImageInstance * m_pImageInstance;
#ifdef ENABLE_NEW_TARGET_UI
			bool m_bIsShowImage;
			float m_fDisplayProcent;
#endif
	};
	class CMarkBox : public CWindow
	{
		public:
			CMarkBox(PyObject * ppyObject);
			virtual ~CMarkBox();

			void LoadImage(const char * c_szFilename);
			void SetDiffuseColor(float fr, float fg, float fb, float fa);
			void SetIndex(UINT uIndex);
			void SetScale(FLOAT fScale);

		protected:
			virtual void OnCreateInstance();
			virtual void OnDestroyInstance();

			virtual void OnUpdate();
			virtual void OnRender();
			void OnChangePosition();
		protected:
			CGraphicMarkInstance * m_pMarkInstance;
	};
	class CExpandedImageBox : public CImageBox
	{
		public:
			static DWORD Type();

		public:
			CExpandedImageBox(PyObject * ppyObject);
			virtual ~CExpandedImageBox();

			void SetScale(float fx, float fy);
			void SetOrigin(float fx, float fy);
			void SetRotation(float fRotation);
			void SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
			void SetRenderingMode(int iMode);
// #if defined(ENABLE_IMAGE_CLIP_RECT) || defined(ENABLE_BATTLE_PASS_SYSTEM)
			void SetImageClipRect(float fLeft, float fTop, float fRight, float fBottom, bool bIsVertical = false);
// #endif
#ifdef INSIDE_RENDER
			int GetRenderingWidth();
			int GetRenderingHeight();
			void OnSetRenderingRect();
			void SetExpandedRenderingRect(float fLeftTop, float fLeftBottom, float fTopLeft, float fTopRight, float fRightTop, float fRightBottom, float fBottomLeft, float fBottomRight);
			void SetTextureRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
			DWORD GetPixelColor(DWORD x, DWORD y);
			void OnUpdateRenderBox();
#endif

		protected:
			void OnCreateInstance();
			void OnDestroyInstance();

			virtual void OnUpdate();
			virtual void OnRender();

			BOOL OnIsType(DWORD dwType);
	};
	class CAniImageBox : public CWindow
	{
		public:
			static DWORD Type();

		public:
			explicit CAniImageBox(PyObject * ppyObject);
			virtual ~CAniImageBox();

			void SetDelay(int iDelay);
			#ifdef ENABLE_SASH_SYSTEM
			void	AppendImage(const char * c_szFileName, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0);
			#else
			void	AppendImage(const char * c_szFileName);
			#endif
			void SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
			void SetRenderingMode(int iMode);
			void AppendImageScale(const char * c_szFileName, float scale_x, float scale_y);
			void SetDiffuseColor(float r, float g, float b, float a);
			const std::vector<CGraphicExpandedImageInstance*>& GetImageVector() const { return m_ImageVector; };

#if defined(ENABLE_FISHING_GAME)
			void SetRotation(float fRotation);
#endif
			void ResetFrame();
			[[nodiscard]] size_t GetFrameIndex() const { return m_curIndex; }
			void ClearImages();
			void SetSkipCount(int iSkipCount);
			float GetColor(BYTE bIndex);
		protected:
			void OnUpdate() override;
			void OnRender() override;
			void OnChangePosition() override;
			virtual void OnEndFrame();
			virtual void OnNextFrame();
			BOOL OnIsType(DWORD dwType) override;

		protected:
			size_t m_curDelay;
			size_t m_delay;
			size_t m_curIndex;
			size_t m_SkipCount;
			float m_fScaleX;
			float m_fScaleY;
			float m_fColorR;
			float m_fColorG;
			float m_fColorB;
			float m_fColorA;

#if defined(ENABLE_FISHING_GAME)
			std::function<void(CGraphicExpandedImageInstance*)> m_SetRotation;
#endif
			std::vector<CGraphicExpandedImageInstance*> m_ImageVector;
			std::queue<std::string> m_ImageFileNames;
			std::function<void(CGraphicExpandedImageInstance*)> m_SetRenderingRect, m_SetRenderingMode, m_SetDiffuseColor, m_SetScale, m_SetRenderingRectWithScale;
			std::chrono::steady_clock::time_point m_lastUpdate = std::chrono::high_resolution_clock::now();

	};

	// Button
	class CButton : public CWindow
	{
		public:
			CButton(PyObject * ppyObject);
			virtual ~CButton();

			void LeftRightReverse();

			BOOL SetUpVisual(const char * c_szFileName);
			BOOL SetOverVisual(const char * c_szFileName);
			BOOL SetDownVisual(const char * c_szFileName);
			BOOL SetDisableVisual(const char * c_szFileName);
#ifdef ENABLE_NEW_GAMEOPTION
			static DWORD Type();
			void SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
#endif
			const char * GetUpVisualFileName();
			const char * GetOverVisualFileName();
			const char * GetDownVisualFileName();
			void FlashEx();

#ifdef ENABLE_NEW_PET_SYSTEM
			void Flash(bool bFlash);
#else
			void Flash();
#endif
			void Enable();
			void Disable();

			void SetUp();
			void Up();
			void Over();
			void Down();

			BOOL IsDisable();
			BOOL IsPressed();
#ifdef INSIDE_RENDER
			void OnSetRenderingRect();
			void OnUpdateRenderBox();
#endif

		protected:
			void OnUpdate();
			void OnRender();
			void OnChangePosition();

			BOOL OnMouseLeftButtonDown();
			BOOL OnMouseLeftButtonDoubleClick();
			BOOL OnMouseLeftButtonUp();
			void OnMouseOverIn();
			void OnMouseOverOut();

			BOOL IsEnable();

#ifdef ENABLE_NEW_GAMEOPTION
			void SetCurrentVisual(CGraphicExpandedImageInstance* pVisual);
			BOOL OnIsType(DWORD dwType);
#else
			void SetCurrentVisual(CGraphicImageInstance* pVisual);
#endif

		protected:
			BOOL m_bEnable;
			BOOL m_isPressed;
			BOOL m_isFlashEx;
			BOOL m_isFlash;
#ifdef ENABLE_NEW_GAMEOPTION
			CGraphicExpandedImageInstance* m_pcurVisual;
			CGraphicExpandedImageInstance m_upVisual;
			CGraphicExpandedImageInstance m_overVisual;
			CGraphicExpandedImageInstance m_downVisual;
			CGraphicExpandedImageInstance m_disableVisual;
#else
			CGraphicImageInstance* m_pcurVisual;
			CGraphicImageInstance m_upVisual;
			CGraphicImageInstance m_overVisual;
			CGraphicImageInstance m_downVisual;
			CGraphicImageInstance m_disableVisual;
#endif

#ifdef USE_CHEQUE_CURRENCY
		protected:
			float m_fScaleX;
			float m_fScaleY;

		public:
			void SetScale(float x, float y);
#endif
	};

	class CRadioButton : public CButton
	{
	public:
#ifdef ENABLE_NEW_GAMEOPTION
		static DWORD Type();
#endif
		CRadioButton(PyObject* ppyObject);
		virtual ~CRadioButton();

	protected:
		BOOL OnMouseLeftButtonDown();
		BOOL OnMouseLeftButtonUp();
		void OnMouseOverIn();
		void OnMouseOverOut();
#ifdef ENABLE_NEW_GAMEOPTION
		BOOL OnIsType(DWORD dwType);
#endif
	};
	class CToggleButton : public CButton
	{
	public:
#ifdef ENABLE_NEW_GAMEOPTION
		static DWORD Type();
#endif
		CToggleButton(PyObject* ppyObject);
		virtual ~CToggleButton();

	protected:
		BOOL OnMouseLeftButtonDown();
		BOOL OnMouseLeftButtonUp();
		void OnMouseOverIn();
		void OnMouseOverOut();
#ifdef ENABLE_NEW_GAMEOPTION
		BOOL OnIsType(DWORD dwType);
#endif
	};
	class CDragButton : public CButton
	{
		public:
			CDragButton(PyObject * ppyObject);
			virtual ~CDragButton();

			void SetRestrictMovementArea(int ix, int iy, int iwidth, int iheight);

		protected:
			void OnChangePosition();
			void OnMouseOverIn();
			void OnMouseOverOut();

		protected:
			RECT m_restrictArea;
	};
};

extern BOOL g_bOutlineBoxEnable;
