#ifndef CRYPTO_FUNCTIONS_H
#define CRYPTO_FUNCTIONS_H

#include <stdexcept>
#include <string>

#include "core_context.h"
#include "crypto_definitions.h"

extern "C"
{
#include "bip32.h"
}

#define PRIVATE_KEY_BYTE_SIZE 32
#define PUBLIC_KEY_BYTE_SIZE 33

#define STRING(s) #s
#define S(s) STRING(s)
#define BITCOIN_DEFAULT_PATH "m/" S(BITCOIN_PATH_PURPOSE) "'/" S(BITCOIN_PATH_COIN_TYPE) "'/" S(BITCOIN_PATH_ACCOUNT) "'"
#define DOGE_DEFAULT_PATH "m/" S(DOGE_PATH_PURPOSE) "'/" S(DOGE_PATH_COIN_TYPE) "'/" S(DOGE_PATH_ACCOUNT) "'"
#define ETHEREUM_CLASSIC_DEFAULT_PATH "m/" S(ETHEREUM_CLASSIC_PATH_PURPOSE) "'/" S(ETHEREUM_CLASSIC_PATH_COIN_TYPE) "'/" S(ETHEREUM_CLASSIC_PATH_ACCOUNT) "'"
#define ETHEREUM_DEFAULT_PATH "m/" S(ETHEREUM_PATH_PURPOSE) "'/" S(ETHEREUM_PATH_COIN_TYPE) "'/" S(ETHEREUM_PATH_ACCOUNT) "'"
#define NOSTR_DEFAULT_PATH "m/" S(NOSTR_PATH_PURPOSE) "'/" S(NOSTR_PATH_COIN_TYPE) "'/" S(NOSTR_PATH_ACCOUNT) "'/" S(NOSTR_PATH_CHANGE) "/" S(NOSTR_PATH_INDEX)

namespace RetroCrypto
{
	struct AddressInformation
	{
		union
		{
			uint8_t privateKey[PRIVATE_KEY_BYTE_SIZE];
			uint8_t privateSpendKey[MONERO_PRIVATE_SPEND_KEY_LENGTH];
		};
		union
		{
			uint8_t publicKey[PUBLIC_KEY_BYTE_SIZE];
			uint8_t privateViewKey[MONERO_PRIVATE_VIEW_KEY_LENGTH];
		};
		std::string address;

		AddressInformation()
		: privateKey{ 0 }, publicKey{ 0 }, address("")
		{
		}

		AddressInformation(std::string inAddress)
		: AddressInformation()
		{
			address = inAddress;
		}

		~AddressInformation()
		{
			for (int i = 0; i < PRIVATE_KEY_BYTE_SIZE; i++)
				privateKey[i] = 0;
		}

		std::string getKeyAsHexString(const uint8_t* keyArray, uint8_t keySize)
		{
			char hexString[2*PUBLIC_KEY_BYTE_SIZE];
			for(int i = 0; i < keySize; i++)
				sprintf(hexString+i*2, "%02x", keyArray[i]);
			return std::string(hexString);
		}

		std::string getPrivateKeyAsHexString()
		{
			return getKeyAsHexString(privateKey, PRIVATE_KEY_BYTE_SIZE);
		}

		std::string getPublicKeyAsHexString()
		{
			return getKeyAsHexString(publicKey, PUBLIC_KEY_BYTE_SIZE);
		}

		std::string getPrivateSpendKeyAsHexString()
		{
			return getKeyAsHexString(privateSpendKey, MONERO_PRIVATE_SPEND_KEY_LENGTH);
		}

		std::string getPrivateViewKeyAsHexString()
		{
			return getKeyAsHexString(privateViewKey, MONERO_PRIVATE_VIEW_KEY_LENGTH);
		}
	};

	class AddressPath
	{
	private:
		std::string path;
		int headValue;
		std::string::size_type nextSlashPosition;
		bool isValueHardened;
		bool isValidPath;
	public:
		AddressPath()
		{
			path = "";
			headValue = -1;
			nextSlashPosition = std::string::npos;
			isValueHardened = false;
			isValidPath = false;
		}

