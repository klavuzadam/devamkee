#include "StdAfx.h"
#include "../eterBase/CRC32.h"
#include "PythonWindow.h"
#include "PythonSlotWindow.h"
#include "PythonWindowManager.h"
#include "../eterLib/CRenderTargetManager.h"

#ifdef ENABLE_ANIMATED_WINDOW
#include <directxmath/DirectXMath.h>
#include "../EterLib/StateManager.h"
#endif

BOOL g_bOutlineBoxEnable = FALSE;

#ifdef ENABLE_FISH_GAME
#include "../eterBase/Random.h"
#include <math.h>
#define _PI    ((float) 3.141592654f)
#define _1BYPI ((float) 0.318309886f)
#define DegreeToRadian( degree ) ((degree) * (_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / _PI))
typedef struct SVector
{
	float x;
	float y;
	float z;
} VECTOR;
inline float DISTANCE_SQRT(long dx, long dy)
{
	return ::sqrt((float)dx * dx + (float)dy * dy);
}
void Normalize(VECTOR* pV1, VECTOR* pV2)
{
	float l = sqrtf(pV1->x * pV1->x + pV1->y * pV1->y + pV1->z * pV1->z + 1.0e-12);
	pV2->x = pV1->x / l;
	pV2->y = pV1->y / l;
	pV2->z = pV1->z / l;
}
float DotProduct(VECTOR* pV1, VECTOR* pV2)
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}
float GetDegreeFromPositionEx(float x, float y)
{
	VECTOR	vtDir;
	VECTOR	vtStan;
	float	ret;
	vtDir.x = x;
	vtDir.y = y;
	vtDir.z = 0.0f;
	Normalize(&vtDir, &vtDir);
	vtStan.x = 0.0f;
	vtStan.y = 1.0f;
	vtStan.z = 0.0f;
	ret = RadianToDegree(acosf(DotProduct(&vtDir, &vtStan)));
	if (vtDir.x < 0.0f)
		ret = 360.0f - ret;
	return (ret);
}
float GetDegreeFromPositionXY(long sx, long sy, long ex, long ey)
{
	return GetDegreeFromPositionEx(ex - sx, ey - sy);
}
float GetDegreeDeltaEx(float iDegree, float iDegree2)
{
	if (iDegree > 180.0f)
		iDegree = iDegree - 360.0f;

	if (iDegree2 > 180.0f)
		iDegree2 = iDegree2 - 360.0f;

	return fabs(iDegree - iDegree2);
}
float GetDegreeFromPositionNew(float x, float y)
{
	D3DXVECTOR3 vtDir(floor(x), floor(y), 0.0f);
	D3DXVec3Normalize(&vtDir, &vtDir);

	D3DXVECTOR3 vtStan(0, -1, 0);
	float ret = D3DXToDegree(acosf(D3DXVec3Dot(&vtDir, &vtStan)));

	if (vtDir.x < 0.0f)
		ret = 360.0f - ret;

	return ret;
}
void GetDeltaByDegree(float fDegree, float fDistance, float* x, float* y)
{
	float fRadian = DegreeToRadian(fDegree);

	*x = fDistance * sin(fRadian);
	*y = fDistance * cos(fRadian);
}
#endif

namespace UI
{

	CWindow::CWindow(PyObject * ppyObject) :
		m_x(0),
		m_y(0),
		m_lWidth(0),
		m_lHeight(0),
		m_poHandler(ppyObject),
		m_bShow(false),
		m_pParent(NULL),
		m_dwFlag(0),
		m_isUpdatingChildren(FALSE)
#ifdef INSIDE_RENDER
		, m_isInsideRender(false)
#endif
#ifdef ENABLE_MOUSEWHEEL_EVENT
		, m_bIsScrollable(false)
#endif
		, m_bAnimation(false)
	{
#ifdef _DEBUG
		static DWORD DEBUG_dwGlobalCounter=0;
		DEBUG_dwCounter=DEBUG_dwGlobalCounter++;

		m_strName = "!!debug";
#endif
		//assert(m_poHandler != NULL);
		m_HorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		m_VerticalAlign = VERTICAL_ALIGN_TOP;
		m_rect.bottom = m_rect.left = m_rect.right = m_rect.top = 0;
		m_limitBiasRect.bottom = m_limitBiasRect.left = m_limitBiasRect.right = m_limitBiasRect.top = 0;
#ifdef INSIDE_RENDER
		memset(&m_renderBox, 0, sizeof(m_renderBox));
#endif
#ifdef ENABLE_ANIMATED_WINDOW
		D3DXMatrixIdentity(&m_matScaling);
#endif

#ifdef USE_CLIP_MASK
        m_bEnableMask = false;
        m_pMaskWindow = nullptr;
        memset(&m_rMaskRect, 0, sizeof(m_rMaskRect));
#endif
	}

	CWindow::~CWindow()
	{
	}

	DWORD CWindow::Type()
	{
		static DWORD s_dwType = GetCRC32("CWindow", strlen("CWindow"));
		return (s_dwType);
	}

	BOOL CWindow::IsType(DWORD dwType)
	{
		return OnIsType(dwType);
	}

	BOOL CWindow::OnIsType(DWORD dwType)
	{
		if (CWindow::Type() == dwType)
			return TRUE;

		return FALSE;
	}

	struct FClear
	{
		void operator () (CWindow * pWin)
		{
			pWin->Clear();
		}
	};

	void CWindow::Clear()
	{
#ifdef USE_CLIP_MASK
        m_pMaskWindow = nullptr;
#endif

		// FIXME : Children을 즉시 Delete하지는 않는다.
		//         어차피 Python쪽에서 Destroy가 하나씩 다시 호출 될 것이므로..
		//         하지만 만약을 위해 링크는 끊어 놓는다.
		//         더 좋은 형태는 있는가? - [levites]
		std::for_each(m_pChildList.begin(), m_pChildList.end(), FClear());
		m_pChildList.clear();

		m_pParent = NULL;
		DestroyHandle();
		Hide();
	}

	void CWindow::DestroyHandle()
	{
		m_poHandler = NULL;
	}

	void CWindow::Show(bool bForceShow)
	{
#ifdef ENABLE_ANIMATED_WINDOW
		if (IsFlag(FLAG_ANIMATED_BOARD) && !bForceShow)
		{
			if (!this->m_bShow)
			{
				this->m_bShow = true;
				m_bAnimation = true;
				m_sizeAnimation = tweeny::from(0.0f)
					.to(1.0f)
					.during(175)
					.via(tweeny::easing::cubicOut)
					.onStep([this](tweeny::tween<float>& t, float scale) {
					if (t.progress() == 0.0f)
					{
						SetScale(0.0f, 0.0f);
						//SetAllAlpha(0.0f);
					}

					SetScale(scale, scale);
					//SetAllAlpha(scale);

					if (t.progress() == 1.0f)
					{
						SetScale(1.0f, 1.0f);
						m_bAnimation = false;
						//SetAllAlpha(1.0f);

						return true;
					}
					else
					{
						return false;
					}
						});
				m_sizeAnimation.value().step(0.0f);
			}
			else
			{
				this->m_bShow = true;
				SetScale(1.0f, 1.0f);

				m_sizeAnimation.reset();
				m_bAnimation = false;
			}
		}
		else
		{
			m_bShow = true;
		}
#else
		m_bShow = true;
#endif
	}

	void CWindow::Hide(bool bForceHide)
	{
#ifdef ENABLE_ANIMATED_WINDOW
		if (IsFlag(FLAG_ANIMATED_BOARD) && !bForceHide)
		{
			if (m_bShow)
			{

				m_bAnimation = true;
				m_sizeAnimation = tweeny::from(1.0f)
					.to(0.0f)
					.during(120)
					.via(tweeny::easing::cubicOut)
					.onStep([this](tweeny::tween<float>& t, float scale) {
					if (t.progress() == 0.0f)
					{
						SetScale(1.0f, 1.0f);
						//SetAllAlpha(1.0f);
					}

					SetScale(scale, scale);
					//SetAllAlpha(scale);

					if (t.progress() == 1.0f)
					{
						SetScale(0.0f, 0.0f);
						//SetAllAlpha(0.0f);

						m_bShow = false;
						m_bAnimation = false;
						return true;
					}
					else
					{
						return false;
					}
						});
				m_sizeAnimation.value().step(0.0f);
			}
			else
			{
				m_bShow = false;

				m_sizeAnimation.reset();
				m_bAnimation = false;
			}
		}
		else
		{
			m_bShow = false;
		}
#else
		m_bShow = false;
#endif
	}

#ifdef ENABLE_ANIMATED_WINDOW
	void CWindow::SetScale(float fx, float fy)
	{
		m_v2Scale.x = fx;
		m_v2Scale.y = fy;
		UpdateRect();

		using namespace DirectX;

		XMFLOAT2 vCenter(m_x + (m_lWidth / 2), m_y + (m_lHeight / 2));
		XMStoreFloat4x4((XMFLOAT4X4*)&m_matScaling, XMMatrixTransformation2D(XMLoadFloat2((XMFLOAT2*)&vCenter), NULL, XMLoadFloat2((XMFLOAT2*)&m_v2Scale), XMLoadFloat2((XMFLOAT2*)&vCenter), NULL, XMVectorZero()));

	}
#endif

#ifdef INSIDE_RENDER
	void CWindow::OnHideWithChilds()
	{
		OnHide();
		std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::OnHideWithChilds));
	}
	void CWindow::OnHide()
	{
		PyCallClassMemberFunc(m_poHandler, "OnHide", BuildEmptyTuple());
	}
#endif

	// NOTE : IsShow는 "자신이 보이는가?" 이지만, __IsShowing은 "자신이 그려지고 있는가?" 를 체크한다
	//        자신은 Show 지만 Tree 위쪽의 Parent 중 하나는 Hide 일 수 있으므로.. - [levites]
	bool CWindow::IsRendering()
	{
		if (!IsShow())
			return false;

		if (!m_pParent)
			return true;
		

		return m_pParent->IsRendering();
	}

	void CWindow::__RemoveReserveChildren()
	{
		if (m_pReserveChildList.empty())
			return;

		TWindowContainer::iterator it;
		for(it = m_pReserveChildList.begin(); it != m_pReserveChildList.end(); ++it)
		{
			m_pChildList.remove(*it);
		}
		m_pReserveChildList.clear();
	}

	void CWindow::Update()
	{
		if (!IsShow())
			return;

		__RemoveReserveChildren();

#ifdef ENABLE_ANIMATED_WINDOW
		if (m_sizeAnimation && m_bAnimation)
			m_sizeAnimation.value().step(static_cast<uint32_t>(CTimer::instance().GetElapsedMilliecond()));
		
		if (!m_bAnimation)
			OnUpdate();
#else
		OnUpdate();
#endif
		m_isUpdatingChildren = TRUE;
		TWindowContainer::iterator it;
		for(it = m_pChildList.begin(); it != m_pChildList.end();)
		{
			TWindowContainer::iterator it_next = it;
			++it_next;
			(*it)->Update();
			it = it_next;
		}
		m_isUpdatingChildren = FALSE;
	}

