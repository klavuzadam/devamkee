#include "StdAfx.h"
#include "PythonChat.h"

#include "AbstractApplication.h"
#include "PythonCharacterManager.h"
#include "../eterbase/Timer.h"
#include "../GameLib/ItemManager.h"
#ifdef ENABLE_MULTILANGUAGE_SYSTEM
#include "PythonApplication.h"
#endif
int CPythonChat::TChatSet::ms_iChatModeSize = CHAT_TYPE_MAX_NUM;

const float c_fStartDisappearingTime = 5.0f;
const int c_iMaxLineCount = 5;

///////////////////////////////////////////////////////////////////////////////////////////////////

CDynamicPool<CPythonChat::SChatLine> CPythonChat::SChatLine::ms_kPool;

void CPythonChat::SetChatColor(UINT eType, UINT r, UINT g, UINT b)
{
	if (eType>=CHAT_TYPE_MAX_NUM)
		return;

	DWORD dwColor=(0xff000000)|(r<<16)|(g<<8)|(b);
	m_akD3DXClrChat[eType]=D3DXCOLOR(dwColor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CPythonChat::SChatLine* CPythonChat::SChatLine::New()
{
	return ms_kPool.Alloc();
}

void CPythonChat::SChatLine::Delete(CPythonChat::SChatLine* pkChatLine)
{
	pkChatLine->Instance.Destroy();
#ifdef ENABLE_MULTILANGUAGE_SYSTEM	
	pkChatLine->InstanceLang.Destroy();
	pkChatLine->InstanceEmpire.Destroy();
#endif	
	ms_kPool.Free(pkChatLine);
}

void CPythonChat::SChatLine::DestroySystem()
{
	ms_kPool.Destroy();
}

void CPythonChat::SChatLine::SetColor(DWORD dwID, DWORD dwColor)
{
	assert(dwID < CHAT_LINE_COLOR_ARRAY_MAX_NUM);

	if (dwID >= CHAT_LINE_COLOR_ARRAY_MAX_NUM)
		return;

	aColor[dwID] = dwColor;
}

void CPythonChat::SChatLine::SetColorAll(DWORD dwColor)
{
	for (int i = 0; i < CHAT_LINE_COLOR_ARRAY_MAX_NUM; ++i)
		aColor[i] = dwColor;
}

D3DXCOLOR & CPythonChat::SChatLine::GetColorRef(DWORD dwID)
{
#ifdef __CHAT_SETTINGS__
	if (dwID >= CHAT_LINE_COLOR_ARRAY_MAX_NUM)
		return aColor[0];
#else
	assert(dwID < CHAT_LINE_COLOR_ARRAY_MAX_NUM);
	if (dwID >= CHAT_LINE_COLOR_ARRAY_MAX_NUM)
	{
		static D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
		return color;
	}
#endif

	return aColor[dwID];
}

CPythonChat::SChatLine::SChatLine()
{
	for (int i = 0; i < CHAT_LINE_COLOR_ARRAY_MAX_NUM; ++i)
		aColor[i] = 0xff0000ff;
#ifdef __CHAT_SETTINGS__
	countryIndex = 0;
	empireIndex = 0;
#endif

}
CPythonChat::SChatLine::~SChatLine()
{
	Instance.Destroy();
#ifdef ENABLE_MULTILANGUAGE_SYSTEM	
	InstanceLang.Destroy();
	InstanceEmpire.Destroy();
#endif	
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int CPythonChat::CreateChatSet(DWORD dwID)
{
#ifdef __CHAT_SETTINGS__
	auto it = m_ChatLineNewDeque.find(dwID);
	if (it != m_ChatLineNewDeque.end())
		return dwID;


	TChatSet pChatSet;
	TChatLineDeque map_vec;
	map_vec.clear();

	auto itChat = m_ChatLineNewDeque.find(0);

	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());

	if (itChat != m_ChatLineNewDeque.end())
	{
		if (itChat->second.size())
		{
			for (auto chat = itChat->second.begin(); chat != itChat->second.end(); ++chat)
			{
				SChatLine* chatLine = *chat;
				SChatLine* pChatLine = SChatLine::New();

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
				pChatLine->empireIndex = chatLine->empireIndex;
				pChatLine->countryIndex = chatLine->countryIndex;
				if (chatLine->iType == CHAT_TYPE_SHOUT)
				{
					if (pChatLine->countryIndex != 0)
					{
						const char* langlist[] = { "en","de","tr","pt","es","fr","ro","pl","it","cz","hu" };
						char buf[128];
						sprintf(buf, "d:/ymir work/ui/language_flag/%s.tga", langlist[pChatLine->countryIndex - 1]);
						if (CResourceManager::Instance().IsFileExist(buf, __FUNCTION__))
							pChatLine->InstanceLang.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
					}
					if (pChatLine->empireIndex != 0)
					{
						char buf[128];
						sprintf(buf, "d:/ymir work/ui/flag_empire/empire_%d_256.dds", pChatLine->empireIndex);
						if (CResourceManager::Instance().IsFileExist(buf, __FUNCTION__))
							pChatLine->InstanceEmpire.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
					}
				}
#endif

				pChatLine->text = chatLine->text;
				pChatLine->iType = chatLine->iType;
				pChatLine->Instance.SetValue(chatLine->text.c_str());
				pChatLine->Instance.SetTextPointer(pkDefaultFont);
				pChatLine->fAppendedTime = chatLine->fAppendedTime;
				pChatLine->SetColorAll(GetChatColor(pChatLine->iType));
				map_vec.push_back(pChatLine);
			}
		}
	}

	m_ChatSetMap.emplace(dwID, pChatSet);
	m_ChatLineNewDeque.emplace(dwID, map_vec);
	ArrangeShowingChat(dwID);
#else
	m_ChatSetMap.insert(std::make_pair(dwID, TChatSet()));
#endif
	return dwID;
}
void CPythonChat::UpdateViewMode(DWORD dwID)
{
	IAbstractApplication& rApp=IAbstractApplication::GetSingleton();

	float fcurTime = rApp.GetGlobalTime();
	//float felapsedTime = rApp.GetGlobalElapsedTime();

	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	TChatLineList * pLineList = &(pChatSet->m_ShowingChatLineList);
	int iLineIndex = pLineList->size();
	int iHeight = -(int(pLineList->size()+1) * pChatSet->m_iStep);

	TChatLineList::iterator itor;
	for (itor = pLineList->begin(); itor != pLineList->end();)
	{
		TChatLine * pChatLine = (*itor);

		D3DXCOLOR & rColor = pChatLine->GetColorRef(dwID);

		float fElapsedTime = (fcurTime - pChatLine->fAppendedTime);
		if (fElapsedTime >= c_fStartDisappearingTime || iLineIndex >= c_iMaxLineCount)
		{
			rColor.a -= rColor.a / 10.0f;

			if (rColor.a <= 0.1f)
			{
				itor = pLineList->erase(itor);
			}
			else
			{
				++itor;
			}
		}
		else
		{
			++itor;
		}

		/////

		iHeight += pChatSet->m_iStep;
		--iLineIndex;

		int iWidth = pChatSet->m_ix;
#ifdef ENABLE_MULTILANGUAGE_SYSTEM	
		if (pChatLine->iType == CHAT_TYPE_SHOUT)
		{
			bool isAttached = (pChatLine->InstanceLang.GetWidth() > 0 && pChatLine->InstanceEmpire.GetWidth() > 0);
			if (isAttached)
			{
				pChatLine->InstanceLang.SetPosition(iWidth, pChatSet->m_iy + iHeight + 2);
				iWidth += pChatLine->InstanceLang.GetWidth() + 2;

				pChatLine->InstanceEmpire.SetPosition(iWidth + 4, pChatSet->m_iy + iHeight + 2);
				iWidth += pChatLine->InstanceEmpire.GetWidth() + 8;
			}
		}
#endif
		pChatLine->Instance.SetPosition(iWidth, pChatSet->m_iy + iHeight);
		pChatLine->Instance.SetColor(rColor);
		pChatLine->Instance.Update();
	}
}

void CPythonChat::UpdateEditMode(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	const int c_iAlphaLine = max(0, GetVisibleLineCount(dwID) - GetEditableLineCount(dwID) + 2);

	int iLineIndex = 0;
	float fAlpha = 0.0f;
	float fAlphaStep = 0.0f;

	if (c_iAlphaLine > 0)
		fAlphaStep = 1.0f / float(c_iAlphaLine);

	TChatLineList * pLineList = &(pChatSet->m_ShowingChatLineList);
	int iHeight = -(int(pLineList->size()+1) * pChatSet->m_iStep);

	for (TChatLineList::iterator itor = pLineList->begin(); itor != pLineList->end(); ++itor)
	{
		TChatLine * pChatLine = (*itor);

		D3DXCOLOR & rColor = pChatLine->GetColorRef(dwID);

		if (iLineIndex < c_iAlphaLine)
		{
			rColor.a += (fAlpha - rColor.a) / 10.0f;
			fAlpha = fMIN(fAlpha+fAlphaStep, 1.0f);
		}
		else
		{
			rColor.a = fMIN(rColor.a+0.05f, 1.0f);
		}

		iHeight += pChatSet->m_iStep;
		int iWidth = pChatSet->m_ix;

#ifdef ENABLE_MULTILANGUAGE_SYSTEM	
		if (pChatLine->iType == CHAT_TYPE_SHOUT)
		{
			bool isAttached = (pChatLine->InstanceLang.GetWidth() > 0 && pChatLine->InstanceEmpire.GetWidth() > 0);
			if (isAttached)
			{
				pChatLine->InstanceLang.SetPosition(iWidth, pChatSet->m_iy + iHeight + 2);
				iWidth += pChatLine->InstanceLang.GetWidth() + 2;

				pChatLine->InstanceEmpire.SetPosition(iWidth + 4, pChatSet->m_iy + iHeight + 2);
				iWidth += pChatLine->InstanceEmpire.GetWidth() + 8;
			}
		}
#endif
		pChatLine->Instance.SetPosition(iWidth, pChatSet->m_iy + iHeight);
		pChatLine->Instance.SetColor(rColor);
		pChatLine->Instance.Update();
	}
}

void CPythonChat::UpdateLogMode(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	TChatLineList * pLineList = &(pChatSet->m_ShowingChatLineList);
	int iHeight = 0;

	for (TChatLineList::reverse_iterator itor = pLineList->rbegin(); itor != pLineList->rend(); ++itor)
	{
		TChatLine * pChatLine = (*itor);

		iHeight -= pChatSet->m_iStep;
		int iWidth = pChatSet->m_ix;

#ifdef ENABLE_MULTILANGUAGE_SYSTEM	
		if (pChatLine->iType == CHAT_TYPE_SHOUT)
		{
			bool isAttached = (pChatLine->InstanceLang.GetWidth() > 0 && pChatLine->InstanceEmpire.GetWidth() > 0);
			if (isAttached)
			{
				pChatLine->InstanceLang.SetPosition(iWidth, pChatSet->m_iy + iHeight + 2);
				iWidth += pChatLine->InstanceLang.GetWidth() + 2;

				pChatLine->InstanceEmpire.SetPosition(iWidth + 4, pChatSet->m_iy + iHeight + 2);
				iWidth += pChatLine->InstanceEmpire.GetWidth() + 8;
			}
		}
#endif
		pChatLine->Instance.SetPosition(iWidth, pChatSet->m_iy + iHeight);
		pChatLine->Instance.SetColor(pChatLine->GetColorRef(dwID));
		pChatLine->Instance.Update();
	}
}

void CPythonChat::Update(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	switch (pChatSet->m_iBoardState)
	{
		case BOARD_STATE_VIEW:
			UpdateViewMode(dwID);
			break;
		case BOARD_STATE_EDIT:
			UpdateEditMode(dwID);
			break;
		case BOARD_STATE_LOG:
			UpdateLogMode(dwID);
			break;
	}

    DWORD dwCurTime = CTimer::Instance().GetCurrentMillisecond();
    for (auto it = m_WaitChatList.begin(); it != m_WaitChatList.end();)
    {
        TWaitChat& rWaitChat = *it;

        if (rWaitChat.dwAppendingTime < dwCurTime)
        {
			AppendChat(rWaitChat.iType, rWaitChat.strChat.c_str());
            it = m_WaitChatList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void CPythonChat::Render(DWORD dwID)
{
	TChatLineList * pLineList = GetChatLineListPtr(dwID);
	if (!pLineList)
		return;

	for (TChatLineList::iterator itor = pLineList->begin(); itor != pLineList->end(); ++itor)
	{
		CGraphicTextInstance& rInstance = (*itor)->Instance;
		rInstance.Render();
#ifdef ENABLE_MULTILANGUAGE_SYSTEM	
		if ((*itor)->iType == CHAT_TYPE_SHOUT)
		{
			CGraphicImageInstance& lInstance = (*itor)->InstanceLang;
			if(lInstance.GetWidth() > 0)
				lInstance.Render();
			CGraphicImageInstance& eInstance = (*itor)->InstanceEmpire;
			if(eInstance.GetWidth() > 0)
				eInstance.Render();
		}
#endif	
	}
}


void CPythonChat::SetBoardState(DWORD dwID, int iState)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iBoardState = iState;
	ArrangeShowingChat(dwID);
}
void CPythonChat::SetPosition(DWORD dwID, int ix, int iy)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_ix = ix;
	pChatSet->m_iy = iy;
}
void CPythonChat::SetHeight(DWORD dwID, int iHeight)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iHeight = iHeight;
}
void CPythonChat::SetStep(DWORD dwID, int iStep)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iStep = iStep;
}
void CPythonChat::ToggleChatMode(DWORD dwID, int iMode)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iMode[iMode] = 1 - pChatSet->m_iMode[iMode];
// 	Tracef("ToggleChatMode : %d\n", iMode);
	ArrangeShowingChat(dwID);
}
void CPythonChat::EnableChatMode(DWORD dwID, int iMode)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iMode[iMode] = TRUE;
// 	Tracef("EnableChatMode : %d\n", iMode);
	ArrangeShowingChat(dwID);
}
void CPythonChat::DisableChatMode(DWORD dwID, int iMode)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iMode[iMode] = FALSE;
// 	Tracef("DisableChatMode : %d\n", iMode);
	ArrangeShowingChat(dwID);
}
void CPythonChat::SetEndPos(DWORD dwID, float fPos)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	fPos = max(0.0f, fPos);
	fPos = min(1.0f, fPos);
	if (pChatSet->m_fEndPos != fPos)
	{
		pChatSet->m_fEndPos = fPos;
		ArrangeShowingChat(dwID);
	}
}


