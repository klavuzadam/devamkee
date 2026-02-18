#include "StdAfx.h"
#include "EffectElementBaseInstance.h"

bool CEffectElementBaseInstance::Update(float fElapsedTime)
{
	if (m_bStart)
	{
		m_fElapsedTime = fElapsedTime;
		m_fLocalTime += fElapsedTime;

		return OnUpdate(fElapsedTime);
	}
	else
	{
		m_fRemainingTime -= fElapsedTime;
		if (m_fRemainingTime<=0.0f)
			m_bStart = true;
		return true;
	}
}

void CEffectElementBaseInstance::Render()
{
    if (!m_bStart)
    {
        return;
    }

    if (!isActive())
    {
        return;
    }

#ifdef USE_EFFECTS_LOD
    if (IsHiddenByLod()
#ifdef ENABLE_WIKI
        && !m_wikiIgnoreFrustum
#endif
        && !m_ignoreFrustum)
    {
        return;
    }
#endif

	assert(mc_pmatLocal);

	OnRender();
}

void CEffectElementBaseInstance::SetParticleScale(float fParticleScale)
{
	m_fParticleScale = fParticleScale;
}

void CEffectElementBaseInstance::SetLocalMatrixPointer(const D3DXMATRIX * c_pMatrix)
{
	mc_pmatLocal = c_pMatrix;
}

void CEffectElementBaseInstance::SetDataPointer(CEffectElementBase * pElement)
{
	m_pBase = pElement;

	m_dwStartTime = CTimer::Instance().GetCurrentMillisecond();

	//////////////////////////////////////////////////////////////////////////
	//add by ipkn, start time management

	m_fRemainingTime = pElement->GetStartTime();
	if (m_fRemainingTime<=0.0f)
		m_bStart = true;
	else
		m_bStart = false;

	//////////////////////////////////////////////////////////////////////////

	OnSetDataPointer(pElement);
}

bool CEffectElementBaseInstance::isActive()
{
	return m_isActive;
}
void CEffectElementBaseInstance::SetActive()
{
	m_isActive = true;
}
void CEffectElementBaseInstance::SetDeactive()
{
	m_isActive = false;
}

#ifdef USE_EFFECTS_LOD
bool CEffectElementBaseInstance::IsHiddenByLod()
{
    return m_isHiddenByLod;
}

void CEffectElementBaseInstance::SetHiddenByLod()
{
    m_isHiddenByLod = true;
}

void CEffectElementBaseInstance::SetShownByLod()
{
    m_isHiddenByLod = false;
}
#endif

void CEffectElementBaseInstance::Initialize()
{
	mc_pmatLocal = NULL;

	m_isActive = true;

#ifdef USE_EFFECTS_LOD
    m_isHiddenByLod = false;
#endif

	m_fLocalTime = 0.0f;
	m_dwStartTime = 0;
	m_fElapsedTime = 0.0f;
#ifdef ENABLE_WIKI
	m_wikiIgnoreFrustum = false;
#endif
	m_bStart = false;
	m_fRemainingTime = 0.0f;

	m_ignoreFrustum = false;
	
	m_fParticleScale = 1.0f;


	OnInitialize();
}

void CEffectElementBaseInstance::Destroy()
{
	OnDestroy();
	Initialize();
}

CEffectElementBaseInstance::CEffectElementBaseInstance()
{
}
CEffectElementBaseInstance::~CEffectElementBaseInstance()
{
}