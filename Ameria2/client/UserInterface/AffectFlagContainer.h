#pragma once
//#define DEBUG_AFFECT_BITS

class CAffectFlagContainer
{
public:
	enum
	{
		BIT_SIZE = 64 + 32, // added 4 more bytes
		BYTE_SIZE = BIT_SIZE / 8 + (1 * ((BIT_SIZE & 7) ? 1 : 0)),
	};

public:
	CAffectFlagContainer();
	~CAffectFlagContainer();

	void Clear();
	void CopyInstance(const CAffectFlagContainer& c_rkAffectContainer);
	void Set(uint32_t uPos, bool isSet);
	bool IsSet(uint32_t uPos) const;

#ifdef DEBUG_AFFECT_BITS
	void DumpBits() const;
#endif

	void CopyData(uint32_t uPos, uint32_t uByteSize, const void* c_pvData);

	void ConvertToPosition(unsigned* uRetX, unsigned* uRetY) const;

private:
	typedef unsigned char Element;

	Element m_aElement[BYTE_SIZE];
};