#ifdef INSIDE_RENDER
	bool CWindow::IsShow()
	{
		if (!m_bShow)
			return false;

		if (m_isInsideRender)
			if (m_renderBox.left + m_renderBox.right >= m_lWidth || m_renderBox.top + m_renderBox.bottom >= m_lHeight)
				return false;

		return true;
	}
#endif

	void CWindow::Render()
	{
		if (!IsShow())
			return;

		OnRender();
#ifdef ENABLE_ANIMATED_WINDOW
		D3DXMATRIX currentMat;
		STATEMANAGER.GetTransform(D3DTS_WORLD, &currentMat);

		D3DXMATRIX newMat;
		D3DXMatrixMultiply(&newMat, &currentMat, &m_matScaling);
		STATEMANAGER.SetTransform(D3DTS_WORLD, &newMat);
#endif
		if (g_bOutlineBoxEnable)
		{
			CPythonGraphic::Instance().SetDiffuseColor(1.0f, 1.0f, 1.0f);
			CPythonGraphic::Instance().RenderBox2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}

		std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::Render));
#ifdef INSIDE_RENDER
		OnAfterRender();
#endif
#ifdef ENABLE_ANIMATED_WINDOW
		STATEMANAGER.SetTransform(D3DTS_WORLD, &currentMat);
#endif
	}

	void CWindow::OnUpdate()
	{
		if (!m_poHandler)
			return;

		if (!IsShow())
			return;

		static PyObject* poFuncName_OnUpdate = PyString_InternFromString("OnUpdate");

		//PyCallClassMemberFunc(m_poHandler, "OnUpdate", BuildEmptyTuple());
		PyCallClassMemberFunc_ByPyString(m_poHandler, poFuncName_OnUpdate, BuildEmptyTuple());

	}

	void CWindow::OnRender()
	{
		if (!m_poHandler)
			return;

		if (!IsShow())
			return;

		//PyCallClassMemberFunc(m_poHandler, "OnRender", BuildEmptyTuple());
		PyCallClassMemberFunc(m_poHandler, "OnRender", BuildEmptyTuple());
	}

#ifdef INSIDE_RENDER
	void CWindow::OnAfterRender()
	{
		if (!m_poHandler)
			return;
		if (!IsShow())
			return;
		PyCallClassMemberFunc(m_poHandler, "OnAfterRender", BuildEmptyTuple());
	}
#endif

	void CWindow::SetName(const char * c_szName)
	{
		m_strName = c_szName;
	}

	void CWindow::SetSize(long width, long height)
	{
		m_lWidth = width;
		m_lHeight = height;

		m_rect.right = m_rect.left + m_lWidth;
		m_rect.bottom = m_rect.top + m_lHeight;

#ifdef INSIDE_RENDER
		if (m_isInsideRender)
			UpdateRenderBoxRecursive();
#endif
	}

	void CWindow::SetHorizontalAlign(DWORD dwAlign)
	{
		m_HorizontalAlign = (EHorizontalAlign)dwAlign;
		UpdateRect();
#ifdef INSIDE_RENDER
		if (m_isInsideRender)
			UpdateRenderBoxRecursive();
#endif
	}

	void CWindow::SetVerticalAlign(DWORD dwAlign)
	{
		m_VerticalAlign = (EVerticalAlign)dwAlign;
		UpdateRect();
	}

	void CWindow::SetPosition(long x, long y)
	{
		m_x = x;
		m_y = y;

		UpdateRect();
#ifdef INSIDE_RENDER
		if (m_isInsideRender)
			UpdateRenderBoxRecursive();
#endif
	}

#ifdef INSIDE_RENDER
	void CWindow::UpdateRenderBoxRecursive()
	{
		UpdateRenderBox();
		for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
			(*it)->UpdateRenderBoxRecursive();
	}
#endif

	void CWindow::GetPosition(long * plx, long * ply)
	{
		*plx = m_x;
		*ply = m_y;
	}

	long CWindow::UpdateRect()
	{
		m_rect.top		= m_y;
		if (m_pParent)
		{
			switch (m_VerticalAlign)
			{
				case VERTICAL_ALIGN_BOTTOM:
					m_rect.top = m_pParent->GetHeight() - m_rect.top;
					break;
				case VERTICAL_ALIGN_CENTER:
					m_rect.top = (m_pParent->GetHeight() - GetHeight()) / 2 + m_rect.top;
					break;
			}
			m_rect.top += m_pParent->m_rect.top;
		}
		m_rect.bottom	= m_rect.top + m_lHeight;

#if defined( _USE_CPP_RTL_FLIP )
		if( m_pParent == NULL ) {
			m_rect.left		= m_x;
			m_rect.right	= m_rect.left + m_lWidth;
		} else {
			if( m_pParent->IsFlag(UI::CWindow::FLAG_RTL) == true ) {
				m_rect.left = m_pParent->GetWidth() - m_lWidth - m_x;
				switch (m_HorizontalAlign)
				{
					case HORIZONTAL_ALIGN_RIGHT:
						m_rect.left = - m_x;
						break;
					case HORIZONTAL_ALIGN_CENTER:
						m_rect.left = m_pParent->GetWidth() / 2 - GetWidth() - m_x;
						break;
				}
				m_rect.left += m_pParent->m_rect.left;
				m_rect.right = m_rect.left + m_lWidth;
			} else {
				m_rect.left		= m_x;
				switch (m_HorizontalAlign)
				{
					case HORIZONTAL_ALIGN_RIGHT:
						m_rect.left = m_pParent->GetWidth() - m_rect.left;
						break;
					case HORIZONTAL_ALIGN_CENTER:
						m_rect.left = (m_pParent->GetWidth() - GetWidth()) / 2 + m_rect.left;
						break;
				}
				m_rect.left += m_pParent->m_rect.left;
				m_rect.right = m_rect.left + m_lWidth;
			}
		}
#else
		m_rect.left		= m_x;
		if (m_pParent)
		{
			switch (m_HorizontalAlign)
			{
				case HORIZONTAL_ALIGN_RIGHT:
					m_rect.left = ::abs(m_pParent->GetWidth()) - m_rect.left;
					break;
				case HORIZONTAL_ALIGN_CENTER:
					m_rect.left = m_pParent->GetWidth() / 2 - GetWidth() / 2 + m_rect.left;
					break;
			}
			m_rect.left += 0L < m_pParent->GetWidth() ? m_pParent->m_rect.left : m_pParent->m_rect.right + ::abs(m_pParent->GetWidth());
		}
		m_rect.right = m_rect.left + m_lWidth;
#endif
		std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::UpdateRect));

		OnChangePosition();

		return 1;
	}

	void CWindow::GetLocalPosition(long & rlx, long & rly)
	{
		rlx = rlx - m_rect.left;
		rly = rly - m_rect.top;
	}

	void CWindow::GetMouseLocalPosition(long & rlx, long & rly)
	{
		CWindowManager::Instance().GetMousePosition(rlx, rly);
		rlx = rlx - m_rect.left;
		rly = rly - m_rect.top;
	}

	void CWindow::AddChild(CWindow * pWin)
	{
		m_pChildList.push_back(pWin);
		pWin->m_pParent = this;
#ifdef INSIDE_RENDER
		if (m_isInsideRender && !pWin->m_isInsideRender)
			pWin->SetInsideRender(m_isInsideRender);
#endif
	}

#ifdef INSIDE_RENDER
	void CWindow::SetInsideRender(BOOL flag)
	{
		if (!m_pParent || m_isInsideRender && m_pParent->m_isInsideRender)
			return;
		if (m_isInsideRender && flag)
			return;
		m_isInsideRender = flag;
		UpdateRenderBox();
		for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
			(*it)->SetInsideRender(m_isInsideRender);
	}
	void CWindow::GetRenderBox(RECT* box)
	{
		memcpy(box, &m_renderBox, sizeof(RECT));
	}
	void CWindow::UpdateTextLineRenderBox()
	{
		int width, height;
		((CTextLine*)this)->GetTextSize(&width, &height);

		int pWidth = m_pParent->GetWidth();
		int pHeight = m_pParent->GetHeight();

		if (m_x - m_pParent->m_renderBox.left < 0)
			m_renderBox.left = -m_x + m_pParent->m_renderBox.left;
		else
			m_renderBox.left = 0;

		if (m_y - m_pParent->m_renderBox.top < 0)
			m_renderBox.top = -m_y + m_pParent->m_renderBox.top;
		else
			m_renderBox.top = 0;

		if (m_x + width > pWidth - m_pParent->m_renderBox.right)
			m_renderBox.right = m_x + width - pWidth + m_pParent->m_renderBox.right;
		else
			m_renderBox.right = 0;

		if (m_y + height > pHeight - m_pParent->m_renderBox.bottom)
			m_renderBox.bottom = m_y + height - pHeight + m_pParent->m_renderBox.bottom;
		else
			m_renderBox.bottom = 0;
	}
	void CWindow::UpdateRenderBox()
	{
#ifdef __DUNGEON_INFO__
		if (std::string(GetName()) == "not_render_tooltip")
			return;
#endif

		if (!m_isInsideRender || !m_pParent)
			memset(&m_renderBox, 0, sizeof(m_renderBox));
		else
		{
			const int width = m_lWidth;
			const int height = m_lHeight;
			const int pWidth = m_pParent->GetWidth();
			const int pHeight = m_pParent->GetHeight();
			if (m_x - m_pParent->m_renderBox.left < 0)
				m_renderBox.left = -m_x + m_pParent->m_renderBox.left;
			else
				m_renderBox.left = 0;
			if (m_y - m_pParent->m_renderBox.top < 0)
				m_renderBox.top = -m_y + m_pParent->m_renderBox.top;
			else
				m_renderBox.top = 0;
			if (m_x + width > pWidth - m_pParent->m_renderBox.right)
				m_renderBox.right = m_x + width - pWidth + m_pParent->m_renderBox.right;
			else
				m_renderBox.right = 0;
			if (m_y + height > pHeight - m_pParent->m_renderBox.bottom)
				m_renderBox.bottom = m_y + height - pHeight + m_pParent->m_renderBox.bottom;
			else
				m_renderBox.bottom = 0;
		}
		OnUpdateRenderBox();
	}
