#include <string>

#include "core_context.h"

#define XPRIV_BYTE_SIZE 32
#define XPUB_BYTE_SIZE 33

namespace RetroCrypto
{
	struct AddressInformation
	{
		uint8_t xpriv[XPRIV_BYTE_SIZE];
		uint8_t xpub[XPUB_BYTE_SIZE];
		std::string address;

		AddressInformation()
		: xpriv{ 0 }, xpub{ 0 }, address("")
		{
		}

		AddressInformation(std::string inAddress)
		: AddressInformation()
		{
			address = inAddress;
		}

		~AddressInformation()
		{
			for (int i = 0; i < XPRIV_BYTE_SIZE; i++)
				xpriv[i] = 0;
		}

		std::string getKeyAsHexString(const uint8_t* keyArray, uint8_t keySize)
		{
			char hexString[2*XPUB_BYTE_SIZE];
			for(int i = 0; i < keySize; i++)
				sprintf(hexString+i*2, "%02x", keyArray[i]);
			return std::string(hexString);
		}

		std::string getXprivAsHexString()
		{
			return getKeyAsHexString(xpriv, XPRIV_BYTE_SIZE);
		}

		std::string getXpubAsHexString()
		{
			return getKeyAsHexString(xpub, XPUB_BYTE_SIZE);
		}
	};

	AddressInformation cryptoAddressFromContextData(const ContextData& data);
	AddressInformation cryptoAddressFromGlobalContext();
	AddressInformation bitcoinAddressFromGlobalContext();
	AddressInformation bitcoinAddressFromSeedBits(const ContextData& data);
	AddressInformation bitcoinAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	AddressInformation dogeAddressFromGlobalContext();
	AddressInformation dogeAddressFromSeedBits(const ContextData& data);
	AddressInformation dogeAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	AddressInformation ethereumClassicAddressFromGlobalContext();
	AddressInformation ethereumClassicAddressFromSeedBits(const ContextData& data);
	AddressInformation ethereumClassicAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	AddressInformation ethereumAddressFromGlobalContext();
	AddressInformation ethereumAddressFromSeedBits(const ContextData& data);
	AddressInformation ethereumAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	AddressInformation nostrAddressFromGlobalContext();
	AddressInformation nostrAddressFromSeedBits(const ContextData& data);
	AddressInformation nostrAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	AddressInformation moneroAddressFromGlobalContext();
	AddressInformation moneroAddressFromSeed(const ContextData& data);
	AddressInformation moneroAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string mnemonicFromGlobalContext();
	std::string bip39MnemonicFromGlobalContext();
	std::string bip39MnemonicFromSeedBits(const ContextData& data);
	std::string bip39MnemonicFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string legacyMoneroMnemonicFromGlobalContext();
	std::string legacyMoneroMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
}
