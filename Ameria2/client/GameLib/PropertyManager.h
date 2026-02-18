#ifndef METIN2_CLIENT_GAMELIB_PROPERTYMANAGER_H
#define METIN2_CLIENT_GAMELIB_PROPERTYMANAGER_H

#if VSTD_HAS_PRAGMA_ONCE
#pragma once
#endif

#include "Property.h"
#include <set>
#include <map>

class CPropertyManager : public CSingleton<CPropertyManager>
{
    public:
        CPropertyManager();
        virtual ~CPropertyManager();

        void Clear();

        bool Initialize(const std::string& listFileName);
        bool Register(const char* c_pszFileName, CProperty** ppProperty = NULL);

        bool Get(uint32_t dwCRC, CProperty** ppProperty);
        bool Get(const char* c_pszFileName, CProperty** ppProperty);

    protected:
        typedef std::map<uint32_t, std::unique_ptr<CProperty>> TPropertyCRCMap;
        typedef std::set<uint32_t> TCRCSet;

        bool m_bInitialized;
        TPropertyCRCMap m_PropertyByCRCMap;
        TCRCSet m_ReservedCRCSet;
};
#endif // METIN2_CLIENT_GAMELIB_PROPERTYMANAGER_H
