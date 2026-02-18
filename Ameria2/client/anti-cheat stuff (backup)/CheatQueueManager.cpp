#include "StdAfx.h"
#include "CheatQueueManager.h"
#include "PythonNetworkStream.h"

#include <nb30.h>
#include <XORstr.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>


CCheatDetectQueueMgr::CCheatDetectQueueMgr()
{
	m_pCheckTimer.reset();
}
CCheatDetectQueueMgr::~CCheatDetectQueueMgr()
{
}

void CCheatDetectQueueMgr::Initialize()
{
	m_kQueCheatQuarentine = std::make_shared< CQueue <std::shared_ptr <SCheatQueueCtx> > >();
//	m_kQueCheatQuarentine->BindCallback(std::bind(&CCheatDetectQueueMgr::OnQueueCall, this, std::placeholders::_1));
}

std::string CCheatDetectQueueMgr::GetHardwareIdentifier()
{
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER NameBuff[30];
	} ASTAT, *PASTAT;
	ASTAT pAdapter = { 0 };

	LANA_ENUM laLanaEnum = { 0 };

	NCB ncb = { 0 };
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (LPBYTE)&laLanaEnum;
	ncb.ncb_length = sizeof(laLanaEnum);

	auto pRet = Netbios(&ncb);
	if (pRet == NRC_GOODRET)
	{
		for (int lana = 0; lana < laLanaEnum.length; lana++)
		{
			ncb.ncb_command = NCBRESET;
			ncb.ncb_lana_num = laLanaEnum.lana[lana];
			pRet = Netbios(&ncb);
			if (pRet == NRC_GOODRET)
				break;
		}
		if (pRet == NRC_GOODRET)
		{
			memset(&ncb, 0, sizeof(ncb));

			ncb.ncb_command = NCBASTAT;
			ncb.ncb_lana_num = laLanaEnum.lana[0];
			strcpy((char*)ncb.ncb_callname, "*");
			ncb.ncb_buffer = (LPBYTE)&pAdapter;
			ncb.ncb_length = sizeof(pAdapter);

			pRet = Netbios(&ncb);
			if (pRet == NRC_GOODRET)
			{
				char szMacAddr[128];
				sprintf(szMacAddr, XOR("%02X-%02X-%02X-%02X-%02X-%02X"),
					pAdapter.adapt.adapter_address[0],
					pAdapter.adapt.adapter_address[1],
					pAdapter.adapt.adapter_address[2],
					pAdapter.adapt.adapter_address[3],
					pAdapter.adapt.adapter_address[4],
					pAdapter.adapt.adapter_address[5]
				);

				return szMacAddr;
			}
		}
	}
	return "";
}

bool CCheatDetectQueueMgr::AppendDetection(DWORD dwViolationID, DWORD dwSystemErrorCode, const std::string & strMessage, DWORD dwWaitMS)
{
#ifdef _DEBUG
	Tracenf("AppendDetection routine started! Violation: %u-%u Message: %s", 
		dwViolationID, dwSystemErrorCode, strMessage.c_str());
#endif

	if (m_vSentDetections.size() > 0)
	{
		for (const auto & pCurrSentItem : m_vSentDetections)
		{
			if (dwViolationID == pCurrSentItem->dwViolation && dwSystemErrorCode == pCurrSentItem->dwErrorCode && strMessage == pCurrSentItem->szMessage)
				return true;
		}
	}

	auto pCheatDetectQueueData = std::make_shared<SCheatQueueCtx>();
	if (!pCheatDetectQueueData || !pCheatDetectQueueData.get())
	{
#ifdef _DEBUG
		TraceError("Cheat queue data container can NOT allocated!");
#endif
		return false;
	}

	pCheatDetectQueueData->dwViolation = dwViolationID;
	pCheatDetectQueueData->dwErrorCode = dwSystemErrorCode;
	strcpy_s(pCheatDetectQueueData->szMessage, strMessage.c_str());

	if (m_kQueCheatQuarentine->InsertObject(pCheatDetectQueueData) == false)
	{
#ifdef _DEBUG
		TraceError("Cheat object is already attached to queue!");
#endif
		return true;
	}

	m_vSentDetections.push_back(pCheatDetectQueueData);

	if (dwWaitMS > 0)
	{
		auto bCompleted = false;
		auto pCheckTimer = CSimpleTimer<std::chrono::milliseconds>();
		while (pCheckTimer.diff() < dwWaitMS)
		{
			if (m_kQueCheatQuarentine->HasListed(pCheatDetectQueueData) == false && m_kQueCheatQuarentine->HasProcessed(pCheatDetectQueueData))
			{
				bCompleted = true;
				break;
			}
			Sleep(10);
		}

		if (bCompleted)
		{
#ifdef _DEBUG
			Tracen("Notification succesfully sent to server!");
#endif
			return true;
		}

#ifdef _DEBUG
		TraceError("Notification can NOT sent to server!");
#endif
		return false;
	}

#ifdef _DEBUG
	Tracen("Notification attached to queue!");
#endif
	return true;
}


void CCheatDetectQueueMgr::OnTick()
{
	if (m_pCheckTimer.diff() > 2000)
	{
//		m_kQueCheatQuarentine->ProcessFirstObject();

		if (m_kQueCheatQuarentine->HasWorkObject())
		{
			auto pFirstObject = m_kQueCheatQuarentine->GetFirstObject();
			if (pFirstObject && pFirstObject.get())
			{
				OnQueueCall(pFirstObject);

				m_kQueCheatQuarentine->RemoveFirstObject();
			}
		}	

		m_pCheckTimer.reset();
	}
}

void CCheatDetectQueueMgr::OnQueueCall(std::shared_ptr <SCheatQueueCtx> pData)
{
	if (pData && pData.get()) 
	{
#ifdef _DEBUG
		TraceError("Current cheat queue obj: %u-%u", pData->dwErrorCode, pData->dwViolation);
#endif

		std::string strCheatMsg = std::to_string(pData->dwViolation) + "[" + std::to_string(pData->dwErrorCode) + "]";

		if (false == CPythonNetworkStream::Instance().SendHackNotification(strCheatMsg.c_str(), pData->szMessage, GetHardwareIdentifier().c_str()))
		{
#ifdef _DEBUG
			TraceError("Current cheat queue obj: '%s' can not send to server!", strCheatMsg.c_str());
#endif
		}
	}
}

