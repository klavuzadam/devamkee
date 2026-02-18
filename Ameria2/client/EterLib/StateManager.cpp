#include "StdAfx.h"
#include "StateManager.h"

#define StateManager_Assert(a) assert(a)

struct SLightData
{
    enum
    {
        LIGHT_NUM = 8,
    };

#ifdef USE_D3D9
    D3DLIGHT9 m_akD3DLight[LIGHT_NUM];
#else
    D3DLIGHT8 m_akD3DLight[LIGHT_NUM];
#endif
} m_kLightData;

#ifdef USE_D3D9
void CStateManager::SetLight(DWORD index, CONST D3DLIGHT9* pLight)
#else
void CStateManager::SetLight(DWORD index, CONST D3DLIGHT8* pLight)
#endif
{
    assert(index < SLightData::LIGHT_NUM);

    m_kLightData.m_akD3DLight[index] = *pLight;
    m_lpD3DDev->SetLight(index, pLight);
}

#ifdef USE_D3D9
void CStateManager::GetLight(DWORD index, D3DLIGHT9* pLight)
#else
void CStateManager::GetLight(DWORD index, D3DLIGHT8* pLight)
#endif
{
    assert(index < 8);
    *pLight = m_kLightData.m_akD3DLight[index];
}

bool CStateManager::BeginScene()
{
    m_bScene = true;

    D3DXMATRIX m4Proj;
    D3DXMATRIX m4View;
    D3DXMATRIX m4World;
    GetTransform(D3DTS_WORLD, &m4World);
    GetTransform(D3DTS_PROJECTION, &m4Proj);
    GetTransform(D3DTS_VIEW, &m4View);
    SetTransform(D3DTS_WORLD, &m4World);
    SetTransform(D3DTS_PROJECTION, &m4Proj);
    SetTransform(D3DTS_VIEW, &m4View);

    if (FAILED(m_lpD3DDev->BeginScene()))
    {
        return false;
    }

    return true;
}

void CStateManager::EndScene()
{
    m_lpD3DDev->EndScene();
    m_bScene = false;
}

#ifdef USE_D3D9
CStateManager::CStateManager(LPDIRECT3DDEVICE9 lpDevice) : m_lpD3DDev(NULL)
#else
CStateManager::CStateManager(LPDIRECT3DDEVICE8 lpDevice) : m_lpD3DDev(NULL)
#endif
{
    m_bScene = false;
    m_dwBestMinFilter = D3DTEXF_LINEAR;
    m_dwBestMagFilter = D3DTEXF_LINEAR;
    SetDevice(lpDevice);
}

CStateManager::~CStateManager()
{
    if (m_lpD3DDev)
    {
        m_lpD3DDev->Release();
        m_lpD3DDev = NULL;
    }
}

#ifdef USE_D3D9
void CStateManager::SetDevice(LPDIRECT3DDEVICE9 lpDevice)
#else
void CStateManager::SetDevice(LPDIRECT3DDEVICE8 lpDevice)
#endif
{
    StateManager_Assert(lpDevice);
    lpDevice->AddRef();

    if (m_lpD3DDev)
    {
        m_lpD3DDev->Release();
        m_lpD3DDev = NULL;
    }

    m_lpD3DDev = lpDevice;

/*
#ifdef USE_D3D9
    D3DCAPS9 d3dCaps;
#else
    D3DCAPS8 d3dCaps;
#endif
    m_lpD3DDev->GetDeviceCaps(&d3dCaps);

    if (d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC)
    {
        m_dwBestMagFilter = D3DTEXF_ANISOTROPIC;
    }
    else
    {
        m_dwBestMagFilter = D3DTEXF_LINEAR;
    }

    if (d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC)
    {
        m_dwBestMinFilter = D3DTEXF_ANISOTROPIC;
    }
    else
    {
        m_dwBestMinFilter = D3DTEXF_LINEAR;
    }

    DWORD dwMax = d3dCaps.MaxAnisotropy;
    dwMax = dwMax < 4 ? dwMax : 4;

    for (uint8_t i = 0; i < 8; ++i)
    {
#ifdef USE_D3D9
        m_lpD3DDev->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, dwMax);
#else
        m_lpD3DDev->SetTextureStageState(i, D3DTSS_MAXANISOTROPY, dwMax);
#endif
    }
*/

    SetDefaultState();
}