int CPythonChat::GetVisibleLineCount(DWORD dwID)
{
	TChatLineList * pLineList = GetChatLineListPtr(dwID);
	if (!pLineList)
		return 0;

	return pLineList->size();
}

int CPythonChat::GetEditableLineCount(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return 0;

	return pChatSet->m_iHeight / pChatSet->m_iStep + 1;
}

int CPythonChat::GetLineCount(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return 0;
#ifdef __CHAT_SETTINGS__
	auto it = m_ChatLineNewDeque.find(dwID);
	if (it == m_ChatLineNewDeque.end())
		return 0;
#endif
	int iCount = 0;
#ifdef __CHAT_SETTINGS__
	for (DWORD i = 0; i < it->second.size(); ++i)
	{
		if (!pChatSet->CheckMode(it->second[i]->iType))
			continue;
		if (!pChatSet->CheckCountryMode(it->second[i]->countryIndex) && it->second[i]->iType == CHAT_TYPE_SHOUT)
			continue;
#else
	for (DWORD i = 0; i < m_ChatLineDeque.size(); ++i)
	{
		if (!pChatSet->CheckMode(m_ChatLineDeque[i]->iType))
			continue;

#endif
		++iCount;
	}

	return iCount;
}

int CPythonChat::GetLineStep(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return 0;

	return pChatSet->m_iStep;
}

CPythonChat::TChatLineList * CPythonChat::GetChatLineListPtr(DWORD dwID)
{
	TChatSetMap::iterator itor = m_ChatSetMap.find(dwID);
	if (m_ChatSetMap.end() == itor)
		return NULL;

	TChatSet & rChatSet = itor->second;
	return &(rChatSet.m_ShowingChatLineList);
}

CPythonChat::TChatSet * CPythonChat::GetChatSetPtr(DWORD dwID)
{
	TChatSetMap::iterator itor = m_ChatSetMap.find(dwID);
	if (m_ChatSetMap.end() == itor)
		return NULL;

	TChatSet & rChatSet = itor->second;
	return &rChatSet;
}

void CPythonChat::ArrangeShowingChat(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_ShowingChatLineList.clear();
#ifdef __CHAT_SETTINGS__
	auto it = m_ChatLineNewDeque.find(dwID);
	if (it == m_ChatLineNewDeque.end())
		return;
#endif
	TChatLineDeque TempChatLineDeque;
#ifdef __CHAT_SETTINGS__
	for (TChatLineDeque::iterator itor = it->second.begin(); itor != it->second.end(); ++itor)
#else
	for (TChatLineDeque::iterator itor = m_ChatLineDeque.begin(); itor != m_ChatLineDeque.end(); ++itor)
#endif
	{
		TChatLine * pChatLine = *itor;
		if (pChatSet->CheckMode(pChatLine->iType))
			TempChatLineDeque.push_back(pChatLine);
#ifdef __CHAT_SETTINGS__
		if (!pChatSet->CheckCountryMode(pChatLine->countryIndex) && pChatLine->iType == CHAT_TYPE_SHOUT)
			continue;
#endif
	}

	int icurLineCount = TempChatLineDeque.size();
	int iVisibleLineCount = min(icurLineCount, (pChatSet->m_iHeight + pChatSet->m_iStep) / pChatSet->m_iStep);
	int iEndLine = iVisibleLineCount + int(float(icurLineCount - iVisibleLineCount - 1) * pChatSet->m_fEndPos);

	/////

	int iHeight = 12;
	for (int i = min(icurLineCount-1, iEndLine); i >= 0; --i)
	{
		if (!pChatSet->CheckMode(TempChatLineDeque[i]->iType))
			continue;
#ifdef __CHAT_SETTINGS__
		if (!pChatSet->CheckCountryMode(TempChatLineDeque[i]->countryIndex) && TempChatLineDeque[i]->iType == CHAT_TYPE_SHOUT)
			continue;
#endif
		if (pChatSet->m_iHeight + pChatSet->m_iStep <= iHeight)
		{
			break;
		}

		pChatSet->m_ShowingChatLineList.push_front(TempChatLineDeque[i]);

		iHeight += pChatSet->m_iStep;
	}
}

void CPythonChat::AppendChat(int iType, const char * c_szChat)
{
	if (strstr(c_szChat, "#ebvs"))
		return;
	// DEFAULT_FONT
	
	if (iType == CHAT_TYPE_SHOUT && IsDelayEnabled()) {
		m_vecDelayedChatMessages.emplace_back(iType, c_szChat);
		// Check and flush delayed messages if necessary
		FlushDelayedChatsIfNeeded();
		return; // Skip appending directly
	}

	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());
	if (!pkDefaultFont)
	{
		TraceError("CPythonChat::AppendChat - CANNOT_FIND_DEFAULT_FONT");
		return;
	}
	// END_OF_DEFAULT_FONT

	std::string str = c_szChat;
	std::string findl1 = "|Hitem:";
	std::size_t find1 = 0;

	std::size_t pos1 = 0;
	for (int i = 0; i < str.length(); i++)
	{
		find1 = str.find(findl1, i);;
		if (find1 != std::string::npos)
		{
			pos1 = find1 + findl1.length();
			i = find1 + findl1.length();

			if (str.length() - pos1 <= 0 || str.length() - pos1 >= str.length())
				break;

			const std::string item_vnumabc = str.substr(pos1, str.length() - pos1);		// |cffffc700|Hitem:12b:1:1:1:0:0:48:25:47:-6|h[Sabie luna plina+9]|h|r 
			const std::size_t pos2 = item_vnumabc.find(":");
			if (pos2 == std::string::npos)
				break;
			const std::string item_vnum = item_vnumabc.substr(0, pos2);
			char * p;
			long n = strtol(item_vnum.c_str(), &p, 16);
			if (*p != 0)
			{
				TraceError("%s a number %s", item_vnum, str);
			}
			else
			{
				const std::size_t pos_1 = item_vnumabc.find("|h[");
				const std::size_t pos_2 = item_vnumabc.find("]|h");
				if (pos_1 != std::string::npos && pos_2 != std::string::npos)
				{
					std::string item_name = item_vnumabc.substr(pos_1 + 3, pos_2 - pos_1 - 3);
					CItemData* pItemData = NULL;
					if (CItemManager::Instance().GetItemDataPointer(n, &pItemData))
					{
						if (strcmp(item_name.c_str(), pItemData->GetName()) != 0)
						{
							str = str.replace(str.find(item_name), item_name.length(), pItemData->GetName());
							c_szChat = str.c_str();
						}
					}
				}
				else
					break;
			}
		}
	}

	IAbstractApplication& rApp=IAbstractApplication::GetSingleton();

#ifdef __CHAT_SETTINGS__
	if (m_ChatLineNewDeque.size())
	{
		std::string strContent = c_szChat;

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		BYTE empireIndex = 0;
		BYTE countryIndex = 0;
		if (iType == CHAT_TYPE_SHOUT)
		{
			std::string isLangFlag("|lang|");
			int position = strContent.find(isLangFlag);
			if (position != std::string::npos)
			{
				strContent = strContent.substr(position + isLangFlag.length(), strContent.length());
				position = strContent.find("|");
				std::string c_szLanguage = strContent.substr(0, position);
				countryIndex = atoi(c_szLanguage.c_str());
				if (countryIndex > 11 || countryIndex == 0)
					countryIndex = 1;
				strContent = strContent.substr(position + 1, strContent.length());

			}
			std::string isEmpireFlag("|empire|");
			position = strContent.find(isEmpireFlag);
			if (position != std::string::npos)
			{
				strContent = strContent.substr(position + isEmpireFlag.length(), strContent.length());
				position = strContent.find("|");
				std::string c_szEmpire = strContent.substr(0, position);
				empireIndex = atoi(c_szEmpire.c_str());
				strContent = strContent.substr(position + 1, strContent.length());
			}
		}
		
#endif
		for (auto it = m_ChatLineNewDeque.begin(); it != m_ChatLineNewDeque.end(); ++it)
		{
			SChatLine* pChatLine = SChatLine::New();

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
			pChatLine->empireIndex = empireIndex;
			pChatLine->countryIndex = countryIndex;
			if (iType == CHAT_TYPE_SHOUT)
			{
				if (pChatLine->countryIndex != 0)
				{
					const char* langlist[] = { "en","de","tr","pt","es","fr","ro","pl","it","cz","hu" };
					char buf[128];
					sprintf(buf, "d:/ymir work/ui/language_flag/%s.tga", langlist[pChatLine->countryIndex-1]);
					if (CResourceManager::Instance().IsFileExist(buf, __FUNCTION__))
						pChatLine->InstanceLang.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
				}
				if (pChatLine->empireIndex != 0)
				{
					char buf[128];
					sprintf(buf, "d:/ymir work/ui/flag_empire/empire_%d_256.dds", pChatLine->empireIndex);
					if (CResourceManager::Instance().IsFileExist(buf, __FUNCTION__))
						pChatLine->InstanceEmpire.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
				}
				//TraceError("country %d empire %d", pChatLine->countryIndex, pChatLine->empireIndex);
			}
#endif
			pChatLine->text = strContent;
			pChatLine->iType = iType;
			pChatLine->Instance.SetValue(strContent.c_str());
			pChatLine->Instance.SetTextPointer(pkDefaultFont);
			pChatLine->fAppendedTime = rApp.GetGlobalTime();
			pChatLine->SetColorAll(GetChatColor(iType));
			it->second.push_back(pChatLine);
			if (it->second.size() > CHAT_LINE_MAX_NUM)
			{
				SChatLine* pChatLine = it->second.front();
				SChatLine::Delete(pChatLine);
				it->second.pop_front();
			}
			auto itChatSet = m_ChatSetMap.find(it->first);
			if (itChatSet != m_ChatSetMap.end())
			{
				TChatSet* pChatSet = &(itChatSet->second);
				if (BOARD_STATE_EDIT == pChatSet->m_iBoardState)
					ArrangeShowingChat(itChatSet->first);
				else
				{
					pChatSet->m_ShowingChatLineList.push_back(pChatLine);
					if (pChatSet->m_ShowingChatLineList.size() > CHAT_LINE_MAX_NUM)
						pChatSet->m_ShowingChatLineList.pop_front();
					ArrangeShowingChat(itChatSet->first);
				}
			}

		}
	}
#else
	SChatLine* pChatLine = SChatLine::New();
	std::string strContent = c_szChat;

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	if (iType == CHAT_TYPE_SHOUT)
	{
		BYTE langIndex = 0;
		BYTE empireIndex = 0;


		std::string isLangFlag("|lang|");
		int position = strContent.find(isLangFlag);

		if (position != std::string::npos)
		{
			strContent = strContent.substr(position + isLangFlag.length(), strContent.length());
			position = strContent.find("|");
			std::string c_szLanguage = strContent.substr(0, position);
			langIndex = atoi(c_szLanguage.c_str());
			strContent = strContent.substr(position + 1, strContent.length());
		}
		
		std::string isEmpireFlag("|empire|");
		int position2 = strContent.find(isEmpireFlag);

		if (position2 != std::string::npos)
		{
			strContent = strContent.substr(position2 + isEmpireFlag.length(), strContent.length());
			position2 = strContent.find("|");
			std::string c_szEmpire = strContent.substr(0, position2);
			empireIndex = atoi(c_szEmpire.c_str());
			strContent = strContent.substr(position2 + 1, strContent.length());
		}

		char buf[128];
		if (langIndex > 0)
		{
			if (langIndex > 11)
				langIndex = 1;

			const char* langlist[] = { "en","de","tr","pt","es","fr","ro","pl","it","cz","hu" };
			snprintf(buf, sizeof(buf), "d:/ymir work/ui/language_flag/%s.tga", langlist[langIndex - 1]);
			if (CResourceManager::Instance().IsFileExist(buf))
				pChatLine->InstanceLang.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
		}

		if (empireIndex > 0)
		{
			if (empireIndex > 3)
				empireIndex = 1;
			sprintf(buf, "d:/ymir work/ui/flag_empire/empire_%d_256.dds", empireIndex);
			if (CResourceManager::Instance().IsFileExist(buf))
				pChatLine->InstanceEmpire.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
		}
	}
#endif

	pChatLine->iType = iType;
	pChatLine->Instance.SetValue(strContent.c_str());
	pChatLine->Instance.SetTextPointer(pkDefaultFont);
	pChatLine->fAppendedTime = rApp.GetGlobalTime();
	pChatLine->SetColorAll(GetChatColor(iType));

	m_ChatLineDeque.push_back(pChatLine);
	if (m_ChatLineDeque.size() > CHAT_LINE_MAX_NUM)
	{
		SChatLine* pChatLine = m_ChatLineDeque.front();
		SChatLine::Delete(pChatLine);
		m_ChatLineDeque.pop_front();
	}

	for (TChatSetMap::iterator itor = m_ChatSetMap.begin(); itor != m_ChatSetMap.end(); ++itor)
	{
		TChatSet* pChatSet = &(itor->second);
		//pChatLine->SetColor(itor->first, GetChatColor(iType));

		// Edit Mode 를 억지로 끼워 맞추기 위해 추가
		if (BOARD_STATE_EDIT == pChatSet->m_iBoardState)
		{
			ArrangeShowingChat(itor->first);
		}
		else// if (BOARD_STATE_VIEW == pChatSet->m_iBoardState)
		{
			pChatSet->m_ShowingChatLineList.push_back(pChatLine);
			if (pChatSet->m_ShowingChatLineList.size() > CHAT_LINE_MAX_NUM)
			{
				pChatSet->m_ShowingChatLineList.pop_front();
			}
		}
	}
#endif
}