#endif

	CWindow * CWindow::GetRoot()
	{
		if (m_pParent)
			if (m_pParent->IsWindow())
				return m_pParent->GetRoot();

		return this;
	}

	CWindow * CWindow::GetParent()
	{
		return m_pParent;
	}

#ifdef INSIDE_RENDER
	bool CWindow::IsChild(CWindow* pWin, bool bCheckRecursive)
#else
	bool CWindow::IsChild(CWindow * pWin)
#endif
	{
		std::list<CWindow *>::iterator itor = m_pChildList.begin();

		while (itor != m_pChildList.end())
		{
			if (*itor == pWin)
				return true;

#ifdef INSIDE_RENDER
			if (bCheckRecursive)
				if ((*itor)->IsChild(pWin, true))
					return true;
#endif

			++itor;
		}

		return false;
	}

	void CWindow::DeleteChild(CWindow * pWin)
	{
		if (m_isUpdatingChildren)
		{
			m_pReserveChildList.push_back(pWin);
		}
		else
		{
			m_pChildList.remove(pWin);
		}
	}

	void CWindow::SetTop(CWindow * pWin)
	{
		if (!pWin->IsFlag(CWindow::FLAG_FLOAT))
			return;

		TWindowContainer::iterator itor = std::find(m_pChildList.begin(), m_pChildList.end(), pWin);
		if (m_pChildList.end() != itor)
		{
			m_pChildList.push_back(*itor);
			m_pChildList.erase(itor);

			pWin->OnTop();
		}
		else
		{
			TraceError(" CWindow::SetTop - Failed to find child window\n");
		}
	}

	void CWindow::OnMouseDrag(long lx, long ly)
	{
		PyCallClassMemberFunc(m_poHandler, "OnMouseDrag", Py_BuildValue("(ii)", lx, ly));
	}

	void CWindow::OnMoveWindow(long lx, long ly)
	{
		PyCallClassMemberFunc(m_poHandler, "OnMoveWindow", Py_BuildValue("(ii)", lx, ly));
	}

	void CWindow::OnSetFocus()
	{
		//PyCallClassMemberFunc(m_poHandler, "OnSetFocus", BuildEmptyTuple());
		PyCallClassMemberFunc(m_poHandler, "OnSetFocus", BuildEmptyTuple());
	}

	void CWindow::OnKillFocus()
	{
		PyCallClassMemberFunc(m_poHandler, "OnKillFocus", BuildEmptyTuple());
	}

	void CWindow::OnMouseOverIn()
	{
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverIn", BuildEmptyTuple());
	}

	void CWindow::OnMouseOverOut()
	{
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverOut", BuildEmptyTuple());
	}

	void CWindow::OnMouseOver()
	{
	}

	void CWindow::OnDrop()
	{
		PyCallClassMemberFunc(m_poHandler, "OnDrop", BuildEmptyTuple());
	}

	void CWindow::OnTop()
	{
		PyCallClassMemberFunc(m_poHandler, "OnTop", BuildEmptyTuple());
	}

	void CWindow::OnIMEUpdate()
	{
		PyCallClassMemberFunc(m_poHandler, "OnIMEUpdate", BuildEmptyTuple());
	}

#ifdef USE_CLIP_MASK
    void CWindow::SetClippingMaskRect(const RECT& rMask)
    {
        m_bEnableMask = true;
        m_rMaskRect = rMask;

        TWindowContainer::const_reverse_iterator itor;
        for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
        {
            CWindow* pWindow = *itor;
            if (pWindow)
            {
                pWindow->SetClippingMaskRect(rMask);
            }
        }
    }

    void CWindow::SetClippingMaskWindow(CWindow* pMaskWindow)
    {
        if (!pMaskWindow)
        {
            return;
        }

        m_bEnableMask = true;
        m_pMaskWindow = pMaskWindow;

        TWindowContainer::const_reverse_iterator itor;
        for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
        {
            CWindow* pWindow = *itor;
            if (pWindow)
            {
                pWindow->SetClippingMaskWindow(pMaskWindow);
            }
        }
    }
#endif

	BOOL CWindow::RunIMETabEvent()
	{
		if (!IsRendering())
			return FALSE;

		if (OnIMETabEvent())
			return TRUE;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->RunIMETabEvent())
				return TRUE;
		}

		return FALSE;
	}

	BOOL CWindow::RunIMEReturnEvent()
	{
		if (!IsRendering())
			return FALSE;

		if (OnIMEReturnEvent())
			return TRUE;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->RunIMEReturnEvent())
				return TRUE;
		}

		return FALSE;
	}

	BOOL CWindow::RunIMEKeyDownEvent(int ikey)
	{
		if (!IsRendering())
			return FALSE;

		if (OnIMEKeyDownEvent(ikey))
			return TRUE;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->RunIMEKeyDownEvent(ikey))
				return TRUE;
		}

		return FALSE;
	}

	CWindow * CWindow::RunKeyDownEvent(int ikey)
	{
		if (OnKeyDown(ikey))
			return this;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->IsShow())
			{
				CWindow * pProcessedWindow = pWindow->RunKeyDownEvent(ikey);
				if (NULL != pProcessedWindow)
				{
					return pProcessedWindow;
				}
			}
		}

		return NULL;
	}

	BOOL CWindow::RunKeyUpEvent(int ikey)
	{
		if (OnKeyUp(ikey))
			return TRUE;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->IsShow())
			if (pWindow->RunKeyUpEvent(ikey))
				return TRUE;
		}

		return FALSE;
	}

	BOOL CWindow::RunPressEscapeKeyEvent()
	{
		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->IsShow())
			if (pWindow->RunPressEscapeKeyEvent())
				return TRUE;
		}

		if (OnPressEscapeKey())
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::RunPressExitKeyEvent()
	{
		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->RunPressExitKeyEvent())
				return TRUE;

			if (pWindow->IsShow())
			if (pWindow->OnPressExitKey())
				return TRUE;
		}

		return FALSE;
	}

	BOOL CWindow::OnMouseLeftButtonDown()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseLeftButtonDown", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseLeftButtonUp()
	{
		PyCallClassMemberFunc(m_poHandler, "OnMouseLeftButtonUp", BuildEmptyTuple());
		return TRUE; // NOTE : ButtonUp은 예외로 무조건 TRUE
	}

	BOOL CWindow::OnMouseLeftButtonDoubleClick()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseLeftButtonDoubleClick", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseRightButtonDown()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseRightButtonDown", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseRightButtonUp()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseRightButtonUp", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseRightButtonDoubleClick()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseRightButtonDoubleClick", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseMiddleButtonDown()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseMiddleButtonDown", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseMiddleButtonUp()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseMiddleButtonUp", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::RunMouseWheelEvent(long nLen)
	{
	    bool bValue = false;
	    if (PyCallClassMemberFunc(m_poHandler, "OnRunMouseWheel", Py_BuildValue("(l)", nLen), &bValue))
	        return bValue;
	    return bValue;
	}

	BOOL CWindow::OnIMETabEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMETab", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