void CStateManager::SetBestFiltering(DWORD dwStage)
{
#ifdef USE_D3D9
    SetSamplerState(dwStage, D3DSAMP_MINFILTER, m_dwBestMinFilter);
    SetSamplerState(dwStage, D3DSAMP_MAGFILTER, m_dwBestMagFilter);
    SetSamplerState(dwStage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
#else
    SetTextureStageState(dwStage, D3DTSS_MINFILTER, m_dwBestMinFilter);
    SetTextureStageState(dwStage, D3DTSS_MAGFILTER, m_dwBestMagFilter);
    SetTextureStageState(dwStage, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
#endif
}

void CStateManager::Restore()
{
    m_bForce = true;

    int32_t i, j;
    for (i = 0; i < STATEMANAGER_MAX_RENDERSTATES; ++i)
    {
        SetRenderState(D3DRENDERSTATETYPE(i), m_CurrentState.m_RenderStates[i]);
    }

    for (i = 0; i < STATEMANAGER_MAX_STAGES; ++i)
    {
        for (j = 0; j < STATEMANAGER_MAX_TEXTURESTATES; ++j)
        {
            SetTextureStageState(i, D3DTEXTURESTAGESTATETYPE(j), m_CurrentState.m_TextureStates[i][j]);
#ifdef USE_D3D9
            SetTextureStageState(i, D3DTEXTURESTAGESTATETYPE(j), m_CurrentState.m_SamplerStates[i][j]);
#endif
        }
    }

    for (i = 0; i < STATEMANAGER_MAX_STAGES; ++i)
    {
        SetTexture(i, m_CurrentState.m_Textures[i]);
    }

    m_bForce = false;
}

//#ifdef USE_D3D9
void CStateManager::UpdateAnisotropy()
{
#ifdef USE_D3D9
    D3DCAPS9 d3dCaps;
#else
    D3DCAPS8 d3dCaps;
#endif
    m_lpD3DDev->GetDeviceCaps(&d3dCaps);

    if (d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC)
    {
        m_dwBestMagFilter = D3DTEXF_ANISOTROPIC;
    }
    else
    {
        m_dwBestMagFilter = D3DTEXF_LINEAR;
    }

    if (d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC)
    {
        m_dwBestMinFilter = D3DTEXF_ANISOTROPIC;
    }
    else
    {
        m_dwBestMinFilter = D3DTEXF_LINEAR;
    }

    DWORD dwMax = d3dCaps.MaxAnisotropy;
    dwMax = dwMax < 4 ? dwMax : 4;

    for (uint8_t i = 0; i < 8; ++i)
    {
#ifdef USE_D3D9
        m_lpD3DDev->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, dwMax);
        //SetSamplerState(i, D3DSAMP_MAXANISOTROPY, dwMax);
#else
        m_lpD3DDev->SetTextureStageState(i, D3DTSS_MAXANISOTROPY, dwMax);
#endif
    }

//#ifdef USE_D3D9
//    SaveVertexProcessing(false);
//    m_lpD3DDev->SetSoftwareVertexProcessing(false);
//#endif
}
//#endif

void CStateManager::SetDefaultState()
{
//#ifdef USE_D3D9
    UpdateAnisotropy();
//#endif

    m_CurrentState.ResetState();
    m_CopyState.ResetState();

    m_bScene = false;
    m_bForce = true;

    D3DXMATRIX Identity;
    D3DXMatrixIdentity(&Identity);

    SetTransform(D3DTS_WORLD, &Identity);
    SetTransform(D3DTS_VIEW, &Identity);
    SetTransform(D3DTS_PROJECTION, &Identity);

#ifdef USE_D3D9
    D3DMATERIAL9 DefaultMat;
#else
    D3DMATERIAL8 DefaultMat;
#endif
    ZeroMemory(&DefaultMat, sizeof(DefaultMat));

    DefaultMat.Diffuse.r = 1.0f;
    DefaultMat.Diffuse.g = 1.0f;
    DefaultMat.Diffuse.b = 1.0f;
    DefaultMat.Diffuse.a = 1.0f;
    DefaultMat.Ambient.r = 1.0f;
    DefaultMat.Ambient.g = 1.0f;
    DefaultMat.Ambient.b = 1.0f;
    DefaultMat.Ambient.a = 1.0f;
    DefaultMat.Emissive.r = 0.0f;
    DefaultMat.Emissive.g = 0.0f;
    DefaultMat.Emissive.b = 0.0f;
    DefaultMat.Emissive.a = 0.0f;
    DefaultMat.Specular.r = 0.0f;
    DefaultMat.Specular.g = 0.0f;
    DefaultMat.Specular.b = 0.0f;
    DefaultMat.Specular.a = 0.0f;
    DefaultMat.Power = 0.0f;

    SetMaterial(&DefaultMat);

    SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
    SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
    SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
    SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

#ifndef USE_D3D9
    SetRenderState(D3DRS_LINEPATTERN, 0xFFFFFFFF);
#endif
    SetRenderState(D3DRS_LASTPIXEL, false);
    SetRenderState(D3DRS_ALPHAREF, 1);
    SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
#ifndef USE_D3D9
    SetRenderState(D3DRS_ZVISIBLE, false);
#endif
    SetRenderState(D3DRS_FOGSTART, 0);
    SetRenderState(D3DRS_FOGEND, 0);
    SetRenderState(D3DRS_FOGDENSITY, 0);
#ifndef USE_D3D9
    SetRenderState(D3DRS_EDGEANTIALIAS, false);
    SetRenderState(D3DRS_ZBIAS, 0);
#endif
    SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
    SetRenderState(D3DRS_AMBIENT, 0x00000000);
    SetRenderState(D3DRS_LOCALVIEWER, false);
    SetRenderState(D3DRS_NORMALIZENORMALS, false);
    SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
    SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
#ifdef USE_D3D9
    SaveVertexProcessing(false);
#else
    SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING, false);
#endif
    SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
    SetRenderState(D3DRS_MULTISAMPLEMASK, 0xFFFFFFFF);
    SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, false);
    SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
    SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    SetRenderState(D3DRS_FOGENABLE, false);
    SetRenderState(D3DRS_FOGCOLOR, 0xFF000000);
    SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    SetRenderState(D3DRS_RANGEFOGENABLE, false);
    SetRenderState(D3DRS_ZENABLE, true);
    SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    SetRenderState(D3DRS_ZWRITEENABLE, true);
    SetRenderState(D3DRS_DITHERENABLE, true);
    SetRenderState(D3DRS_STENCILENABLE, false);
    SetRenderState(D3DRS_ALPHATESTENABLE, false);
    SetRenderState(D3DRS_CLIPPING, true);
    SetRenderState(D3DRS_LIGHTING, false);
    SetRenderState(D3DRS_SPECULARENABLE, false);
    SetRenderState(D3DRS_COLORVERTEX, false);
    SetRenderState(D3DRS_WRAP0, 0);
    SetRenderState(D3DRS_WRAP1, 0);
    SetRenderState(D3DRS_WRAP2, 0);
    SetRenderState(D3DRS_WRAP3, 0);
    SetRenderState(D3DRS_WRAP4, 0);
    SetRenderState(D3DRS_WRAP5, 0);
    SetRenderState(D3DRS_WRAP6, 0);
    SetRenderState(D3DRS_WRAP7, 0);

    SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
    SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
    SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    for (uint8_t i = 0; i < 8; i++)
    {
        SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
        SetTextureStageState(i, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        SetTextureStageState(i, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        SetTextureStageState(i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        SetTextureStageState(i, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }

    for (uint8_t i = 0; i < 8; ++i)
    {
        SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, i);
    }

    for (uint8_t i = 0; i < 8; i++)
    {
#ifdef USE_D3D9
        SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

        SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
#else
        SetTextureStageState(i, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
        SetTextureStageState(i, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
        SetTextureStageState(i, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

        SetTextureStageState(i, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
        SetTextureStageState(i, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
#endif

        SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, 0);

        SetTexture(i, NULL);
    }

    SetPixelShader(0);
#ifdef USE_D3D9
    SetFVF(D3DFVF_XYZ);
#else
    SetVertexShader(D3DFVF_XYZ);
#endif

    D3DXVECTOR4 av4Null[STATEMANAGER_MAX_VCONSTANTS];
    memset(av4Null, 0, sizeof(av4Null));
    SetVertexShaderConstant(0, av4Null, STATEMANAGER_MAX_VCONSTANTS);
    SetPixelShaderConstant(0, av4Null, STATEMANAGER_MAX_PCONSTANTS);

    m_bForce = false;

#ifdef _DEBUG
    int32_t i, j;
    for (i = 0; i < STATEMANAGER_MAX_RENDERSTATES; i++)
    {
        m_bRenderStateSavingFlag[i] = false;
    }

/*
    for (j = 0; j < STATEMANAGER_MAX_TRANSFORMSTATES; j++)
    {
        m_bTransformSavingFlag[j] = false;
    }
*/

    for (j = 0; j < STATEMANAGER_MAX_STAGES; ++j)
    {
        for (i = 0; i < STATEMANAGER_MAX_TEXTURESTATES; ++i)
        {
            m_bTextureStageStateSavingFlag[j][i] = false;
        }

#ifdef USE_D3D9
        for (i = 0; i < STATEMANAGER_MAX_TEXTURESTATES; ++i)
        {
            m_bSamplerStateSavingFlag[j][i] = false;
        }
#endif
    }
#endif
}

// Material
void CStateManager::SaveMaterial()
{
    m_CopyState.m_D3DMaterial = m_CurrentState.m_D3DMaterial;
}

#ifdef USE_D3D9
void CStateManager::SaveMaterial(const D3DMATERIAL9* pMaterial)
#else
void CStateManager::SaveMaterial(const D3DMATERIAL8* pMaterial)
#endif
{
    // Check that we have set this up before, if not, the default is this.
    m_CopyState.m_D3DMaterial = m_CurrentState.m_D3DMaterial;
    SetMaterial(pMaterial);
}

void CStateManager::RestoreMaterial()
{
    SetMaterial(&m_CopyState.m_D3DMaterial);
}

#ifdef USE_D3D9
void CStateManager::SetMaterial(const D3DMATERIAL9* pMaterial)
#else
void CStateManager::SetMaterial(const D3DMATERIAL8* pMaterial)
#endif
{
    m_lpD3DDev->SetMaterial(pMaterial);
    m_CurrentState.m_D3DMaterial = *pMaterial;
}

#ifdef USE_D3D9
void CStateManager::GetMaterial(D3DMATERIAL9* pMaterial)
#else
void CStateManager::GetMaterial(D3DMATERIAL8* pMaterial)
#endif
{
    // Set the renderstate and remember it.
    *pMaterial = m_CurrentState.m_D3DMaterial;
}

// Renderstates
DWORD CStateManager::GetRenderState(D3DRENDERSTATETYPE Type)
{
    return m_CurrentState.m_RenderStates[Type];
}

void CStateManager::SaveRenderState(D3DRENDERSTATETYPE Type, DWORD dwValue)
{
#ifdef _DEBUG
    if (m_bRenderStateSavingFlag[Type])
    {
        Tracef("CStateManager::SaveRenderState - This render state is already saved [%d, %d]\n"
                , Type
                , dwValue);

        StateManager_Assert(!" This render state is already saved!");
    }

    m_bRenderStateSavingFlag[Type] = true;
#endif

    // Check that we have set this up before, if not, the default is this.
    m_CopyState.m_RenderStates[Type] = m_CurrentState.m_RenderStates[Type];
    SetRenderState(Type, dwValue);
}

void CStateManager::RestoreRenderState(D3DRENDERSTATETYPE Type)
{
#ifdef _DEBUG
    if (!m_bRenderStateSavingFlag[Type])
    {
        Tracef("CStateManager::SaveRenderState - This render state was not saved [%d, %d]\n"
                , Type);

        StateManager_Assert(!" This render state was not saved!");
    }

    m_bRenderStateSavingFlag[Type] = false;
#endif

    SetRenderState(Type, m_CopyState.m_RenderStates[Type]);
}

void CStateManager::SetRenderState(D3DRENDERSTATETYPE Type, DWORD Value)
{
    if (m_CurrentState.m_RenderStates[Type] == Value)
    {
        return;
    }

    m_lpD3DDev->SetRenderState(Type, Value);
    m_CurrentState.m_RenderStates[Type] = Value;
}

void CStateManager::GetRenderState(D3DRENDERSTATETYPE Type, DWORD* pdwValue)
{
    *pdwValue = m_CurrentState.m_RenderStates[Type];
}

// Textures
#ifdef USE_D3D9
void CStateManager::SaveTexture(DWORD dwStage, LPDIRECT3DBASETEXTURE9 pTexture)
#else
void CStateManager::SaveTexture(DWORD dwStage, LPDIRECT3DBASETEXTURE8 pTexture)
#endif
{
    // Check that we have set this up before, if not, the default is this.
    m_CopyState.m_Textures[dwStage] = m_CurrentState.m_Textures[dwStage];
    SetTexture(dwStage, pTexture);
}

void CStateManager::RestoreTexture(DWORD dwStage)
{
    SetTexture(dwStage, m_CopyState.m_Textures[dwStage]);
}

#ifdef USE_D3D9
void CStateManager::SetTexture(DWORD dwStage, LPDIRECT3DBASETEXTURE9 pTexture)
#else
void CStateManager::SetTexture(DWORD dwStage, LPDIRECT3DBASETEXTURE8 pTexture)
#endif
{
    if (pTexture == m_CurrentState.m_Textures[dwStage])
    {
        return;
    }

    m_lpD3DDev->SetTexture(dwStage, pTexture);
    m_CurrentState.m_Textures[dwStage] = pTexture;
}

#ifdef USE_D3D9
void CStateManager::GetTexture(DWORD dwStage, LPDIRECT3DBASETEXTURE9* ppTexture)
#else
void CStateManager::GetTexture(DWORD dwStage, LPDIRECT3DBASETEXTURE8* ppTexture)
#endif
{
    *ppTexture = m_CurrentState.m_Textures[dwStage];
}

// Texture stage states
void CStateManager::SaveTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE Type, DWORD dwValue)
{
    // Check that we have set this up before, if not, the default is this.
#ifdef _DEBUG
    if (m_bTextureStageStateSavingFlag[dwStage][Type])
    {
        Tracef("CStateManager::SaveTextureStageState - This texture stage state is already saved [%d, %d]\n"
                , dwStage
                , Type);

        StateManager_Assert(!" This texture stage state is already saved!");
    }

    m_bTextureStageStateSavingFlag[dwStage][Type] = true;
#endif

    m_CopyState.m_TextureStates[dwStage][Type] = m_CurrentState.m_TextureStates[dwStage][Type];
    SetTextureStageState(dwStage, Type, dwValue);
}

void CStateManager::RestoreTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE Type)
{
#ifdef _DEBUG
    if (!m_bTextureStageStateSavingFlag[dwStage][Type])
    {
        Tracef(" CStateManager::RestoreTextureStageState - This texture stage state was not saved [%d, %d]\n"
                , dwStage
                , Type);

        StateManager_Assert(!" This texture stage state was not saved!");
    }

    m_bTextureStageStateSavingFlag[dwStage][Type] = false;
#endif

    SetTextureStageState(dwStage, Type, m_CopyState.m_TextureStates[dwStage][Type]);
}

void CStateManager::SetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE Type, DWORD dwValue)
{
    if (m_CurrentState.m_TextureStates[dwStage][Type] == dwValue)
    {
        return;
    }

    m_lpD3DDev->SetTextureStageState(dwStage, Type, dwValue);
    m_CurrentState.m_TextureStates[dwStage][Type] = dwValue;
}

void CStateManager::GetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pdwValue)
{
    *pdwValue = m_CurrentState.m_TextureStates[dwStage][Type];
}

#ifdef USE_D3D9
// Sampler states
void CStateManager::SaveSamplerState(DWORD dwStage
                                        , D3DSAMPLERSTATETYPE Type
                                        , DWORD dwValue)
{
#ifdef _DEBUG
    if (m_bSamplerStateSavingFlag[dwStage][Type])
    {
        Tracef("CStateManager::SaveSamplerState - This texture stage state is already saved [%d, %d]\n"
                , dwStage
                , Type);

        StateManager_Assert(!" This texture stage state is already saved!");
    }

    m_bSamplerStateSavingFlag[dwStage][Type] = true;
#endif

    m_CopyState.m_SamplerStates[dwStage][Type] = m_CurrentState.m_SamplerStates[dwStage][Type];
    SetSamplerState(dwStage, Type, dwValue);
}

void CStateManager::RestoreSamplerState(DWORD dwStage
                                        , D3DSAMPLERSTATETYPE Type)
{
#ifdef _DEBUG
    if (!m_bSamplerStateSavingFlag[dwStage][Type])
    {
        Tracef("CStateManager::RestoreSamplerState - This texture stage state was not saved [%d, %d]\n"
                , dwStage
                , Type);

        StateManager_Assert(!" This texture stage state was not saved!");
    }

    m_bSamplerStateSavingFlag[dwStage][Type] = false;
#endif

    SetSamplerState(dwStage, Type, m_CopyState.m_SamplerStates[dwStage][Type]);
}

void CStateManager::SetSamplerState(DWORD dwStage
                                    , D3DSAMPLERSTATETYPE Type
                                    , DWORD dwValue)
{
/*
    if (m_CurrentState.m_SamplerStates[dwStage][Type] == dwValue)
    {
        return;
    }
*/

    m_lpD3DDev->SetSamplerState(dwStage, Type, dwValue);
    m_CurrentState.m_SamplerStates[dwStage][Type] = dwValue;
}

void CStateManager::GetSamplerState(DWORD dwStage
                                    , D3DSAMPLERSTATETYPE Type
                                    , DWORD* pdwValue)
{
    *pdwValue = m_CurrentState.m_SamplerStates[dwStage][Type];
}
#endif

// Vertex Shader
#ifdef USE_D3D9
void CStateManager::SaveVertexShader(LPDIRECT3DVERTEXSHADER9 dwShader)
#else
void CStateManager::SaveVertexShader(DWORD dwShader)
#endif
{
    m_CopyState.m_dwVertexShader = m_CurrentState.m_dwVertexShader;
    SetVertexShader(dwShader);
}

void CStateManager::RestoreVertexShader()
{
    SetVertexShader(m_CopyState.m_dwVertexShader);
}

#ifdef USE_D3D9
void CStateManager::SetVertexShader(LPDIRECT3DVERTEXSHADER9 dwShader)
#else
void CStateManager::SetVertexShader(DWORD dwShader)
#endif
{
#ifndef USE_D3D9
    if (m_CurrentState.m_dwVertexShader == dwShader)
    {
        return;
    }
#endif

    m_lpD3DDev->SetVertexShader(dwShader);
    m_CurrentState.m_dwVertexShader = dwShader;
}

#ifdef USE_D3D9
void CStateManager::GetVertexShader(LPDIRECT3DVERTEXSHADER9* pdwShader)
#else
void CStateManager::GetVertexShader(DWORD* pdwShader)
#endif
{
    *pdwShader = m_CurrentState.m_dwVertexShader;
}

#ifdef USE_D3D9
// Vertex Declaration
void CStateManager::SaveVertexProcessing(bool IsON)
{
    if (m_CurrentState.m_bVertexProcessing == IsON)
    {
        return;
    }

    m_CopyState.m_bVertexProcessing = m_CurrentState.m_bVertexProcessing;
    m_lpD3DDev->SetSoftwareVertexProcessing(IsON);
    m_CurrentState.m_bVertexProcessing = IsON;
}

void CStateManager::RestoreVertexProcessing()
{
    if (m_CopyState.m_bVertexProcessing == m_CurrentState.m_bVertexProcessing)
    {
        return;
    }

    m_lpD3DDev->SetSoftwareVertexProcessing(m_CopyState.m_bVertexProcessing);
}

void CStateManager::SaveVertexDeclaration(LPDIRECT3DVERTEXDECLARATION9 dwShader)
{
    m_CopyState.m_dwVertexDeclaration = m_CurrentState.m_dwVertexDeclaration;
    SetVertexDeclaration(dwShader);
}

void CStateManager::RestoreVertexDeclaration()
{
    SetVertexDeclaration(m_CopyState.m_dwVertexDeclaration);
}

void CStateManager::SetVertexDeclaration(LPDIRECT3DVERTEXDECLARATION9 dwShader)
{
/*
    if (m_CurrentState.m_dwVertexDeclaration == dwShader)
    {
        return;
    }
*/

    m_lpD3DDev->SetVertexDeclaration(dwShader);
    m_CurrentState.m_dwVertexDeclaration = dwShader;
}

void CStateManager::GetVertexDeclaration(LPDIRECT3DVERTEXDECLARATION9* pdwShader)
{
    *pdwShader = m_CurrentState.m_dwVertexDeclaration;
}

// FVF
void CStateManager::SaveFVF(DWORD dwShader)
{
    m_CopyState.m_dwFVF = m_CurrentState.m_dwFVF;
    SetFVF(dwShader);
}

void CStateManager::RestoreFVF()
{
    SetFVF(m_CopyState.m_dwFVF);
}

void CStateManager::SetFVF(DWORD dwShader)
{
/*
    if (m_CurrentState.m_dwFVF == dwShader)
    {
        return;
    }
*/

    m_lpD3DDev->SetFVF(dwShader);
    m_CurrentState.m_dwFVF = dwShader;
}

void CStateManager::GetFVF(DWORD* pdwShader)
{
    *pdwShader = m_CurrentState.m_dwFVF;
}
#endif

// Pixel Shader
#ifdef USE_D3D9
void CStateManager::SavePixelShader(LPDIRECT3DPIXELSHADER9 dwShader)
#else
void CStateManager::SavePixelShader(DWORD dwShader)
#endif
{
    m_CopyState.m_dwPixelShader = m_CurrentState.m_dwPixelShader;
    SetPixelShader(dwShader);
}

void CStateManager::RestorePixelShader()
{
    SetPixelShader(m_CopyState.m_dwPixelShader);
}

#ifdef USE_D3D9
void CStateManager::SetPixelShader(LPDIRECT3DPIXELSHADER9 dwShader)
#else
void CStateManager::SetPixelShader(DWORD dwShader)
#endif
{
    if (m_CurrentState.m_dwPixelShader == dwShader)
    {
        return;
    }

    m_lpD3DDev->SetPixelShader(dwShader);
    m_CurrentState.m_dwPixelShader = dwShader;
}

#ifdef USE_D3D9
void CStateManager::GetPixelShader(LPDIRECT3DPIXELSHADER9* pdwShader)
#else
void CStateManager::GetPixelShader(DWORD* pdwShader)
#endif
{
    *pdwShader = m_CurrentState.m_dwPixelShader;
}

// *** These states are cached, but not protected from multiple sends of the same value.
// Transform
void CStateManager::SaveTransform(D3DTRANSFORMSTATETYPE Type, const D3DMATRIX* pMatrix)
{
/*
#ifdef _DEBUG
    if (m_bTransformSavingFlag[Type])
    {
        Tracef(" CStateManager::SaveTransform - This transform is already saved [%d]\n", Type);
        StateManager_Assert(!" This trasform is already saved!");
    }
    m_bTransformSavingFlag[Type] = true;
#endif
*/

    m_CopyState.m_Matrices[Type] = m_CurrentState.m_Matrices[Type];
    SetTransform(Type, (D3DXMATRIX*)pMatrix);
}

void CStateManager::RestoreTransform(D3DTRANSFORMSTATETYPE Type)
{
/*
#ifdef _DEBUG
    if (!m_bTransformSavingFlag[Type])
    {
        Tracef(" CStateManager::RestoreTransform - This transform was not saved [%d]\n", Type);
        StateManager_Assert(!" This render state was not saved!");
    }
    m_bTransformSavingFlag[Type] = false;
#endif
*/

    SetTransform(Type, &m_CopyState.m_Matrices[Type]);
}

// Don't cache-check the transform.  To much to do
void CStateManager::SetTransform(D3DTRANSFORMSTATETYPE Type, const D3DMATRIX* pMatrix)
{
    if (m_bScene)
    {
        m_lpD3DDev->SetTransform(Type, pMatrix);
    }
    else
    {
        assert(D3DTS_VIEW == Type || D3DTS_PROJECTION == Type || D3DTS_WORLD == Type);
    }

    m_CurrentState.m_Matrices[Type] = *pMatrix;
}

void CStateManager::GetTransform(D3DTRANSFORMSTATETYPE Type, D3DMATRIX* pMatrix)
{
    *pMatrix = m_CurrentState.m_Matrices[Type];
}

// SetVertexShaderConstant
void CStateManager::SaveVertexShaderConstant(DWORD dwRegister, CONST void* pConstantData, DWORD dwConstantCount)
{
    DWORD i;

    for (i = 0; i < dwConstantCount; i++)
    {
        StateManager_Assert((dwRegister + i) < STATEMANAGER_MAX_VCONSTANTS);
        m_CopyState.m_VertexShaderConstants[dwRegister + i] = m_CurrentState.m_VertexShaderConstants[dwRegister + i];
    }

    SetVertexShaderConstant(dwRegister, pConstantData, dwConstantCount);
}

void CStateManager::RestoreVertexShaderConstant(DWORD dwRegister, DWORD dwConstantCount)
{
    SetVertexShaderConstant(dwRegister, &m_CopyState.m_VertexShaderConstants[dwRegister], dwConstantCount);
}

void CStateManager::SetVertexShaderConstant(DWORD dwRegister, CONST void* pConstantData, DWORD dwConstantCount)
{
#ifdef USE_D3D9
    m_lpD3DDev->SetVertexShaderConstantF(dwRegister, (const float*)pConstantData, dwConstantCount);
#else
    m_lpD3DDev->SetVertexShaderConstant(dwRegister, pConstantData, dwConstantCount);
#endif

    // Set the renderstate and remember it.
    for (DWORD i = 0; i < dwConstantCount; i++)
    {
        StateManager_Assert((dwRegister + i) < STATEMANAGER_MAX_VCONSTANTS);
        m_CurrentState.m_VertexShaderConstants[dwRegister + i] = *(((D3DXVECTOR4*)pConstantData) + i);
    }
}

// SetPixelShaderConstant
void CStateManager::SavePixelShaderConstant(DWORD dwRegister, CONST void* pConstantData, DWORD dwConstantCount)
{
    for (DWORD i = 0; i < dwConstantCount; i++)
    {
        StateManager_Assert((dwRegister + i) < STATEMANAGER_MAX_VCONSTANTS);
        m_CopyState.m_PixelShaderConstants[dwRegister + i] = *(((D3DXVECTOR4*)pConstantData) + i);
    }

    SetPixelShaderConstant(dwRegister, pConstantData, dwConstantCount);
}

void CStateManager::RestorePixelShaderConstant(DWORD dwRegister, DWORD dwConstantCount)
{
    SetPixelShaderConstant(dwRegister, &m_CopyState.m_PixelShaderConstants[dwRegister], dwConstantCount);
}

void CStateManager::SetPixelShaderConstant(DWORD dwRegister, CONST void* pConstantData, DWORD dwConstantCount)
{
#ifdef USE_D3D9
    m_lpD3DDev->SetVertexShaderConstantF(dwRegister, *(D3DXVECTOR4*)pConstantData, dwConstantCount);
#else
    m_lpD3DDev->SetPixelShaderConstant(dwRegister, pConstantData, dwConstantCount);
#endif

    // Set the renderstate and remember it.
    for (DWORD i = 0; i < dwConstantCount; i++)
    {
        StateManager_Assert((dwRegister + i) < STATEMANAGER_MAX_VCONSTANTS);
        m_CurrentState.m_PixelShaderConstants[dwRegister + i] = *(((D3DXVECTOR4*)pConstantData) + i);
    }
}

#ifdef USE_D3D9
void CStateManager::SaveStreamSource(UINT StreamNumber, LPDIRECT3DVERTEXBUFFER9 pStreamData, UINT Stride)
#else
void CStateManager::SaveStreamSource(UINT StreamNumber, LPDIRECT3DVERTEXBUFFER8 pStreamData, UINT Stride)
#endif
{
    // Check that we have set this up before, if not, the default is this.
    m_CopyState.m_StreamData[StreamNumber] = m_CurrentState.m_StreamData[StreamNumber];
    SetStreamSource(StreamNumber, pStreamData, Stride);
}

void CStateManager::RestoreStreamSource(UINT StreamNumber)
{
    SetStreamSource(StreamNumber,
        m_CopyState.m_StreamData[StreamNumber].m_lpStreamData,
        m_CopyState.m_StreamData[StreamNumber].m_Stride);
}

#ifdef USE_D3D9
void CStateManager::SetStreamSource(UINT StreamNumber, LPDIRECT3DVERTEXBUFFER9 pStreamData, UINT Stride)
#else
void CStateManager::SetStreamSource(UINT StreamNumber, LPDIRECT3DVERTEXBUFFER8 pStreamData, UINT Stride)
#endif
{
    CStreamData kStreamData(pStreamData, Stride);
    if (m_CurrentState.m_StreamData[StreamNumber] == kStreamData)
    {
        return;
    }

    m_lpD3DDev->SetStreamSource(StreamNumber, pStreamData
#ifdef USE_D3D9
    , 0
#endif
    , Stride);

    m_CurrentState.m_StreamData[StreamNumber] = kStreamData;
}

#ifdef USE_D3D9
void CStateManager::SaveIndices(LPDIRECT3DINDEXBUFFER9 pIndexData, UINT BaseVertexIndex)
#else
void CStateManager::SaveIndices(LPDIRECT3DINDEXBUFFER8 pIndexData, UINT BaseVertexIndex)
#endif
{
    m_CopyState.m_IndexData = m_CurrentState.m_IndexData;
    SetIndices(pIndexData, BaseVertexIndex);
}

void CStateManager::RestoreIndices()
{
    SetIndices(m_CopyState.m_IndexData.m_lpIndexData, m_CopyState.m_IndexData.m_BaseVertexIndex);
}

#ifdef USE_D3D9
void CStateManager::SetIndices(LPDIRECT3DINDEXBUFFER9 pIndexData, UINT BaseVertexIndex)
#else
void CStateManager::SetIndices(LPDIRECT3DINDEXBUFFER8 pIndexData, UINT BaseVertexIndex)
#endif
{
    CIndexData kIndexData(pIndexData, BaseVertexIndex);
    if (m_CurrentState.m_IndexData == kIndexData)
    {
        return;
    }

#ifdef USE_D3D9
    m_lpD3DDev->SetIndices(pIndexData);
#else
    m_lpD3DDev->SetIndices(pIndexData, BaseVertexIndex);
#endif
    m_CurrentState.m_IndexData = kIndexData;
}

HRESULT CStateManager::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    return (m_lpD3DDev->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount));
}

HRESULT CStateManager::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    m_CurrentState.m_StreamData[0] = NULL;
    return (m_lpD3DDev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride));
}

HRESULT CStateManager::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT minIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
#ifdef USE_D3D9
    return (m_lpD3DDev->DrawIndexedPrimitive(PrimitiveType, minIndex, 0, NumVertices, startIndex, primCount));
#else
    return (m_lpD3DDev->DrawIndexedPrimitive(PrimitiveType, minIndex, NumVertices, startIndex, primCount));
#endif
}

#ifdef USE_D3D9
HRESULT CStateManager::DrawIndexedPrimitiveReversed(D3DPRIMITIVETYPE PrimitiveType, UINT minIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
    return (m_lpD3DDev->DrawIndexedPrimitive(PrimitiveType, 0, minIndex, NumVertices, startIndex, primCount));
}
#endif

HRESULT CStateManager::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    m_CurrentState.m_IndexData = NULL;
    m_CurrentState.m_StreamData[0] = NULL;
    return (m_lpD3DDev->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride));
}
