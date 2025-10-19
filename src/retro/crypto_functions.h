#include <string>

#include "core_context.h"
#include "crypto_definitions.h"

#define PRIVATE_KEY_BYTE_SIZE 32
#define PUBLIC_KEY_BYTE_SIZE 33

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

	AddressInformation cryptoAddressFromContextData(const ContextData& data);
	AddressInformation cryptoAddressFromGlobalContext();
	AddressInformation bitcoinAddressFromGlobalContext();
	AddressInformation bitcoinAddressFromSeedBits(const ContextData& data);
	AddressInformation bitcoinAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string privateKeyAsBitcoinWIF(const uint8_t privateKey[PRIVATE_KEY_BYTE_SIZE]);
	AddressInformation dogeAddressFromGlobalContext();
	AddressInformation dogeAddressFromSeedBits(const ContextData& data);
	AddressInformation dogeAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string privateKeyAsDogeImportKey(const uint8_t privateKey[PRIVATE_KEY_BYTE_SIZE]);
	AddressInformation ethereumClassicAddressFromGlobalContext();
	AddressInformation ethereumClassicAddressFromSeedBits(const ContextData& data);
	AddressInformation ethereumClassicAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	AddressInformation ethereumAddressFromGlobalContext();
	AddressInformation ethereumAddressFromSeedBits(const ContextData& data);
	AddressInformation ethereumAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	AddressInformation nostrAddressFromGlobalContext();
	AddressInformation nostrAddressFromSeedBits(const ContextData& data);
	AddressInformation nostrAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
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