void CPythonChat::FlushDelayedChatsIfNeeded() {
    DWORD dwCurrentTime = ELTimer_GetMSec();
    if (!m_vecDelayedChatMessages.empty() && dwCurrentTime >= m_dwNextFlushTime) {
        // Only flush the first message in the queue
        const auto& delayedChat = m_vecDelayedChatMessages.front();
        DirectAppendChat(delayedChat.iType, delayedChat.strChat.c_str());
        m_vecDelayedChatMessages.erase(m_vecDelayedChatMessages.begin()); // Remove the message that was just flushed

        // Set the time for the next flush
        m_dwNextFlushTime = dwCurrentTime + DELAY_FLUSH_INTERVAL;
    }
}

void CPythonChat::DirectAppendChat(int iType, const char * c_szChat)
{
	if (strstr(c_szChat, "#ebvs"))
		return;
	// DEFAULT_FONT
	//static CResource * s_pResource = CResourceManager::Instance().GetResourcePointer(g_strDefaultFontName.c_str());

	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());
	if (!pkDefaultFont)
	{
		TraceError("CPythonChat::AppendChat - CANNOT_FIND_DEFAULT_FONT");
		return;
	}
	// END_OF_DEFAULT_FONT

	std::string str = c_szChat;
	std::string findl1 = "|Hitem:";
	std::size_t find1 = 0;

	std::size_t pos1 = 0;
	for (int i = 0; i < str.length(); i++)
	{
		find1 = str.find(findl1, i);;
		if (find1 != std::string::npos)
		{
			pos1 = find1 + findl1.length();
			i = find1 + findl1.length();

			if (str.length() - pos1 <= 0 || str.length() - pos1 >= str.length())
				break;

			const std::string item_vnumabc = str.substr(pos1, str.length() - pos1);		// |cffffc700|Hitem:12b:1:1:1:0:0:48:25:47:-6|h[Sabie luna plina+9]|h|r 
			const std::size_t pos2 = item_vnumabc.find(":");
			if (pos2 == std::string::npos)
				break;
			const std::string item_vnum = item_vnumabc.substr(0, pos2);
			char * p;
			long n = strtol(item_vnum.c_str(), &p, 16);
			if (*p != 0)
			{
				TraceError("%s a number %s", item_vnum, str);
			}
			else
			{
				const std::size_t pos_1 = item_vnumabc.find("|h[");
				const std::size_t pos_2 = item_vnumabc.find("]|h");
				if (pos_1 != std::string::npos && pos_2 != std::string::npos)
				{
					std::string item_name = item_vnumabc.substr(pos_1 + 3, pos_2 - pos_1 - 3);
					CItemData* pItemData = NULL;
					if (CItemManager::Instance().GetItemDataPointer(n, &pItemData))
					{
						if (strcmp(item_name.c_str(), pItemData->GetName()) != 0)
						{
							str = str.replace(str.find(item_name), item_name.length(), pItemData->GetName());
							c_szChat = str.c_str();
						}
					}
				}
				else
					break;
			}
		}
	}

	IAbstractApplication& rApp=IAbstractApplication::GetSingleton();

