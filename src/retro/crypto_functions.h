#include <string>

#include "core_context.h"

namespace RetroCrypto
{
	std::string cryptoAddressFromContextData(const ContextData& data);
	std::string cryptoAddressFromGlobalContext();
	std::string bitcoinAddressFromGlobalContext();
	std::string bitcoinAddressFromSeedBits(const ContextData& data);
	std::string bitcoinAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string dogeAddressFromGlobalContext();
	std::string dogeAddressFromSeedBits(const ContextData& data);
	std::string dogeAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string ethereumClassicAddressFromGlobalContext();
	std::string ethereumClassicAddressFromSeedBits(const ContextData& data);
	std::string ethereumClassicAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string ethereumAddressFromGlobalContext();
	std::string ethereumAddressFromSeedBits(const ContextData& data);
	std::string ethereumAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string nostrAddressFromGlobalContext();
	std::string nostrAddressFromSeedBits(const ContextData& data);
	std::string nostrAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string moneroAddressFromGlobalContext();
	std::string moneroAddressFromSeed(const ContextData& data);
	std::string moneroAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string mnemonicFromGlobalContext();
	std::string bip39MnemonicFromGlobalContext();
	std::string bip39MnemonicFromSeedBits(const ContextData& data);
	std::string bip39MnemonicFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize);
	std::string legacyMoneroMnemonicFromGlobalContext();
	std::string legacyMoneroMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
}