#ifdef ENABLE_MOUSEWHEEL_EVENT
	BOOL CWindow::OnMouseWheel(int nLen)
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseWheel", Py_BuildValue("(i)", nLen), &lValue))
		{
			if (0 != lValue)
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL CWindow::OnMouseWheelScroll(short wDelta)
	{
#ifdef _DEBUG
		Tracenf("Mouse Wheel Scroll : wDelta %d ", wDelta);
#endif
		if (m_bIsScrollable) {
			PyCallClassMemberFunc(m_poHandler, "OnMouseWheelScroll", Py_BuildValue("(s)", wDelta > 0 ? "UP" : "DOWN"));
			return true;
		}

		return GetParent() && GetParent()->OnMouseWheelScroll(wDelta);
	}


	void CWindow::SetScrollable()
	{
		m_bIsScrollable = true;
	}
#endif

	BOOL CWindow::OnIMEReturnEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEReturn", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMEKeyDownEvent(int ikey)
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEKeyDown", Py_BuildValue("(i)", ikey), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMEChangeCodePage()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEChangeCodePage", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMEOpenCandidateListEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEOpenCandidateList", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMECloseCandidateListEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMECloseCandidateList", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMEOpenReadingWndEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEOpenReadingWnd", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMECloseReadingWndEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMECloseReadingWnd", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnKeyDown(int ikey)
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnKeyDown", Py_BuildValue("(i)", ikey), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnKeyUp(int ikey)
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnKeyUp", Py_BuildValue("(i)", ikey), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnPressEscapeKey()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnPressEscapeKey", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnPressExitKey()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnPressExitKey", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	/////

	bool CWindow::IsIn(long x, long y)
	{
		if (x >= m_rect.left && x <= m_rect.right)
			if (y >= m_rect.top && y <= m_rect.bottom)
				return true;

		return false;
	}

	bool CWindow::IsIn()
	{
		long lx, ly;
		UI::CWindowManager::Instance().GetMousePosition(lx, ly);

		return IsIn(lx, ly);
	}

	CWindow * CWindow::PickWindow(long x, long y)
	{
		std::list<CWindow *>::reverse_iterator ritor = m_pChildList.rbegin();
		for (; ritor != m_pChildList.rend(); ++ritor)
		{
			CWindow * pWin = *ritor;
			if (pWin->IsShow())
			{
				if (!pWin->IsFlag(CWindow::FLAG_IGNORE_SIZE))
				{
					if (!pWin->IsIn(x, y)) {
						if (0L <= pWin->GetWidth()) {
							continue;
						}
					}
				}

				CWindow * pResult = pWin->PickWindow(x, y);
				if (pResult)
					return pResult;
			}
		}

		if (IsFlag(CWindow::FLAG_NOT_PICK))
			return NULL;

		return (this);
	}

	CWindow * CWindow::PickTopWindow(long x, long y)
	{
		std::list<CWindow *>::reverse_iterator ritor = m_pChildList.rbegin();
		for (; ritor != m_pChildList.rend(); ++ritor)
		{
			CWindow * pWin = *ritor;
			if (pWin->IsShow())
				if (pWin->IsIn(x, y))
					if (!pWin->IsFlag(CWindow::FLAG_NOT_PICK))
						return pWin;
		}

		return NULL;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CUiRenderTarget::CUiRenderTarget(PyObject* ppyObject) : CWindow(ppyObject)
	{
		m_dwIndex = -1;
#ifdef ENABLE_WIKI
		memset(&rect_ex, 0, sizeof(rect_ex));
#endif
	}

	DWORD CUiRenderTarget::Type()
	{
		static DWORD s_dwType = GetCRC32("CUiRenderTarget", strlen("CUiRenderTarget"));
		return (s_dwType);
	}
	BOOL CUiRenderTarget::OnIsType(DWORD dwType)
	{
		if (CUiRenderTarget::Type() == dwType)
			return TRUE;

		return FALSE;
	}
	
	CUiRenderTarget::~CUiRenderTarget() {
		if (m_dwIndex != 1)
		{
			//TraceError("removing index %d", m_dwIndex);
			std::shared_ptr<CRenderTarget> t = CRenderTargetManager::Instance().GetRenderTarget(m_dwIndex);
			if (t)
			{
				t->ResetModel();
			}
		}
	};

	bool CUiRenderTarget::SetRenderTarget(int index)
	{
		if (!CRenderTargetManager::Instance().GetRenderTarget(index))
		{
			if (!CRenderTargetManager::Instance().CreateRenderTarget(index, GetWidth(), GetHeight()))
			{
				TraceError("CRenderTargetManager could not create the texture. w %d h %d", GetWidth(), GetHeight());
				return false;
			}
		}
		m_dwIndex = index;

		UpdateRect();
		return true;
	}

#ifdef ENABLE_WIKI
	void CUiRenderTarget::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		memset(&rect_ex, 0, sizeof(rect_ex));
		float fWidth = float(GetWidth());
		float fHeight = float(GetHeight());

		rect_ex.left = fWidth * fLeft;
		rect_ex.top = fHeight * fTop;
		rect_ex.right = fWidth * fRight;
		rect_ex.bottom = fHeight * fBottom;
	}
#endif

	void CUiRenderTarget::OnRender()
	{
		auto target = CRenderTargetManager::Instance().GetRenderTarget(m_dwIndex);
		if (!target)
		{
			TraceError("SetRenderingRect -> target empty!");
			return;
		}
#ifdef ENABLE_WIKI
		RECT rect_real;
		rect_real.left = m_rect.left - rect_ex.left;
		rect_real.top = m_rect.top - rect_ex.top;
		rect_real.right = m_rect.right + rect_ex.right;
		rect_real.bottom = m_rect.bottom + rect_ex.bottom;
		target->SetRenderingRect(&rect_real);
#else
		target->SetRenderingRect(&m_rect);
#endif
		target->RenderTexture();
	}

#ifdef INSIDE_RENDER
	void CWindow::iSetRenderingRect(int iLeft, int iTop, int iRight, int iBottom)
	{
		m_renderingRect.left = iLeft;
		m_renderingRect.top = iTop;
		m_renderingRect.right = iRight;
		m_renderingRect.bottom = iBottom;
		OnSetRenderingRect();
	}
	void CWindow::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		float fWidth = float(GetWidth());
		float fHeight = float(GetHeight());
		if (IsType(CTextLine::Type()))
		{
			int iWidth, iHeight;
			((CTextLine*)this)->GetTextSize(&iWidth, &iHeight);
			fWidth = float(iWidth);
			fHeight = float(iHeight);
		}
		iSetRenderingRect(fWidth * fLeft, fHeight * fTop, fWidth * fRight, fHeight * fBottom);
	}
	int CWindow::GetRenderingWidth()
	{
		return max(0, GetWidth() + m_renderingRect.right + m_renderingRect.left);
	}
	int CWindow::GetRenderingHeight()
	{
		return max(0, GetHeight() + m_renderingRect.bottom + m_renderingRect.top);
	}
	void CWindow::ResetRenderingRect(bool bCallEvent)
	{
		m_renderingRect.bottom = m_renderingRect.left = m_renderingRect.right = m_renderingRect.top = 0;
		if (bCallEvent)
			OnSetRenderingRect();
	}
	void CWindow::OnSetRenderingRect() {}
#endif

	CBox::CBox(PyObject * ppyObject) : CWindow(ppyObject), m_dwColor(0xff000000)
	{
	}
	CBox::~CBox()
	{
	}

	void CBox::SetColor(DWORD dwColor)
	{
		m_dwColor = dwColor;
	}

	void CBox::OnRender()
	{
		CPythonGraphic::Instance().SetDiffuseColor(m_dwColor);
#ifdef INSIDE_RENDER
		CPythonGraphic::Instance().RenderBar2d(m_rect.left + m_renderBox.left, m_rect.top + m_renderBox.top, m_rect.right - m_renderBox.right, m_rect.bottom - m_renderBox.bottom);
#else
		CPythonGraphic::Instance().RenderBox2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CBar::CBar(PyObject * ppyObject) : CWindow(ppyObject), m_dwColor(0xff000000)
	{
	}
	CBar::~CBar()
	{
	}

	void CBar::SetColor(DWORD dwColor)
	{
		m_dwColor = dwColor;
	}

	void CBar::OnRender()
	{
		CPythonGraphic::Instance().SetDiffuseColor(m_dwColor);
		CPythonGraphic::Instance().RenderBar2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CLine::CLine(PyObject * ppyObject) : CWindow(ppyObject), m_dwColor(0xff000000)
	{
	}
	CLine::~CLine()
	{
	}

	void CLine::SetColor(DWORD dwColor)
	{
		m_dwColor = dwColor;
	}

	void CLine::OnRender()
	{
		CPythonGraphic & rkpyGraphic = CPythonGraphic::Instance();
		rkpyGraphic.SetDiffuseColor(m_dwColor);
		rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	DWORD CBar3D::Type()
	{
		static DWORD s_dwType = GetCRC32("CBar3D", strlen("CBar3D"));
		return (s_dwType);
	}

	CBar3D::CBar3D(PyObject * ppyObject) : CWindow(ppyObject)
	{
		m_dwLeftColor = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		m_dwRightColor = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		m_dwCenterColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	CBar3D::~CBar3D()
	{
	}

	void CBar3D::SetColor(DWORD dwLeft, DWORD dwRight, DWORD dwCenter)
	{
		m_dwLeftColor = dwLeft;
		m_dwRightColor = dwRight;
		m_dwCenterColor = dwCenter;
	}

	void CBar3D::OnRender()
	{
		CPythonGraphic & rkpyGraphic = CPythonGraphic::Instance();

		rkpyGraphic.SetDiffuseColor(m_dwCenterColor);
		rkpyGraphic.RenderBar2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);

		rkpyGraphic.SetDiffuseColor(m_dwLeftColor);
		rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.right, m_rect.top);
		rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.left, m_rect.bottom);

		rkpyGraphic.SetDiffuseColor(m_dwRightColor);
		rkpyGraphic.RenderLine2d(m_rect.left, m_rect.bottom, m_rect.right, m_rect.bottom);
		rkpyGraphic.RenderLine2d(m_rect.right, m_rect.top, m_rect.right, m_rect.bottom);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CTextLine::CTextLine(PyObject * ppyObject) : CWindow(ppyObject)
	{
		m_TextInstance.SetColor(0.78f, 0.78f, 0.78f);
		m_TextInstance.SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_LEFT);
		m_TextInstance.SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_TOP);
	}
	CTextLine::~CTextLine()
	{
		m_TextInstance.Destroy();
	}

#ifdef INSIDE_RENDER
	DWORD CTextLine::Type()
	{
		static DWORD s_dwType = GetCRC32("CTextLine", strlen("CTextLine"));
		return (s_dwType);
	}
#endif

	void CTextLine::SetMax(int iMax)
	{
		m_TextInstance.SetMax(iMax);
	}
	void CTextLine::SetHorizontalAlign(int iType)
	{
		m_TextInstance.SetHorizonalAlign(iType);
	}
	void CTextLine::SetVerticalAlign(int iType)
	{
		m_TextInstance.SetVerticalAlign(iType);
	}
	void CTextLine::SetSecret(BOOL bFlag)
	{
		m_TextInstance.SetSecret(bFlag ? true : false);
	}
	void CTextLine::SetOutline(BOOL bFlag)
	{
		m_TextInstance.SetOutline(bFlag ? true : false);
	}
	void CTextLine::SetFeather(BOOL bFlag)
	{
		m_TextInstance.SetFeather(bFlag ? true : false);
	}
	void CTextLine::SetMultiLine(BOOL bFlag)
	{
		m_TextInstance.SetMultiLine(bFlag ? true : false);
	}
	void CTextLine::SetFontName(const char * c_szFontName)
	{
		std::string stFontName = c_szFontName;
		stFontName += ".fnt";

		CResourceManager& rkResMgr=CResourceManager::Instance();
		CResource* pkRes = rkResMgr.GetTypeResourcePointer(stFontName.c_str());
		CGraphicText* pkResFont=static_cast<CGraphicText*>(pkRes);
		m_TextInstance.SetTextPointer(pkResFont);
	}
	void CTextLine::SetFontColor(DWORD dwColor)
	{
		m_TextInstance.SetColor(dwColor);
	}
	void CTextLine::SetLimitWidth(float fWidth)
	{
		m_TextInstance.SetLimitWidth(fWidth);
	}
	void CTextLine::SetText(const char * c_szText)
	{
		OnSetText(c_szText);
	}
	void CTextLine::GetTextSize(int* pnWidth, int* pnHeight)
	{
		m_TextInstance.GetTextSize(pnWidth, pnHeight);
	}
#ifdef ENABLE_MULTI_TEXTLINE
	int CTextLine::GetTextLineCount()
	{
		return m_TextInstance.GetTextLineCount();
	}
	void CTextLine::DisableEnterToken()
	{
		m_TextInstance.DisableEnterToken();
	}
	void CTextLine::SetLineHeight(int iHeight)
	{
		m_TextInstance.SetLineHeight(iHeight);
	}
	int CTextLine::GetLineHeight()
	{
		return m_TextInstance.GetLineHeight();
	}
#endif
#ifdef ENABLE_SUNG_MAHI_TOWER
	void CTextLine::GetCharSize(short* sWidth)
	{
		m_TextInstance.GetCharacterWidth(sWidth);
	}