#ifdef __CHAT_SETTINGS__
	if (m_ChatLineNewDeque.size())
	{
		std::string strContent = c_szChat;

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
		BYTE empireIndex = 0;
		BYTE countryIndex = 0;
		if (iType == CHAT_TYPE_SHOUT)
		{
			std::string isLangFlag("|lang|");
			int position = strContent.find(isLangFlag);
			if (position != std::string::npos)
			{
				strContent = strContent.substr(position + isLangFlag.length(), strContent.length());
				position = strContent.find("|");
				std::string c_szLanguage = strContent.substr(0, position);
				countryIndex = atoi(c_szLanguage.c_str());
				if (countryIndex > 11 || countryIndex == 0)
					countryIndex = 1;
				strContent = strContent.substr(position + 1, strContent.length());

			}
			std::string isEmpireFlag("|empire|");
			position = strContent.find(isEmpireFlag);
			if (position != std::string::npos)
			{
				strContent = strContent.substr(position + isEmpireFlag.length(), strContent.length());
				position = strContent.find("|");
				std::string c_szEmpire = strContent.substr(0, position);
				empireIndex = atoi(c_szEmpire.c_str());
				strContent = strContent.substr(position + 1, strContent.length());
			}
		}
		
#endif
		for (auto it = m_ChatLineNewDeque.begin(); it != m_ChatLineNewDeque.end(); ++it)
		{
			SChatLine* pChatLine = SChatLine::New();

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
			pChatLine->empireIndex = empireIndex;
			pChatLine->countryIndex = countryIndex;
			if (iType == CHAT_TYPE_SHOUT)
			{
				if (pChatLine->countryIndex != 0)
				{
					const char* langlist[] = { "en","de","tr","pt","es","fr","ro","pl","it","cz","hu" };
					char buf[128];
					sprintf(buf, "d:/ymir work/ui/language_flag/%s.tga", langlist[pChatLine->countryIndex-1]);
					if (CResourceManager::Instance().IsFileExist(buf, __FUNCTION__))
						pChatLine->InstanceLang.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
				}
				if (pChatLine->empireIndex != 0)
				{
					char buf[128];
					sprintf(buf, "d:/ymir work/ui/flag_empire/empire_%d_256.dds", pChatLine->empireIndex);
					if (CResourceManager::Instance().IsFileExist(buf, __FUNCTION__))
						pChatLine->InstanceEmpire.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
				}
				//TraceError("country %d empire %d", pChatLine->countryIndex, pChatLine->empireIndex);
			}
#endif
			pChatLine->text = strContent;
			pChatLine->iType = iType;
			pChatLine->Instance.SetValue(strContent.c_str());
			pChatLine->Instance.SetTextPointer(pkDefaultFont);
			pChatLine->fAppendedTime = rApp.GetGlobalTime();
			pChatLine->SetColorAll(GetChatColor(iType));
			it->second.push_back(pChatLine);
			if (it->second.size() > CHAT_LINE_MAX_NUM)
			{
				SChatLine* pChatLine = it->second.front();
				SChatLine::Delete(pChatLine);
				it->second.pop_front();
			}
			auto itChatSet = m_ChatSetMap.find(it->first);
			if (itChatSet != m_ChatSetMap.end())
			{
				TChatSet* pChatSet = &(itChatSet->second);
				if (BOARD_STATE_EDIT == pChatSet->m_iBoardState)
					ArrangeShowingChat(itChatSet->first);
				else
				{
					pChatSet->m_ShowingChatLineList.push_back(pChatLine);
					if (pChatSet->m_ShowingChatLineList.size() > CHAT_LINE_MAX_NUM)
						pChatSet->m_ShowingChatLineList.pop_front();
					ArrangeShowingChat(itChatSet->first);
				}
			}

		}
	}
