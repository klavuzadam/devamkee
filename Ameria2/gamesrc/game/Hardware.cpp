#include <memory>
#include "stdafx.h"
#ifdef __HARDWARE_BAN__
#include "../common/tables.h"
#include "db.h"
#include "Hardware.h"
#include "aes.h"

// The key we use for that enc/dec
namespace
{
	const std::vector<unsigned char> vAESDecKey = {'K', 'E', 'Z', '5', 'V', 'h', 'd', '7', 'k', 'b', 'U', '4', 'Z', 'z', 'E', '2', 's', 'X', 'q', 'W', 'n', '8', '9', 'E', 'E', 'X', 'q', 'V', 'm', 'F', 'k'};
}

CheckHardware::CheckHardware() : spAES{new AES{AESKeyLength::AES_256}}
{
	bans_cointainer.clear();
	LoadBans();
}

CheckHardware::~CheckHardware()
{
	Destroy();
}

void CheckHardware::Destroy()
{
	bans_cointainer.clear();
}

static size_t HWIDDecryptSize(std::vector<BYTE> & vBuf)
{
	// Checking if it consist at least 4 chars
	if (vBuf.size() < 2)
		return 0;

	// Concat&return size if no error
	std::string sSize;

	try
	{
		std::transform(vBuf.begin(), vBuf.begin() + 2, std::back_inserter(sSize), [] (const BYTE & c) { return static_cast<char>(c); });
		// Scrap that up
		vBuf.erase(vBuf.begin(), vBuf.begin() + 2);
		return std::stoi(sSize);
	}
	catch(...)
	{
		return 0;
	}
}

std::string CheckHardware::DecodeHWID(const std::string & hdid)
{
	// Try to decrypt
	std::vector<BYTE> decrBuff = spAES->DecryptECB(std::vector<unsigned char>(hdid.begin(), hdid.end()), vAESDecKey);

	if (decrBuff.size() < 2)
		return "";

	std::string sDecryptedString(decrBuff.begin() + 2, decrBuff.end());
	int iStringRealSize = strlen(sDecryptedString.c_str());
	
	// If decoded size does not match the standard that means packet was spoofed
	auto stDecryptSize = HWIDDecryptSize(decrBuff);
	if (!stDecryptSize || stDecryptSize != iStringRealSize)
		return "";

	return sDecryptedString;
}

void CheckHardware::AddBan(const std::string & id)
{
	if (!id.size())
		return;

	std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("REPLACE INTO account.HardwareBan VALUES ('%s')", id.c_str()));
	bans_cointainer.insert(id);
}

void CheckHardware::RemoveBan(std::string id)
{
	std::unique_ptr<SQLMsg> pMsgUpdate(DBManager::instance().DirectQuery("DELETE FROM account.HardwareBan WHERE id = '%s'", id.c_str()));
	if (bans_cointainer.find(id) != bans_cointainer.end())
		bans_cointainer.erase(bans_cointainer.find(id));
}

void CheckHardware::LoadBans()
{
	std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("SELECT id FROM account.HardwareBan"));

	if (msg->Get()->uiNumRows == 0)
		return;
	
	MYSQL_RES* pRes = msg->Get()->pSQLResult;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(pRes)))
		bans_cointainer.insert(row[0]);
}
#endif