#endif
	const char * CTextLine::GetText()
	{
		return m_TextInstance.GetValueStringReference().c_str();
	}
	void CTextLine::ShowCursor()
	{
		m_TextInstance.ShowCursor();
	}
	void CTextLine::HideCursor()
	{
		m_TextInstance.HideCursor();
	}
#ifdef INSIDE_RENDER
	bool CTextLine::IsShowCursor()
	{
		return m_TextInstance.IsShowCursor();
	}
#endif
	int CTextLine::GetCursorPosition()
	{
		long lx, ly;
		CWindow::GetMouseLocalPosition(lx, ly);
		return m_TextInstance.PixelPositionToCharacterPosition(lx);
	}

	void CTextLine::OnSetText(const char * c_szText)
	{
		m_TextInstance.SetValue(c_szText);
		m_TextInstance.Update();
#ifdef INSIDE_RENDER
		if (m_isInsideRender)
			UpdateRenderBoxRecursive();
#endif
	}

    void CTextLine::OnUpdate()
    {
        if (IsShow())
        {
#ifdef USE_CLIP_MASK
            if (m_bEnableMask && m_pMaskWindow)
            {
                m_rMaskRect = m_pMaskWindow->GetRect();
            }
#endif

            m_TextInstance.Update();
        }
    }

    void CTextLine::OnRender()
    {
        if (IsShow())
        {
#ifdef USE_CLIP_MASK
            m_TextInstance.Render(m_bEnableMask ? &m_rMaskRect : nullptr);
#else
            m_TextInstance.Render();
#endif
        }
    }

#ifdef INSIDE_RENDER
	bool CTextLine::IsShow()
	{
		if (!m_bShow)
			return false;
		if (m_isInsideRender)
		{
			int cW, cH;
			GetTextSize(&cW, &cH);
			if (m_renderBox.left + m_renderBox.right >= cW || m_renderBox.top + m_renderBox.bottom >= cH)
				return false;
		}
		return true;
	}

	int CTextLine::GetRenderingWidth()
	{
		int iTextWidth, iTextHeight;
		GetTextSize(&iTextWidth, &iTextHeight);
		return iTextWidth + m_renderingRect.right + m_renderingRect.left;
	}
	int CTextLine::GetRenderingHeight()
	{
		int iTextWidth, iTextHeight;
		GetTextSize(&iTextWidth, &iTextHeight);
		return iTextHeight + m_renderingRect.bottom + m_renderingRect.top;
	}
	void CTextLine::OnSetRenderingRect()
	{
		int iTextWidth, iTextHeight;
		GetTextSize(&iTextWidth, &iTextHeight);
		m_TextInstance.iSetRenderingRect(m_renderingRect.left, -m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
	}
#endif

	void CTextLine::OnChangePosition()
	{
		// FOR_ARABIC_ALIGN
		//if (m_TextInstance.GetHorizontalAlign() == CGraphicTextInstance::HORIZONTAL_ALIGN_ARABIC)
		if( GetDefaultCodePage() == CP_ARABIC )
		{
			m_TextInstance.SetPosition(m_rect.right, m_rect.top);
		}
		else
		{
			m_TextInstance.SetPosition(m_rect.left, m_rect.top);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CNumberLine::CNumberLine(PyObject * ppyObject) : CWindow(ppyObject)
	{
		m_strPath = "d:/ymir work/ui/game/taskbar/";
		m_iHorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		m_dwWidthSummary = 0;
	}
	CNumberLine::CNumberLine(CWindow * pParent) : CWindow(NULL)
	{
		m_strPath = "d:/ymir work/ui/game/taskbar/";
		m_iHorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		m_dwWidthSummary = 0;

		m_pParent = pParent;
	}
	CNumberLine::~CNumberLine()
	{
		ClearNumber();
	}

	void CNumberLine::SetPath(const char * c_szPath)
	{
		m_strPath = c_szPath;
	}
	void CNumberLine::SetHorizontalAlign(int iType)
	{
		m_iHorizontalAlign = iType;
	}
	void CNumberLine::SetNumber(const char * c_szNumber)
	{
		if (0 == m_strNumber.compare(c_szNumber))
			return;

		ClearNumber();

		m_strNumber = c_szNumber;

		for (DWORD i = 0; i < m_strNumber.size(); ++i)
		{
			char cChar = m_strNumber[i];
			std::string strImageFileName;

			if (':' == cChar)
			{
				strImageFileName = m_strPath + "colon.sub";
			}
			else if ('?' == cChar)
			{
				strImageFileName = m_strPath + "questionmark.sub";
			}
			else if ('/' == cChar)
			{
				strImageFileName = m_strPath + "slash.sub";
			}
			else if ('%' == cChar)
			{
				strImageFileName = m_strPath + "percent.sub";
			}
			else if ('+' == cChar)
			{
				strImageFileName = m_strPath + "plus.sub";
			}
			else if ('m' == cChar)
			{
				strImageFileName = m_strPath + "m.sub";
			}
			else if ('g' == cChar)
			{
				strImageFileName = m_strPath + "g.sub";
			}
			else if ('p' == cChar)
			{
				strImageFileName = m_strPath + "p.sub";
			}
			else if (cChar >= '0' && cChar <= '9')
			{
				strImageFileName = m_strPath;
				strImageFileName += cChar;
				strImageFileName += ".sub";
			}
			else
				continue;

			if (!CResourceManager::Instance().IsFileExist(strImageFileName.c_str(), __FUNCTION__))
				continue;

			CGraphicImage * pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(strImageFileName.c_str());

			CGraphicImageInstance * pInstance = CGraphicImageInstance::New();
			pInstance->SetImagePointer(pImage);
			m_ImageInstanceVector.push_back(pInstance);

			m_dwWidthSummary += pInstance->GetWidth();
		}
	}

	void CNumberLine::ClearNumber()
	{
		m_ImageInstanceVector.clear();
		m_dwWidthSummary = 0;
		m_strNumber = "";
	}

	void CNumberLine::OnRender()
	{
		for (DWORD i = 0; i < m_ImageInstanceVector.size(); ++i)
		{
			m_ImageInstanceVector[i]->Render();
		}
	}

	void CNumberLine::OnChangePosition()
	{
		int ix = m_x;
		int iy = m_y;

		if (m_pParent)
		{
			ix = m_rect.left;
			iy = m_rect.top;
		}

		switch (m_iHorizontalAlign)
		{
			case HORIZONTAL_ALIGN_LEFT:
				break;
			case HORIZONTAL_ALIGN_CENTER:
				ix -= int(m_dwWidthSummary) / 2;
				break;
			case HORIZONTAL_ALIGN_RIGHT:
				ix -= int(m_dwWidthSummary);
				break;
		}

		for (DWORD i = 0; i < m_ImageInstanceVector.size(); ++i)
		{
			m_ImageInstanceVector[i]->SetPosition(ix, iy);
			ix += m_ImageInstanceVector[i]->GetWidth();
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CImageBox::CImageBox(PyObject * ppyObject) : CWindow(ppyObject)
#ifdef dracaryS_DUNGEON_LIB
		,m_fCoolTime(0.0f),
		m_fCoolTimeStart(0.0f)
#endif
	{
		m_pImageInstance = NULL;
#ifdef ENABLE_NEW_TARGET_UI
		m_bIsShowImage = true;
		m_fDisplayProcent = -1.0f;
#endif
	}
	CImageBox::~CImageBox()
	{
		OnDestroyInstance();
	}
#ifdef dracaryS_DUNGEON_LIB
	void CImageBox::SetCoolTime(float fCoolTime)
	{
		m_fCoolTime = fCoolTime;
	}

	void CImageBox::SetCoolTimeStart(float fCoolTimeStart)
	{
		m_fCoolTimeStart = fCoolTimeStart;
	}
#endif

	void CImageBox::LeftRightReverse()
	{
		if (!m_pImageInstance)
			return;

		m_pImageInstance->LeftRightReverse();
	}

	void CImageBox::Clear()
	{
		OnDestroyInstance();
	}

	void CImageBox::OnCreateInstance()
	{
		OnDestroyInstance();

		m_pImageInstance = CGraphicImageInstance::New();
	}
	void CImageBox::OnDestroyInstance()
	{
		if (m_pImageInstance)
		{
			CGraphicImageInstance::Delete(m_pImageInstance);
			m_pImageInstance=NULL;
		}
	}

	BOOL CImageBox::LoadImage(CGraphicImageInstance* imageInstance)
	{
		if (!imageInstance)
			return FALSE;
		OnCreateInstance();
		m_pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(imageInstance->GetGraphicImagePointer()));
		if (m_pImageInstance->IsEmpty())
			return FALSE;
		SetSize(m_pImageInstance->GetWidth(), m_pImageInstance->GetHeight());
		UpdateRect();
		CGraphicImageInstance::Delete(imageInstance);
		return TRUE;
	}

	BOOL CImageBox::LoadImage(const char * c_szFileName)
	{
		if (!c_szFileName[0])
			return FALSE;

		OnCreateInstance();

		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource)
			return FALSE;
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_pImageInstance->IsEmpty())
			return FALSE;

		SetSize(m_pImageInstance->GetWidth(), m_pImageInstance->GetHeight());
		UpdateRect();

		return TRUE;
	}

	void CImageBox::SetDiffuseColor(float fr, float fg, float fb, float fa)
	{
		if (!m_pImageInstance)
			return;

		m_pImageInstance->SetDiffuseColor(fr, fg, fb, fa);
	}
	void CImageBox::SetScale(float sx, float sy)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicImageInstance*)m_pImageInstance)->SetScale(sx, sy);
		CWindow::SetSize(long(float(GetWidth())*sx), long(float(GetHeight())*sy));
	}
	int CImageBox::GetWidth()
	{
		if (!m_pImageInstance)
			return 0;

		return m_pImageInstance->GetWidth();
	}

	int CImageBox::GetHeight()
	{
		if (!m_pImageInstance)
			return 0;

		return m_pImageInstance->GetHeight();
	}

    void CImageBox::OnUpdate()
    {
#ifdef USE_CLIP_MASK
        if (!m_pImageInstance)
        {
            return;
        }

        if (!IsShow())
        {
            return;
        }

        if (!m_bEnableMask)
        {
            return;
        }

        if (!m_pMaskWindow)
        {
            return;
        }

        m_rMaskRect = m_pMaskWindow->GetRect();
#endif
    }

    void CImageBox::OnRender()
    {
        if (!m_pImageInstance)
        {
            return;
        }

#ifdef ENABLE_NEW_TARGET_UI
		if (IsShow() && m_bIsShowImage)
		{
			if (m_fDisplayProcent != -1.0f)
				m_pImageInstance->DisplayImageProcent(m_fDisplayProcent);
#ifdef dracaryS_DUNGEON_LIB
            else if (m_fCoolTime == 0.0f)
            {
#endif
#ifdef USE_CLIP_MASK
            m_pImageInstance->Render(m_bEnableMask ? &m_rMaskRect : nullptr);
#else
            m_pImageInstance->Render();
#endif
#ifdef dracaryS_DUNGEON_LIB
            }
            else
            {
#ifdef USE_CLIP_MASK
                m_pImageInstance->RenderCoolTime((CTimer::Instance().GetCurrentSecond() - m_fCoolTimeStart) / m_fCoolTime, m_bEnableMask ? &m_rMaskRect : nullptr);
#else
                m_pImageInstance->RenderCoolTime((CTimer::Instance().GetCurrentSecond() - m_fCoolTimeStart) / m_fCoolTime);
#endif
            }
#endif
		}
#else
		if (IsShow())
		{
#ifdef dracaryS_DUNGEON_LIB
            if (m_fCoolTime == 0.0f)
            {
#endif
#ifdef USE_CLIP_MASK
            m_pImageInstance->Render(m_bEnableMask ? &m_rMaskRect : nullptr);
#else
            m_pImageInstance->Render();
#endif
#ifdef dracaryS_DUNGEON_LIB
            }
            else
            {
#ifdef USE_CLIP_MASK
                m_pImageInstance->RenderCoolTime((CTimer::Instance().GetCurrentSecond() - m_fCoolTimeStart) / m_fCoolTime, m_bEnableMask ? &m_rMaskRect : nullptr);
#else
                m_pImageInstance->RenderCoolTime((CTimer::Instance().GetCurrentSecond() - m_fCoolTimeStart) / m_fCoolTime);
#endif
            }
#endif
		}
#endif
    }

	void CImageBox::OnChangePosition()
	{
		if (!m_pImageInstance)
			return;

		m_pImageInstance->SetPosition(m_rect.left, m_rect.top);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MarkBox - 마크 출력용 UI 윈도우
	///////////////////////////////////////////////////////////////////////////////////////////////
	CMarkBox::CMarkBox(PyObject * ppyObject) : CWindow(ppyObject)
	{
		m_pMarkInstance = NULL;
	}

	CMarkBox::~CMarkBox()
	{
		OnDestroyInstance();
	}

	void CMarkBox::OnCreateInstance()
	{
		OnDestroyInstance();
		m_pMarkInstance = CGraphicMarkInstance::New();
	}

	void CMarkBox::OnDestroyInstance()
	{
		if (m_pMarkInstance)
		{
			CGraphicMarkInstance::Delete(m_pMarkInstance);
			m_pMarkInstance=NULL;
		}
	}

	void CMarkBox::LoadImage(const char * c_szFilename)
	{
		OnCreateInstance();

		m_pMarkInstance->SetImageFileName(c_szFilename);
		m_pMarkInstance->Load();
		SetSize(m_pMarkInstance->GetWidth(), m_pMarkInstance->GetHeight());

		UpdateRect();
	}

	void CMarkBox::SetScale(FLOAT fScale)
	{
		if (!m_pMarkInstance)
			return;

		m_pMarkInstance->SetScale(fScale);
	}

	void CMarkBox::SetIndex(UINT uIndex)
	{
		if (!m_pMarkInstance)
			return;

		m_pMarkInstance->SetIndex(uIndex);
	}

	void CMarkBox::SetDiffuseColor(float fr, float fg, float fb, float fa)
	{
		if (!m_pMarkInstance)
			return;

		m_pMarkInstance->SetDiffuseColor(fr, fg, fb, fa);
	}

	void CMarkBox::OnUpdate()
	{
	}

	void CMarkBox::OnRender()
	{
		if (!m_pMarkInstance)
			return;

		if (IsShow())
			m_pMarkInstance->Render();
	}

	void CMarkBox::OnChangePosition()
	{
		if (!m_pMarkInstance)
			return;

		m_pMarkInstance->SetPosition(m_rect.left, m_rect.top);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	DWORD CExpandedImageBox::Type()
	{
		static DWORD s_dwType = GetCRC32("CExpandedImageBox", strlen("CExpandedImageBox"));
		return (s_dwType);
	}

	BOOL CExpandedImageBox::OnIsType(DWORD dwType)
	{
		if (CExpandedImageBox::Type() == dwType)
			return TRUE;

		return FALSE;
	}

	CExpandedImageBox::CExpandedImageBox(PyObject * ppyObject) : CImageBox(ppyObject)
	{
	}
	CExpandedImageBox::~CExpandedImageBox()
	{
		OnDestroyInstance();
	}

	void CExpandedImageBox::OnCreateInstance()
	{
		OnDestroyInstance();

		m_pImageInstance = CGraphicExpandedImageInstance::New();
	}
	void CExpandedImageBox::OnDestroyInstance()
	{
		if (m_pImageInstance)
		{
			CGraphicExpandedImageInstance::Delete((CGraphicExpandedImageInstance*)m_pImageInstance);
			m_pImageInstance=NULL;
		}
	}

	void CExpandedImageBox::SetScale(float fx, float fy)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetScale(fx, fy);
		CWindow::SetSize(long(float(GetWidth())*fx), long(float(GetHeight())*fy));
	}
	void CExpandedImageBox::SetOrigin(float fx, float fy)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetOrigin(fx, fy);
	}
	void CExpandedImageBox::SetRotation(float fRotation)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRotation(fRotation);
	}
	void CExpandedImageBox::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderingRect(fLeft, fTop, fRight, fBottom);
	}