#else
	SChatLine* pChatLine = SChatLine::New();
	std::string strContent = c_szChat;

#ifdef ENABLE_MULTILANGUAGE_SYSTEM
	if (iType == CHAT_TYPE_SHOUT)
	{
		BYTE langIndex = 0;
		BYTE empireIndex = 0;


		std::string isLangFlag("|lang|");
		int position = strContent.find(isLangFlag);

		if (position != std::string::npos)
		{
			strContent = strContent.substr(position + isLangFlag.length(), strContent.length());
			position = strContent.find("|");
			std::string c_szLanguage = strContent.substr(0, position);
			langIndex = atoi(c_szLanguage.c_str());
			strContent = strContent.substr(position + 1, strContent.length());
		}
		
		std::string isEmpireFlag("|empire|");
		int position2 = strContent.find(isEmpireFlag);

		if (position2 != std::string::npos)
		{
			strContent = strContent.substr(position2 + isEmpireFlag.length(), strContent.length());
			position2 = strContent.find("|");
			std::string c_szEmpire = strContent.substr(0, position2);
			empireIndex = atoi(c_szEmpire.c_str());
			strContent = strContent.substr(position2 + 1, strContent.length());
		}

		char buf[128];
		if (langIndex > 0)
		{
			if (langIndex > 11)
				langIndex = 1;

			const char* langlist[] = { "en","de","tr","pt","es","fr","ro","pl","it","cz","hu" };
			snprintf(buf, sizeof(buf), "d:/ymir work/ui/language_flag/%s.tga", langlist[langIndex - 1]);
			if (CResourceManager::Instance().IsFileExist(buf))
				pChatLine->InstanceLang.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
		}

		if (empireIndex > 0)
		{
			if (empireIndex > 3)
				empireIndex = 1;
			sprintf(buf, "d:/ymir work/ui/flag_empire/empire_%d_256.dds", empireIndex);
			if (CResourceManager::Instance().IsFileExist(buf))
				pChatLine->InstanceEmpire.SetImagePointer((CGraphicImage*)CResourceManager::Instance().GetResourcePointer(buf));
		}
	}
