#pragma once

#include "GrpImage.h"
#include "GrpIndexBuffer.h"
#include "GrpVertexBufferDynamic.h"
#include "Pool.h"

class CGraphicImageInstance
{
	public:
		void LeftRightReverse();
	protected:
		bool m_bLeftRightReverse;

	public:
		static DWORD Type();
		BOOL IsType(DWORD dwType);

	public:
		CGraphicImageInstance();
		virtual ~CGraphicImageInstance();

		void Destroy();

#ifdef USE_CLIP_MASK
        void Render(RECT* pClipRect = nullptr);
#else
        void Render();
#endif

		void SetDiffuseColor(float fr, float fg, float fb, float fa);
		void SetScale(float fx, float fy);
		void SetPosition(float fx, float fy);

		void SetImagePointer(CGraphicImage* pImage);
		void ReloadImagePointer(CGraphicImage* pImage);
		bool IsEmpty() const;

		int GetWidth();
		int GetHeight();

#ifdef dracaryS_DUNGEON_LIB
#ifdef USE_CLIP_MASK
        void RenderCoolTime(float fCoolTime, RECT* pClipRect = nullptr);
#else
        void RenderCoolTime(float fCoolTime);
#endif
#endif
		D3DXCOLOR& GetDiffuseColor() { return m_DiffuseColor; };

		CGraphicTexture * GetTexturePointer();
		const CGraphicTexture &	GetTextureReference() const;
		CGraphicImage * GetGraphicImagePointer();

		bool operator == (const CGraphicImageInstance & rhs) const;

	protected:
		void Initialize();

#ifdef USE_CLIP_MASK
        virtual void OnRender(RECT* pClipRect);
#ifdef dracaryS_DUNGEON_LIB
        virtual void OnRenderCoolTime(float fCoolTime, RECT* pClipRect);
#endif
#else
        virtual void OnRender();
#ifdef dracaryS_DUNGEON_LIB
        virtual void OnRenderCoolTime(float fCoolTime);
#endif
#endif

		virtual void OnSetImagePointer();

		virtual BOOL OnIsType(DWORD dwType);

	protected:
		D3DXCOLOR m_DiffuseColor;
		D3DXVECTOR2 m_v2Position;
		D3DXVECTOR2 m_v2Scale;
		float m_vScale;

		CGraphicImage::TRef m_roImage;

	public:
		static void CreateSystem(UINT uCapacity);
		static void DestroySystem();

		static CGraphicImageInstance* New();
		static void Delete(CGraphicImageInstance* pkImgInst);

		static CDynamicPool<CGraphicImageInstance>		ms_kPool;

#ifdef ENABLE_NEW_TARGET_UI
		void DisplayImageProcent(float procent);
#endif
};