		AddressPath(std::string inPath)
		: AddressPath()
		{
			path = inPath;
			const int pathStringLength = path.length();
			for (int i = 0; i < pathStringLength; i++)
			{
				if ((path[i] >= 0x30 && path[i] <= 0x39) || (path[i] == 'm' && i == 0))
				{
					continue;
				}
				else if(path[i] == '/' && ((i+1 < pathStringLength && path[i+1] >= 0x30 && path[i+1] <= 0x39) || i+1 == pathStringLength))
				{
					if (nextSlashPosition == std::string::npos)
					{
						nextSlashPosition = i;
					}
					continue;
				}
				else if(path[i] == '\'' && ((i+1 < pathStringLength && path[i+1] == '/') || i+1 == pathStringLength))
				{
						continue;
				}
				nextSlashPosition = std::string::npos;
				return;
			}
			isValidPath = true;
			if (nextSlashPosition == std::string::npos)
				nextSlashPosition = pathStringLength;
			isValueHardened = nextSlashPosition-1 >= 0 && nextSlashPosition-1 < path.length() && path[nextSlashPosition-1] == '\'';
			try
			{
				headValue = std::stoi(path.substr(0, nextSlashPosition-isValueHardened));
				if (headValue < 0)
				{
					headValue = -1;
					isValidPath = false;
				}
			}
			catch(std::exception& e)
			{
				headValue = -1;
				if (inPath[0] != 'm')
					isValidPath = false;
			}
		}

		AddressPath getSubPath()
		{
			return AddressPath(path.substr(nextSlashPosition+1, path.length()-(nextSlashPosition+1)));
		}

		bool hasSubPath()
		{
			return nextSlashPosition != std::string::npos && nextSlashPosition >= 0 && nextSlashPosition+1 < path.length();
		}

		int getHeadValue()
		{
			return headValue;
		}

		bool getIsValueHardened()
		{
			return isValueHardened;
		}

		bool getIsValidPath()
		{
			return isValidPath;
		}
	};
	bool moveToSubNode(HDNode* node, std::string addressPath);
	AddressInformation cryptoAddressFromContextData(const ContextData& data);
	AddressInformation cryptoAddressFromGlobalContext();
	AddressInformation bitcoinAddressFromGlobalContext();
	AddressInformation bitcoinAddressFromSeedBits(const ContextData& data);
	AddressInformation bitcoinAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize, std::string addressPath = BITCOIN_DEFAULT_PATH);
	std::string privateKeyAsBitcoinWIF(const uint8_t privateKey[PRIVATE_KEY_BYTE_SIZE]);
	AddressInformation dogeAddressFromGlobalContext();
	AddressInformation dogeAddressFromSeedBits(const ContextData& data);
	AddressInformation dogeAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize, std::string addressPath = DOGE_DEFAULT_PATH);
	std::string privateKeyAsDogeImportKey(const uint8_t privateKey[PRIVATE_KEY_BYTE_SIZE]);
	AddressInformation ethereumClassicAddressFromGlobalContext();
	AddressInformation ethereumClassicAddressFromSeedBits(const ContextData& data);
	AddressInformation ethereumClassicAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize, std::string addressPath = ETHEREUM_CLASSIC_DEFAULT_PATH);
	AddressInformation ethereumAddressFromGlobalContext();
	AddressInformation ethereumAddressFromSeedBits(const ContextData& data);
	AddressInformation ethereumAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize, std::string addressPath = ETHEREUM_DEFAULT_PATH);
	AddressInformation nostrAddressFromGlobalContext();
	AddressInformation nostrAddressFromSeedBits(const ContextData& data);
	AddressInformation nostrAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize, std::string addressPath = NOSTR_DEFAULT_PATH);
	std::string nsecFromPrivateKey(const uint8_t* privateKey);
	AddressInformation moneroAddressFromGlobalContext();
	AddressInformation moneroAddressFromSeed(const ContextData& data);
	AddressInformation moneroAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string mnemonicFromGlobalContext();
	std::string bip39MnemonicFromGlobalContext();
	std::string bip39MnemonicFromSeedBits(const ContextData& data);
	std::string bip39MnemonicFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string legacyMoneroEnglishMnemonicFromGlobalContext();
	std::string legacyMoneroEnglishMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroEnglishMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroGermanMnemonicFromGlobalContext();
	std::string legacyMoneroGermanMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroGermanMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroSpanishMnemonicFromGlobalContext();
	std::string legacyMoneroSpanishMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroSpanishMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroFrenchMnemonicFromGlobalContext();
	std::string legacyMoneroFrenchMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroFrenchMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroItalianMnemonicFromGlobalContext();
	std::string legacyMoneroItalianMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroItalianMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroDutchMnemonicFromGlobalContext();
	std::string legacyMoneroDutchMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroDutchMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroPortugueseMnemonicFromGlobalContext();
	std::string legacyMoneroPortugueseMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroPortugueseMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroRussianMnemonicFromGlobalContext();
	std::string legacyMoneroRussianMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroRussianMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroJapaneseMnemonicFromGlobalContext();
	std::string legacyMoneroJapaneseMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroJapaneseMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroChineseMnemonicFromGlobalContext();
	std::string legacyMoneroChineseMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroChineseMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroEsperantoMnemonicFromGlobalContext();
	std::string legacyMoneroEsperantoMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroEsperantoMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroLojbanMnemonicFromGlobalContext();
	std::string legacyMoneroLojbanMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroLojbanMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
}

#endif
