#include "StdAfx.h"
#include "../EterPack/EterPackManager.h"
#include "../EterBase/tinyxml2.h"
#include "PropertyManager.h"
#include "Property.h"

CPropertyManager::CPropertyManager()
{
	m_bInitialized = false;
}

CPropertyManager::~CPropertyManager()
{
	Clear();
}

bool CPropertyManager::Initialize(const std::string& listFilename)
{
	if (m_bInitialized) {
		return true;
	}

	CMappedFile kPropertyXML;
	PBYTE pbPropertyXML;
	if (CEterPackManager::Instance().Get(kPropertyXML, listFilename.c_str(), (LPCVOID*)&pbPropertyXML, __FUNCTION__))
	{
		char* pszXML = new char[kPropertyXML.Size() + 1];
		memcpy(pszXML, pbPropertyXML, kPropertyXML.Size());
		pszXML[kPropertyXML.Size()] = 0;
		std::stringstream kXML;
		kXML << pszXML;


		tinyxml2::XMLDocument doc;
		if (doc.Parse(pszXML) != tinyxml2::XML_SUCCESS) {
			TraceError("CPropertyManager::Initialize: Failed to load %s (%s)", listFilename.c_str(), doc.ErrorName());
			delete[] pszXML;
			return false;
		}

		tinyxml2::XMLElement *levelElement = doc.FirstChildElement("PropertyList");
		if (!levelElement)
			return true;

		for (tinyxml2::XMLElement* child = levelElement->FirstChildElement("Property"); child != NULL; child = child->NextSiblingElement())
		{
			auto pProperty = std::make_unique<CProperty>(child->Attribute("filename"));
			if (!pProperty->ReadFromXML(child->Attribute("crc")))
			{
				TraceError("CPropertyManager::Initialize: Cannot register '%s'!", child->Attribute("filename"));
				continue;
			}

            CProperty *rawPtr = pProperty.get();

			uint32_t dwCRC = pProperty->GetCRC();

			auto itor = m_PropertyByCRCMap.find(dwCRC);
			if (m_PropertyByCRCMap.end() != itor)
			{
#ifdef _DEBUG
				Tracef("CPropertyManager::Initialize: Property already registered, replace %s to %s\n",
					itor->second->GetFileName(), child->Attribute("filename"));
#endif

				itor->second = std::move(pProperty);
			}
			else
			{
				m_PropertyByCRCMap.insert(TPropertyCRCMap::value_type(dwCRC, std::move(pProperty)));
			}

			const tinyxml2::XMLAttribute* pAttrib = child->FirstAttribute();
			while (pAttrib)
			{
				CTokenVector kVec;
				kVec.push_back(pAttrib->Value());
				rawPtr->PutVector(pAttrib->Name(), kVec);
				pAttrib = pAttrib->Next();
			}
		}

		delete[] pszXML;
	}
	else {
		return false;
	}

	m_bInitialized = true;
	return true;
}

bool CPropertyManager::Register(const char* c_pszFileName, CProperty** ppProperty)
{
	CMappedFile file;
	LPCVOID c_pvData;

	if (!CEterPackManager::Instance().Get(file, c_pszFileName, &c_pvData, __FUNCTION__))
	{
		return false;
	}

	auto pProperty = std::make_unique<CProperty>(c_pszFileName);
	if (!pProperty->ReadFromMemory(c_pvData, file.Size(), c_pszFileName))
    {
		return false;
	}

	uint32_t dwCRC = pProperty->GetCRC();

    CProperty *rawPtr = pProperty.get();

	auto itor = m_PropertyByCRCMap.find(dwCRC);

	if (m_PropertyByCRCMap.end() != itor)
	{
		//Tracef("Property already registered, replace %s to %s\n",
		//	itor->second->GetFileName(),
		//	c_pszFileName);

		itor->second = std::move(pProperty);
	} else {
		m_PropertyByCRCMap.emplace(TPropertyCRCMap::value_type(dwCRC, std::move(pProperty)));
	}

	if (ppProperty)
    {
		*ppProperty = rawPtr;
	}

	return true;
}

bool CPropertyManager::Get(const char* c_pszFileName, CProperty** ppProperty)
{
	return Register(c_pszFileName, ppProperty);
}

bool CPropertyManager::Get(uint32_t dwCRC, CProperty** ppProperty)
{
	auto itor = m_PropertyByCRCMap.find(dwCRC);

	if (m_PropertyByCRCMap.end() == itor)
		return false;

	*ppProperty = itor->second.get();
	return true;
}

void CPropertyManager::Clear()
{
    m_PropertyByCRCMap.clear();
}
