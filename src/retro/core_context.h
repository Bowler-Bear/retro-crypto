#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>

namespace RetroCrypto
{
	enum class CryptoType
	{
		NONE,
		BTC,
		DOGE,
		ETC,
		ETH,
		NOSTR,
		XMR
	};

	enum class ContextUpdate : uint32_t
	{
		SEED = 0x00000001 << 0,
		SEED_SIZE = 0x00000001 << 1,
		MNEMONIC = 0x00000001 << 2,
		CRYPTO = 0x00000001 << 3,
		ALL = SEED | SEED_SIZE | MNEMONIC | CRYPTO
	};

	inline constexpr uint8_t operator&(const ContextUpdate& first, const ContextUpdate& second)
	{
		return static_cast<uint32_t>(first) & static_cast<uint32_t>(second);
	}

	struct ContextData
	{
		uint8_t seed[32];
		uint8_t seedSize;
		std::string mnemonic;
		CryptoType crypto;

		ContextData()
		: seed{ 0 }, seedSize(32), mnemonic(""), crypto(CryptoType::NONE)
		{
		}

		std::string getSeedAsHexString()
		{
			char hexString[64];
			for(int i = 0; i < 32; i++)
				sprintf(hexString+i*2, "%02x", seed[i]);
			return std::string(hexString);
		}
	};

	class Context
	{
	private:
		ContextData data;

	public:
		void updateData(ContextUpdate update, ContextData dataContainer);
		ContextData getData();
	};
}
#endif