#endif

	pChatLine->iType = iType;
	pChatLine->Instance.SetValue(strContent.c_str());
	pChatLine->Instance.SetTextPointer(pkDefaultFont);
	pChatLine->fAppendedTime = rApp.GetGlobalTime();
	pChatLine->SetColorAll(GetChatColor(iType));

	m_ChatLineDeque.push_back(pChatLine);
	if (m_ChatLineDeque.size() > CHAT_LINE_MAX_NUM)
	{
		SChatLine* pChatLine = m_ChatLineDeque.front();
		SChatLine::Delete(pChatLine);
		m_ChatLineDeque.pop_front();
	}

	for (TChatSetMap::iterator itor = m_ChatSetMap.begin(); itor != m_ChatSetMap.end(); ++itor)
	{
		TChatSet* pChatSet = &(itor->second);
		//pChatLine->SetColor(itor->first, GetChatColor(iType));

		// Edit Mode 를 억지로 끼워 맞추기 위해 추가
		if (BOARD_STATE_EDIT == pChatSet->m_iBoardState)
		{
			ArrangeShowingChat(itor->first);
		}
		else// if (BOARD_STATE_VIEW == pChatSet->m_iBoardState)
		{
			pChatSet->m_ShowingChatLineList.push_back(pChatLine);
			if (pChatSet->m_ShowingChatLineList.size() > CHAT_LINE_MAX_NUM)
			{
				pChatSet->m_ShowingChatLineList.pop_front();
			}
		}
	}
#endif


}

void CPythonChat::AppendChatWithDelay(int iType, const char * c_szChat, int iDelay)
{
	TWaitChat WaitChat;
	WaitChat.iType = iType;
	WaitChat.strChat = c_szChat;
	WaitChat.dwAppendingTime = CTimer::Instance().GetCurrentMillisecond() + iDelay;
	m_WaitChatList.push_back(WaitChat);
}

DWORD CPythonChat::GetChatColor(int iType)
{
	if (iType<CHAT_TYPE_MAX_NUM)
	{
		return m_akD3DXClrChat[iType];
	}

	return D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
}

void CPythonChat::IgnoreCharacter(const char * c_szName)
{
	TIgnoreCharacterSet::iterator itor = m_IgnoreCharacterSet.find(c_szName);
	// NOTE : 이미 차단 중이라면..
	if (m_IgnoreCharacterSet.end() != itor)
	{
		m_IgnoreCharacterSet.erase(itor);
	}
	// NOTE : 차단이 되지 않은 캐릭터라면..
	else
	{
		m_IgnoreCharacterSet.insert(c_szName);
	}
}