// #if defined(ENABLE_IMAGE_CLIP_RECT) || defined(ENABLE_BATTLE_PASS_SYSTEM)
	void CExpandedImageBox::SetImageClipRect(float fLeft, float fTop, float fRight, float fBottom, bool bIsVertical)
	{
		if (!m_pImageInstance)
			return;

		const RECT& c_rRect = GetRect();

		float fDifLeft = (c_rRect.left < fLeft) ? -(float(fLeft - c_rRect.left) / float(GetWidth())) : 0.0f;
		float fDifTop = (c_rRect.top < fTop) ? -(float(fTop - c_rRect.top) / float(GetHeight())) : 0.0f;
		float fDifRight = (c_rRect.right > fRight) ? -(float(c_rRect.right - fRight) / float(GetWidth())) : 0.0f;
		float fDifBottom = (c_rRect.bottom > fBottom) ? -(float(c_rRect.bottom - fBottom) / float(GetHeight())) : 0.0f;

		if (bIsVertical)
			((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderingRect(fLeft, fDifTop, fRight, fDifBottom);
		else
			((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderingRect(fDifLeft, fDifTop, fDifRight, fDifBottom);
	}
// #endif
	
	void CExpandedImageBox::SetRenderingMode(int iMode)
	{
		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderingMode(iMode);
	}

#ifdef INSIDE_RENDER
	int CExpandedImageBox::GetRenderingWidth()
	{
		return CWindow::GetWidth() + m_renderingRect.right + m_renderingRect.left;
	}
	int CExpandedImageBox::GetRenderingHeight()
	{
		return CWindow::GetHeight() + m_renderingRect.bottom + m_renderingRect.top;
	}
	void CExpandedImageBox::OnSetRenderingRect()
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
	}
	void CExpandedImageBox::SetExpandedRenderingRect(float fLeftTop, float fLeftBottom, float fTopLeft, float fTopRight, float fRightTop, float fRightBottom, float fBottomLeft, float fBottomRight)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetExpandedRenderingRect(fLeftTop, fLeftBottom, fTopLeft, fTopRight, fRightTop, fRightBottom, fBottomLeft, fBottomRight);
	}
	void CExpandedImageBox::SetTextureRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetTextureRenderingRect(fLeft, fTop, fRight, fBottom);
	}
	DWORD CExpandedImageBox::GetPixelColor(DWORD x, DWORD y)
	{
		return ((CGraphicExpandedImageInstance*)m_pImageInstance)->GetPixelColor(x, y);
	}
	void CExpandedImageBox::OnUpdateRenderBox()
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderBox(m_renderBox);
	}
