#ifndef CONTEXT_H
#define CONTEXT_H

#include <cstdint>
#include <string>

#define MAXIMUM_SEED_SIZE 64
#define DEFAULT_SEED_SIZE 32

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
	
	enum class MnemonicType
	{
		NONE,
		BIP39,
		LEGACY_MONERO_ENGLISH,
		LEGACY_MONERO_GERMAN,
		LEGACY_MONERO_SPANISH,
		LEGACY_MONERO_FRENCH,
		LEGACY_MONERO_ITALIAN,
		LEGACY_MONERO_DUTCH,
		LEGACY_MONERO_PORTUGUESE,
		LEGACY_MONERO_RUSSIAN,
		LEGACY_MONERO_JAPANESE,
		LEGACY_MONERO_CHINESE,
		LEGACY_MONERO_ESPERANTO,
		LEGACY_MONERO_LOJBAN
	};

	enum ContextUpdate : uint32_t
	{
		SEED = 0x00000001 << 0,
		SEED_SIZE = 0x00000001 << 1,
		MNEMONIC = 0x00000001 << 2,
		CRYPTO = 0x00000001 << 3,
		MNEMONIC_TYPE = 0x00000001 << 4,
		ALL = SEED | SEED_SIZE | MNEMONIC | CRYPTO | MNEMONIC_TYPE
	};

	inline constexpr ContextUpdate operator&(const ContextUpdate& first, const ContextUpdate& second)
	{
		return static_cast<ContextUpdate>(static_cast<uint32_t>(first) & static_cast<uint32_t>(second));
	};

	inline constexpr ContextUpdate operator|(const ContextUpdate& first, const ContextUpdate& second)
	{
		return static_cast<ContextUpdate>(static_cast<uint32_t>(first) | static_cast<uint32_t>(second));
	};

	struct ContextData
	{
		uint8_t seed[MAXIMUM_SEED_SIZE];
		uint8_t seedSize;
		MnemonicType mnemonicType;
		std::string mnemonic;
		CryptoType crypto;

		ContextData()
		: seed{ 0 }, seedSize(DEFAULT_SEED_SIZE), mnemonicType(MnemonicType::NONE), mnemonic(""), crypto(CryptoType::NONE)
		{
		}

		ContextData(const uint8_t* inSeed, const uint8_t inSeedSize)
		: ContextData()
		{
			seedSize = inSeedSize;
			setSeed(inSeed, inSeedSize);
		}

		ContextData(MnemonicType inMnemonicType)
		: ContextData()
		{
			 mnemonicType = inMnemonicType;
		}

		ContextData(std::string inMnemonic)
		: ContextData()
		{
			 mnemonic = inMnemonic;
		}

		ContextData(CryptoType inCryptoType)
		: ContextData()
		{
			 crypto = inCryptoType;
		}

		void setSeed(const uint8_t* inSeed, uint8_t inSeedSize)
		{
			uint8_t fillIndex = inSeedSize < seedSize ? inSeedSize : seedSize;
			for(int i = 0; i < fillIndex; i++)
				seed[i] = inSeed[i];
			for(int i = fillIndex; i < MAXIMUM_SEED_SIZE; i++)
				seed[i] = 0x0;
		}

		std::string getSeedAsHexString()
		{
			char hexString[2*MAXIMUM_SEED_SIZE];
			for(int i = 0; i < seedSize; i++)
				sprintf(hexString+i*2, "%02x", seed[i]);
			return std::string(hexString);
		}

		uint8_t getMnemonicWordCount()
		{
			uint8_t count = 0;
			for (size_t i = 0; i < mnemonic.size(); i++)
			{
				if (mnemonic[i] == ' ')
					count ++;
			}
			if (mnemonic.size() > 0)
				count += 1;
			return count;
		}
	};
}
#endif