BOOL CPythonChat::IsIgnoreCharacter(const char * c_szName)
{
	TIgnoreCharacterSet::iterator itor = m_IgnoreCharacterSet.find(c_szName);

	if (m_IgnoreCharacterSet.end() == itor)
		return FALSE;

	return TRUE;
}

CWhisper * CPythonChat::CreateWhisper(const char * c_szName)
{
	CWhisper * pWhisper = CWhisper::New();
	m_WhisperMap.insert(TWhisperMap::value_type(c_szName, pWhisper));
	return pWhisper;
}

void CPythonChat::AppendWhisper(int iType, const char * c_szName, const char * c_szChat)
{
	TWhisperMap::iterator itor = m_WhisperMap.find(c_szName);

	CWhisper * pWhisper;
	if (itor == m_WhisperMap.end())
	{
		pWhisper = CreateWhisper(c_szName);
#ifdef ENABLE_DISCORD_STUFF
		pWhisper->lineCount = 0;
#endif
	}
	else
	{
		pWhisper = itor->second;
	}

	pWhisper->AppendChat(iType, c_szChat);
#ifdef ENABLE_DISCORD_STUFF
	++pWhisper->lineCount;
	CPythonApplication::Instance().ChangeIcon();
#endif
}

void CPythonChat::ClearWhisper(const char * c_szName)
{
	TWhisperMap::iterator itor = m_WhisperMap.find(c_szName);

	if (itor != m_WhisperMap.end())
	{
		CWhisper * pWhisper = itor->second;
		CWhisper::Delete(pWhisper);

		m_WhisperMap.erase(itor);
	}
}

BOOL CPythonChat::GetWhisper(const char * c_szName, CWhisper ** ppWhisper)
{
	TWhisperMap::iterator itor = m_WhisperMap.find(c_szName);

	if (itor == m_WhisperMap.end())
		return FALSE;

	*ppWhisper = itor->second;
#ifdef ENABLE_DISCORD_STUFF
	if (itor->second->lineCount > 0)
	{
		itor->second->lineCount = 0;
		CPythonApplication::Instance().ChangeIcon();
	}
#endif
	return TRUE;
}

void CPythonChat::InitWhisper(PyObject * ppyObject)
{
	TWhisperMap::iterator itor = m_WhisperMap.begin();
	for (; itor != m_WhisperMap.end(); ++itor)
	{
		std::string strName = itor->first;
		PyCallClassMemberFunc(ppyObject, "MakeWhisperButton", Py_BuildValue("(s)", strName.c_str()));
	}
}

void CPythonChat::__DestroyWhisperMap()
{
	TWhisperMap::iterator itor = m_WhisperMap.begin();
	for (; itor != m_WhisperMap.end(); ++itor)
	{
		CWhisper::Delete(itor->second);
	}
	m_WhisperMap.clear();
}

void CPythonChat::Close()
{
	TChatSetMap::iterator itor = m_ChatSetMap.begin();
	for (; itor != m_ChatSetMap.end(); ++itor)
	{
		TChatSet & rChatSet = itor->second;
		TChatLineList * pLineList = &(rChatSet.m_ShowingChatLineList);
		for (TChatLineList::iterator itor = pLineList->begin(); itor != pLineList->end(); ++itor)
		{
			TChatLine * pChatLine = (*itor);
			pChatLine->fAppendedTime = 0.0f;
		}
	}
}

void CPythonChat::Destroy()
{
	__DestroyWhisperMap();

	m_ShowingChatLineList.clear();
	m_ChatSetMap.clear();
	m_ChatLineDeque.clear();
#ifdef __CHAT_SETTINGS__
	m_ChatLineNewDeque.clear();
#endif
	SChatLine::DestroySystem();
	CWhisper::DestroySystem();

	__Initialize();
}

void CPythonChat::__Initialize()
{
	m_akD3DXClrChat[CHAT_TYPE_TALKING]		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_INFO]			= D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_NOTICE]		= D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_PARTY]		= D3DXCOLOR(0.542f, 1.0f, 0.949f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_GUILD]		= D3DXCOLOR(0.906f, 0.847f, 1.0f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_COMMAND]		= D3DXCOLOR(0.658f, 1.0f, 0.835f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_SHOUT]		= D3DXCOLOR(0.658f, 1.0f, 0.835f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_WHISPER]		= D3DXCOLOR(0xff4AE14A);
	m_akD3DXClrChat[CHAT_TYPE_BIG_NOTICE]	= D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_MONARCH_NOTICE]	= D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
	// m_akD3DXClrChat[CHAT_TYPE_UNK_10]		= D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
#ifdef ENABLE_DICE_SYSTEM
	m_akD3DXClrChat[CHAT_TYPE_DICE_INFO]	= D3DXCOLOR(0xFFcc00cc);
#endif
#ifdef dracaryS_DUNGEON_LIB
	m_akD3DXClrChat[CHAT_TYPE_MISSION] = D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
#endif
#ifdef __CHAT_SETTINGS__
	m_akD3DXClrChat[CHAT_TYPE_EXP_INFO] = D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_ITEM_INFO] = D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_MONEY_INFO] = D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);

	TChatLineDeque map_vec;
	m_ChatLineNewDeque.emplace(0, map_vec);
	m_ChatLineNewDeque.emplace(1, map_vec);
	m_ChatSetMap.emplace(0, TChatSet());//normal chat
	m_ChatSetMap.emplace(1, TChatSet());//log chat
#endif
}

CPythonChat::CPythonChat()
{
	__Initialize();
}

CPythonChat::~CPythonChat()
{
#ifdef __CHAT_SETTINGS__
	m_ChatSetMap.clear();
#endif
	assert(m_ChatLineDeque.empty());
	assert(m_ShowingChatLineList.empty());
	assert(m_ChatSetMap.empty());
	assert(m_WhisperMap.empty());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CDynamicPool<CWhisper> CWhisper::ms_kPool;

CWhisper* CWhisper::New()
{
	return ms_kPool.Alloc();
}

void CWhisper::Delete(CWhisper* pkWhisper)
{
	pkWhisper->Destroy();
	ms_kPool.Free(pkWhisper);
}

void CWhisper::DestroySystem()
{
	ms_kPool.Destroy();

	SChatLine::DestroySystem();
}

void CWhisper::SetPosition(float fPosition)
{
	m_fcurPosition = fPosition;
	__ArrangeChat();
}

void CWhisper::SetBoxSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	for (TChatLineDeque::iterator itor = m_ChatLineDeque.begin(); itor != m_ChatLineDeque.end(); ++itor)
	{
		TChatLine * pChatLine = *itor;
		pChatLine->Instance.SetLimitWidth(fWidth);
	}
}