#endif

	void CExpandedImageBox::OnUpdate()
	{
#ifdef __DUNGEON_INFO__
		static PyObject* poFuncName_OnUpdate = PyString_InternFromString("OnUpdate");
		PyCallClassMemberFunc_ByPyString(m_poHandler, poFuncName_OnUpdate, BuildEmptyTuple());
#endif
	}
	void CExpandedImageBox::OnRender()
	{
		if (!m_pImageInstance)
			return;

		if (IsShow())
#ifdef dracaryS_DUNGEON_LIB
		{
			if (m_fCoolTime == 0.0f)
				m_pImageInstance->Render();
			else
				((CGraphicExpandedImageInstance*)m_pImageInstance)->RenderCoolTime((CTimer::Instance().GetCurrentSecond() - m_fCoolTimeStart) / m_fCoolTime);
		}
#else
			m_pImageInstance->Render();
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	DWORD CAniImageBox::Type()
	{
		static DWORD s_dwType = GetCRC32("CAniImageBox", strlen("CAniImageBox"));
		return (s_dwType);
	}

	BOOL CAniImageBox::OnIsType(DWORD dwType)
	{
		if (CAniImageBox::Type() == dwType)
			return TRUE;

		return FALSE;
	}

	CAniImageBox::CAniImageBox(PyObject* ppyObject)
		: CWindow(ppyObject),
		m_curDelay(0),
		m_delay(4),
		m_curIndex(0),
		m_SkipCount(0),
		m_fScaleX(1), m_fScaleY(1)
		,m_fColorR(1.0f)
		,m_fColorG(1.0f)
		,m_fColorB(1.0f)
		,m_fColorA(1.0f)
	{
		m_ImageVector.clear();
	}

	CAniImageBox::~CAniImageBox()
	{
		ClearImages();
	}

	void CAniImageBox::SetDelay(int iDelay)
	{
		m_delay = iDelay;
	}

	float CAniImageBox::GetColor(BYTE bIndex)
	{
		if (bIndex == 0)
			return m_fColorR;
		else if (bIndex == 1)
			return m_fColorG;
		else if (bIndex == 2)
			return m_fColorB;
		else if (bIndex == 3)
			return m_fColorA;

		return 1.0f;
	}

	#ifdef ENABLE_SASH_SYSTEM
	void CAniImageBox::AppendImage(const char * c_szFileName, float r, float g, float b, float a)
	#else
	void CAniImageBox::AppendImage(const char * c_szFileName)
	#endif
	{
		m_fColorR = r;
		m_fColorG = g;
		m_fColorB = b;
		m_fColorA = a;
		m_ImageFileNames.push(c_szFileName);

	}
	void CAniImageBox::AppendImageScale(const char * c_szFileName, float scale_x, float scale_y)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return;

		CGraphicExpandedImageInstance * pImageInstance = CGraphicExpandedImageInstance::New();

		pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(pResource));
		pImageInstance->SetScale(scale_x, scale_y);
		if (pImageInstance->IsEmpty())
		{
			CGraphicExpandedImageInstance::Delete(pImageInstance);
			return;
		}

		m_ImageVector.push_back(pImageInstance);

		m_curIndex = rand() % m_ImageVector.size();
		//		SetSize(pImageInstance->GetWidth(), pImageInstance->GetHeight());
		//		UpdateRect();
	}
	struct FSetRenderingRect
	{
		float fLeft, fTop, fRight, fBottom;
		void operator () (CGraphicExpandedImageInstance * pInstance)
		{
			pInstance->SetRenderingRect(fLeft, fTop, fRight, fBottom);
		}
	};

	void CAniImageBox::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		FSetRenderingRect setRenderingRect;
		setRenderingRect.fLeft = fLeft;
		setRenderingRect.fTop = fTop;
		setRenderingRect.fRight = fRight;
		setRenderingRect.fBottom = fBottom;
		for_each(m_ImageVector.begin(), m_ImageVector.end(), setRenderingRect);
	}

	struct FSetRenderingMode
	{
		int iMode;
		void operator () (CGraphicExpandedImageInstance * pInstance)
		{
			pInstance->SetRenderingMode(iMode);
		}
	};
	void CAniImageBox::SetRenderingMode(int iMode)
	{
		FSetRenderingMode setRenderingMode;
		setRenderingMode.iMode = iMode;
		for_each(m_ImageVector.begin(), m_ImageVector.end(), setRenderingMode);
	}
	
	void CAniImageBox::ResetFrame()
	{
		m_curIndex = 0;
	}

	void CAniImageBox::ClearImages()
	{
		std::for_each(m_ImageVector.begin(), m_ImageVector.end(), [](CGraphicExpandedImageInstance* image) { CGraphicExpandedImageInstance::DeleteExpandedImageInstance(image); });
		m_ImageVector.clear();
	}

	void CAniImageBox::SetSkipCount(int iSkipCount)
	{
		m_SkipCount = iSkipCount;
	}

	void CAniImageBox::OnUpdate()
	{
		if (!m_ImageFileNames.empty())
		{
			const std::string& stFileName = m_ImageFileNames.front();

			if (CResource* pResource = CResourceManager::Instance().GetResourcePointer(stFileName.c_str()); pResource->IsType(CGraphicImage::Type()))
			{
				const auto now = std::chrono::high_resolution_clock::now();

				if (std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastUpdate).count() < 100)
					return;

				m_lastUpdate = now;

				CGraphicExpandedImageInstance* pImageInstance = CGraphicExpandedImageInstance::New();
				pImageInstance->SetImagePointer(dynamic_cast<CGraphicImage*>(pResource));

				if (pImageInstance->IsEmpty())
				{
					CGraphicExpandedImageInstance::Delete(pImageInstance);
				}
				else
				{
					pImageInstance->SetPosition(m_rect.left, m_rect.top);
					pImageInstance->SetDiffuseColor(GetColor(0), GetColor(1), GetColor(2), GetColor(3));
					if (m_SetRenderingRect)
						m_SetRenderingRect(pImageInstance);

					if (m_SetRenderingMode)
						m_SetRenderingMode(pImageInstance);

					if (m_SetDiffuseColor)
						m_SetDiffuseColor(pImageInstance);
					
#if defined(ENABLE_FISHING_GAME)
					if (m_SetRotation)
						m_SetRotation(pImageInstance);
#endif

					if (m_SetScale)
						m_SetScale(pImageInstance);

					// if (m_SetRenderingRectWithScale)
						// m_SetRenderingRectWithScale(pImageInstance);

					m_ImageVector.push_back(pImageInstance);
				}
			}

			m_ImageFileNames.pop();
		}

		++m_curDelay;
		if (m_curDelay < m_delay)
			return;

		m_curDelay = 0;

		m_curIndex += 1 + m_SkipCount;
		OnNextFrame();

		if (m_curIndex >= m_ImageVector.size())
		{
			m_curIndex = 0;

			OnEndFrame();
		}
	}

#if defined(ENABLE_FISHING_GAME)
	void CAniImageBox::SetRotation(float fRotation)
	{
		m_SetRotation = std::bind(&CGraphicExpandedImageInstance::SetRotation, std::placeholders::_1, fRotation);
		std::for_each(m_ImageVector.begin(), m_ImageVector.end(), m_SetRotation);
	}
#endif

	struct FSetDiffuseColor
	{
		float r, g, b, a;
		void operator () (CGraphicExpandedImageInstance * pInstance)
		{
			pInstance->SetDiffuseColor(r, g, b, a);
		}
	};

	void CAniImageBox::SetDiffuseColor(float r, float g, float b, float a) {
		FSetDiffuseColor SetDiffuseColor;
		SetDiffuseColor.r = r;
		SetDiffuseColor.g = g;
		SetDiffuseColor.b = b;
		SetDiffuseColor.a = a;

		for_each(m_ImageVector.begin(), m_ImageVector.end(), SetDiffuseColor);
	}

	void CAniImageBox::OnRender()
	{
		if (!IsShow())
			return;

		if (m_curIndex < m_ImageVector.size())
		{
			CGraphicExpandedImageInstance * pImage = m_ImageVector[m_curIndex];
			pImage->Render();
		}
	}

	void CAniImageBox::OnChangePosition()
	{
		std::for_each(m_ImageVector.begin(), m_ImageVector.end(), [this](CGraphicExpandedImageInstance* image) { image->SetPosition(static_cast<float>(m_rect.left), static_cast<float>(m_rect.top)); });
	}
	
	void CAniImageBox::OnEndFrame()
	{
		PyCallClassMemberFunc(m_poHandler, "OnEndFrame", BuildEmptyTuple());
	}

	void CAniImageBox::OnNextFrame()
	{
		PyCallClassMemberFunc(m_poHandler, "OnNextFrame", BuildEmptyTuple());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CButton::CButton(PyObject * ppyObject)
		:	CWindow(ppyObject),
			m_pcurVisual(NULL),
			m_bEnable(TRUE),
			m_isPressed(FALSE),
			m_isFlash(FALSE),
			m_isFlashEx(FALSE)
#ifdef USE_CHEQUE_CURRENCY
, m_fScaleX(1.0f), m_fScaleY(1.0f)
#endif
	{
		CWindow::AddFlag(CWindow::FLAG_NOT_CAPTURE);
	}

	void CButton::LeftRightReverse()
	{
		m_upVisual.LeftRightReverse();
		m_overVisual.LeftRightReverse();
		m_downVisual.LeftRightReverse();
		m_disableVisual.LeftRightReverse();
	}

	CButton::~CButton()
	{
	}

	BOOL CButton::SetUpVisual(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_upVisual.SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_upVisual.IsEmpty())
			return FALSE;

		SetSize(m_upVisual.GetWidth(), m_upVisual.GetHeight());
		//
		SetCurrentVisual(&m_upVisual);
		//

		return TRUE;
	}
	BOOL CButton::SetOverVisual(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_overVisual.SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_overVisual.IsEmpty())
			return FALSE;

		SetSize(m_overVisual.GetWidth(), m_overVisual.GetHeight());

		return TRUE;
	}
	BOOL CButton::SetDownVisual(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_downVisual.SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_downVisual.IsEmpty())
			return FALSE;

		SetSize(m_downVisual.GetWidth(), m_downVisual.GetHeight());

		return TRUE;
	}
	BOOL CButton::SetDisableVisual(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_disableVisual.SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_downVisual.IsEmpty())
			return FALSE;

		SetSize(m_disableVisual.GetWidth(), m_disableVisual.GetHeight());

		return TRUE;
	}

	const char * CButton::GetUpVisualFileName()
	{
		return m_upVisual.GetGraphicImagePointer()->GetFileName();
	}
	const char * CButton::GetOverVisualFileName()
	{
		return m_overVisual.GetGraphicImagePointer()->GetFileName();
	}
	const char * CButton::GetDownVisualFileName()
	{
		return m_downVisual.GetGraphicImagePointer()->GetFileName();
	}

#ifdef ENABLE_NEW_PET_SYSTEM
	void CButton::Flash(bool bFlash)
	{
		m_isFlash = bFlash;
	}
#else
	void CButton::Flash()
	{
		m_isFlash = TRUE;
	}
#endif

	void CButton::Enable()
	{
		if (!IsDisable()) {
			SetUp();
			m_bEnable = TRUE;
		}
	}
	void CButton::FlashEx()
	{
		m_isFlashEx = TRUE;
	}
	void CButton::Disable()
	{
		if (IsDisable()) {
			m_bEnable = FALSE;
			if (!m_disableVisual.IsEmpty())
				SetCurrentVisual(&m_disableVisual);
		}
	}

	BOOL CButton::IsDisable()
	{
		return m_bEnable;
	}

	void CButton::SetUp()
	{
		SetCurrentVisual(&m_upVisual);
		m_isPressed = FALSE;
	}
	void CButton::Up()
	{
		if (IsIn())
			SetCurrentVisual(&m_overVisual);
		else
			SetCurrentVisual(&m_upVisual);

		PyCallClassMemberFunc(m_poHandler, "CallEvent", BuildEmptyTuple());
	}
	void CButton::Over()
	{
		SetCurrentVisual(&m_overVisual);
	}
	void CButton::Down()
	{
		m_isPressed = TRUE;
		SetCurrentVisual(&m_downVisual);
		PyCallClassMemberFunc(m_poHandler, "DownEvent", BuildEmptyTuple());
	}

    void CButton::OnUpdate()
    {
#ifdef USE_CLIP_MASK
        if (!m_bEnableMask)
        {
            return;
        }

        if (!m_pMaskWindow)
        {
            return;
        }

        m_rMaskRect = m_pMaskWindow->GetRect();
#endif
    }

    void CButton::OnRender()
    {
        if (!IsShow())
        {
            return;
        }

        if (m_pcurVisual)
        {
            if (m_isFlash && !IsIn())
            {
                if (int(timeGetTime() / 500) % 2)
                {
                    return;
                }
            }

            if (m_isFlashEx && !IsIn())
            {
                if (int(timeGetTime() / 500) % 2)
                {
                    Over();
                }

                if (int(timeGetTime() / (500 - 15)) % 2)
                {
                    Down();
                }
            }

#ifdef USE_CLIP_MASK
            m_pcurVisual->Render(m_bEnableMask ? &m_rMaskRect : nullptr);
#else
            m_pcurVisual->Render();
#endif
        }

        PyCallClassMemberFunc(m_poHandler, "OnRender", BuildEmptyTuple());
    }

	void CButton::OnChangePosition()
	{
		if (m_pcurVisual)
			m_pcurVisual->SetPosition(m_rect.left, m_rect.top);
	}

	BOOL CButton::OnMouseLeftButtonDown()
	{
		if (!IsEnable())
			return TRUE;

		m_isPressed = TRUE;
		Down();

		return TRUE;
	}
	BOOL CButton::OnMouseLeftButtonDoubleClick()
	{
		if (!IsEnable())
			return TRUE;

		OnMouseLeftButtonDown();

		return TRUE;
	}
	BOOL CButton::OnMouseLeftButtonUp()
	{
		if (!IsEnable())
			return TRUE;
		if (!IsPressed())
			return TRUE;

		m_isPressed = FALSE;
		Up();

		return TRUE;
	}
	void CButton::OnMouseOverIn()
	{
		if (!IsEnable())
			return;

		Over();
		PyCallClassMemberFunc(m_poHandler, "ShowToolTip", BuildEmptyTuple());
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverIn", BuildEmptyTuple());
	}
	void CButton::OnMouseOverOut()
	{
		if (!IsEnable())
			return;

		SetUp();
		PyCallClassMemberFunc(m_poHandler, "HideToolTip", BuildEmptyTuple());
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverOut", BuildEmptyTuple());
	}

#ifdef ENABLE_NEW_GAMEOPTION
	DWORD CButton::Type()
	{
		static DWORD s_dwType = GetCRC32("CButton", strlen("CButton"));
		return (s_dwType);
	}
	BOOL CButton::OnIsType(DWORD dwType)
	{
		if (CButton::Type() == dwType)
			return TRUE;
		return FALSE;
	}
	void CButton::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		m_upVisual.SetRenderingRect(fLeft, fTop, fRight, fBottom);
		m_overVisual.SetRenderingRect(fLeft, fTop, fRight, fBottom);
		m_downVisual.SetRenderingRect(fLeft, fTop, fRight, fBottom);
	}
	void CButton::SetCurrentVisual(CGraphicExpandedImageInstance* pVisual)
#else
	void CButton::SetCurrentVisual(CGraphicImageInstance* pVisual)
#endif
	{
		m_pcurVisual = pVisual;
		m_pcurVisual->SetPosition(m_rect.left, m_rect.top);
	}

	BOOL CButton::IsEnable()
	{
		return m_bEnable;
	}

	BOOL CButton::IsPressed()
	{
		return m_isPressed;
	}

#ifdef INSIDE_RENDER
	void CButton::OnUpdateRenderBox()
	{
		m_upVisual.SetRenderBox(m_renderBox);
		m_overVisual.SetRenderBox(m_renderBox);
		m_downVisual.SetRenderBox(m_renderBox);
		m_disableVisual.SetRenderBox(m_renderBox);
	}
	void CButton::OnSetRenderingRect()
	{
		m_upVisual.iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
		m_overVisual.iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
		m_downVisual.iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
		m_disableVisual.iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
	}
#endif

#ifdef USE_CHEQUE_CURRENCY
	void CButton::SetScale(const float x, const float y)
	{
		m_fScaleX = x;
		m_fScaleY = y;

		if (m_pcurVisual)
		{
			m_pcurVisual->SetScale(x, y);
		}

		if (!m_upVisual.IsEmpty())
		{
			m_upVisual.SetScale(x, y);
		}

		if (!m_overVisual.IsEmpty())
		{
			m_overVisual.SetScale(x, y);
		}

		if (!m_downVisual.IsEmpty())
		{
			m_downVisual.SetScale(x, y);
		}

		if (!m_disableVisual.IsEmpty())
		{
			m_disableVisual.SetScale(x, y);
		}

		const auto w = static_cast<float>(GetWidth()) * x;
		const auto h = static_cast<float>(GetHeight()) * y;

		SetSize(static_cast<int>(w), static_cast<int>(h));
	}
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CRadioButton::CRadioButton(PyObject * ppyObject) : CButton(ppyObject)
	{
	}
	CRadioButton::~CRadioButton()
	{
	}
#ifdef ENABLE_NEW_GAMEOPTION
	DWORD CRadioButton::Type()
	{
		static DWORD s_dwType = GetCRC32("CRadioButton", strlen("CRadioButton"));
		return (s_dwType);
	}
	BOOL CRadioButton::OnIsType(DWORD dwType)
	{
		if (CRadioButton::Type() == dwType)
			return TRUE;
		return FALSE;
	}
#endif
	BOOL CRadioButton::OnMouseLeftButtonDown()
	{
		if (!IsEnable())
			return TRUE;

		if (!m_isPressed)
		{
			Down();
			PyCallClassMemberFunc(m_poHandler, "CallEvent", BuildEmptyTuple());
		}

		return TRUE;
	}
	BOOL CRadioButton::OnMouseLeftButtonUp()
	{
		return TRUE;
	}
	void CRadioButton::OnMouseOverIn()
	{
		if (!IsEnable())
			return;

		if (!m_isPressed)
		{
			SetCurrentVisual(&m_overVisual);
		}

		PyCallClassMemberFunc(m_poHandler, "ShowToolTip", BuildEmptyTuple());
	}
	void CRadioButton::OnMouseOverOut()
	{
		if (!IsEnable())
			return;

		if (!m_isPressed)
		{
			SetCurrentVisual(&m_upVisual);
		}

		PyCallClassMemberFunc(m_poHandler, "HideToolTip", BuildEmptyTuple());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CToggleButton::CToggleButton(PyObject * ppyObject) : CButton(ppyObject)
	{
	}
	CToggleButton::~CToggleButton()
	{
	}
#ifdef ENABLE_NEW_GAMEOPTION
	DWORD CToggleButton::Type()
	{
		static DWORD s_dwType = GetCRC32("CToggleButton", strlen("CToggleButton"));
		return (s_dwType);
	}
	BOOL CToggleButton::OnIsType(DWORD dwType)
	{
		if (CToggleButton::Type() == dwType)
			return TRUE;
		return FALSE;
	}
#endif
	BOOL CToggleButton::OnMouseLeftButtonDown()
	{
		if (!IsEnable())
			return TRUE;

		if (m_isPressed)
		{
			SetUp();
			if (IsIn())
				SetCurrentVisual(&m_overVisual);
			else
				SetCurrentVisual(&m_upVisual);
			PyCallClassMemberFunc(m_poHandler, "OnToggleUp", BuildEmptyTuple());
		}
		else
		{
			Down();
			PyCallClassMemberFunc(m_poHandler, "OnToggleDown", BuildEmptyTuple());
		}

		return TRUE;
	}
	BOOL CToggleButton::OnMouseLeftButtonUp()
	{
		return TRUE;
	}

	void CToggleButton::OnMouseOverIn()
	{
		if (!IsEnable())
			return;

		if (!m_isPressed)
		{
			SetCurrentVisual(&m_overVisual);
		}

		PyCallClassMemberFunc(m_poHandler, "ShowToolTip", BuildEmptyTuple());
	}
	void CToggleButton::OnMouseOverOut()
	{
		if (!IsEnable())
			return;

		if (!m_isPressed)
		{
			SetCurrentVisual(&m_upVisual);
		}

		PyCallClassMemberFunc(m_poHandler, "HideToolTip", BuildEmptyTuple());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CDragButton::CDragButton(PyObject * ppyObject) : CButton(ppyObject)
	{
		CWindow::RemoveFlag(CWindow::FLAG_NOT_CAPTURE);
		m_restrictArea.left = 0;
		m_restrictArea.top = 0;
		m_restrictArea.right = CWindowManager::Instance().GetScreenWidth();
		m_restrictArea.bottom = CWindowManager::Instance().GetScreenHeight();
	}
	CDragButton::~CDragButton()
	{
	}

	void CDragButton::SetRestrictMovementArea(int ix, int iy, int iwidth, int iheight)
	{
		m_restrictArea.left = ix;
		m_restrictArea.top = iy;
		m_restrictArea.right = ix + iwidth;
		m_restrictArea.bottom = iy + iheight;
	}

	void CDragButton::OnChangePosition()
	{
		m_x = max(m_x, m_restrictArea.left);
		m_y = max(m_y, m_restrictArea.top);
		m_x = min(m_x, max(0, m_restrictArea.right - m_lWidth));
		m_y = min(m_y, max(0, m_restrictArea.bottom - m_lHeight));

		m_rect.left = m_x;
		m_rect.top = m_y;

		if (m_pParent)
		{
			const RECT & c_rRect = m_pParent->GetRect();
			m_rect.left += c_rRect.left;
			m_rect.top += c_rRect.top;
		}

		m_rect.right = m_rect.left + m_lWidth;
		m_rect.bottom = m_rect.top + m_lHeight;

		std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::UpdateRect));

		if (m_pcurVisual)
			m_pcurVisual->SetPosition(m_rect.left, m_rect.top);

		if (IsPressed())
			PyCallClassMemberFunc(m_poHandler, "OnMove", BuildEmptyTuple());
	}

	void CDragButton::OnMouseOverIn()
	{
		if (!IsEnable())

			return;

		CButton::OnMouseOverIn();
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverIn", BuildEmptyTuple());
	}

	void CDragButton::OnMouseOverOut()
	{
		if (!IsEnable())
			return;

		CButton::OnMouseOverIn();
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverOut", BuildEmptyTuple());
	}
};