void CWhisper::AppendChat(int iType, const char * c_szChat)
{
	// DEFAULT_FONT
	//static CResource * s_pResource = CResourceManager::Instance().GetResourcePointer(g_strDefaultFontName.c_str());

#if defined(LOCALE_SERVICE_YMIR) || defined(LOCALE_SERVICE_JAPAN) || defined(LOCALE_SERVICE_HONGKONG) || defined(LOCALE_SERVICE_TAIWAN) || defined(LOCALE_SERVICE_NEWCIBN)
	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());
#else
	CGraphicText* pkDefaultFont = (iType == CPythonChat::WHISPER_TYPE_GM) ? static_cast<CGraphicText*>(DefaultItalicFont_GetResource()) : static_cast<CGraphicText*>(DefaultFont_GetResource());
#endif

	if (!pkDefaultFont)
	{
		TraceError("CWhisper::AppendChat - CANNOT_FIND_DEFAULT_FONT");
		return;
	}
	// END_OF_DEFAULT_FONT

	SChatLine * pChatLine = SChatLine::New();
	pChatLine->Instance.SetValue(c_szChat);

	// DEFAULT_FONT
	pChatLine->Instance.SetTextPointer(pkDefaultFont);
	// END_OF_DEFAULT_FONT

	pChatLine->Instance.SetLimitWidth(m_fWidth);
	pChatLine->Instance.SetMultiLine(TRUE);

	switch(iType)
	{
		case CPythonChat::WHISPER_TYPE_SYSTEM:
			pChatLine->Instance.SetColor(D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f));
			break;
		case CPythonChat::WHISPER_TYPE_GM:
			pChatLine->Instance.SetColor(D3DXCOLOR(1.0f, 0.632f, 0.0f, 1.0f));
			break;
		case CPythonChat::WHISPER_TYPE_CHAT:
		default:
			pChatLine->Instance.SetColor(0xffffffff);
			break;
	}

	m_ChatLineDeque.push_back(pChatLine);

	__ArrangeChat();
}

void CWhisper::__ArrangeChat()
{
	for (TChatLineDeque::iterator itor = m_ChatLineDeque.begin(); itor != m_ChatLineDeque.end(); ++itor)
	{
		TChatLine * pChatLine = *itor;
		pChatLine->Instance.Update();
	}
}

void CWhisper::Render(float fx, float fy)
{
	float fHeight = fy + m_fHeight;

	int iViewCount = int(m_fHeight / m_fLineStep) - 1;
	int iLineCount = int(m_ChatLineDeque.size());
	int iStartLine = -1;
	if (iLineCount > iViewCount)
	{
		iStartLine = int(float(iLineCount-iViewCount) * m_fcurPosition) + iViewCount - 1;
	}
	else if (!m_ChatLineDeque.empty())
	{
		iStartLine = iLineCount - 1;
	}

	RECT Rect = { fx, fy, fx+m_fWidth, fy+m_fHeight };

#ifdef USE_CLIP_MASK
    if (GetDefaultCodePage() == CP_ARABIC)
    {
        Rect.left = fx - m_fWidth;
        Rect.right = fx;
    }
#endif

	for (int i = iStartLine; i >= 0; --i)
	{
		assert(i >= 0 && i < int(m_ChatLineDeque.size()));
		TChatLine * pChatLine = m_ChatLineDeque[i];

		WORD wLineCount = pChatLine->Instance.GetTextLineCount();
		fHeight -= wLineCount * m_fLineStep;

		pChatLine->Instance.SetPosition(fx, fHeight);
		pChatLine->Instance.Render(&Rect);

		if (fHeight < fy)
			break;
	}
}

void CWhisper::__Initialize()
{
	m_fLineStep = 15.0f;
	m_fWidth = 300.0f;
	m_fHeight = 120.0f;
	m_fcurPosition = 1.0f;
#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
	m_isRead = false;
#endif
}

void CWhisper::Destroy()
{
	std::for_each(m_ChatLineDeque.begin(), m_ChatLineDeque.end(), SChatLine::Delete);
	m_ChatLineDeque.clear();
	m_ShowingChatLineList.clear();
}

CWhisper::CWhisper()
{
	__Initialize();
}

CWhisper::~CWhisper()
{
	Destroy();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CDynamicPool<CWhisper::SChatLine> CWhisper::SChatLine::ms_kPool;

CWhisper::SChatLine* CWhisper::SChatLine::New()
{
	return ms_kPool.Alloc();
}

void CWhisper::SChatLine::Delete(CWhisper::SChatLine* pkChatLine)
{
	pkChatLine->Instance.Destroy();
	ms_kPool.Free(pkChatLine);
}

void CWhisper::SChatLine::DestroySystem()
{
	ms_kPool.Destroy();
}

#ifdef __CHAT_SETTINGS__
void CPythonChat::DeleteChatSet(DWORD dwID)
{
	auto it = m_ChatSetMap.find(dwID);
	if (it != m_ChatSetMap.end())
		m_ChatSetMap.erase(it);

	auto itChat = m_ChatLineNewDeque.find(dwID);
	if (itChat != m_ChatLineNewDeque.end())
	{
		if (itChat->second.size())
		{
			auto itChatLine = itChat->second.begin();
			while (itChatLine != itChat->second.end())
			{
				SChatLine* chatLine = *(itChatLine);
				SChatLine::Delete(chatLine);
				itChatLine = itChat->second.erase(itChatLine);
			}
		}
		itChat->second.clear();
		m_ChatLineNewDeque.erase(itChat);
	}
}
void CPythonChat::EnableCountryMode(DWORD dwID, int iMode)
{
	TChatSet* pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iCountryMode[iMode] = TRUE;
	ArrangeShowingChat(dwID);
}
void CPythonChat::DisableCountryMode(DWORD dwID, int iMode)
{
	TChatSet* pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iCountryMode[iMode] = FALSE;
	ArrangeShowingChat(dwID);
}
#endif

#ifdef ENABLE_DISCORD_STUFF
DWORD CPythonChat::GetNewChatCount()
{
	DWORD count = 0;
	TWhisperMap::iterator itor = m_WhisperMap.begin();
	for (; itor != m_WhisperMap.end(); ++itor)
		count += itor->second->lineCount;
	return count;
}
#endif

#ifdef ENABLE_BADGE_NOTIFICATION_MANAGER
void CPythonChat::SetUnreadAllWhispers(const std::string& target_name) const
{
	for (const auto& whisper : m_WhisperMap)
	{
		if (whisper.first == target_name || !whisper.second->IsRead())
			continue;

		whisper.second->SetUnread();
	}
}
#endif
