#include <string>

#include "core_context.h"

namespace RetroCrypto
{
	std::string cryptoAddressFromContextData(const ContextData& data);
	std::string cryptoAddressFromGlobalContext();
	std::string bitcoinAddressFromGlobalContext();
	std::string bitcoinAddressFromSeed(const ContextData& data);
	std::string bitcoinAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string dogeAddressFromGlobalContext();
	std::string dogeAddressFromSeed(const ContextData& data);
	std::string dogeAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string ethereumClassicAddressFromGlobalContext();
	std::string ethereumClassicAddressFromSeed(const ContextData& data);
	std::string ethereumClassicAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string ethereumAddressFromGlobalContext();
	std::string ethereumAddressFromSeed(const ContextData& data);
	std::string ethereumAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string nostrAddressFromGlobalContext();
	std::string nostrAddressFromSeed(const ContextData& data);
	std::string nostrAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string moneroAddressFromGlobalContext();
	std::string moneroAddressFromSeed(const ContextData& data);
	std::string moneroAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string mnemonicFromGlobalContext();
	std::string bip39MnemonicFromGlobalContext();
	std::string bip39MnemonicFromSeed(const ContextData& data);
	std::string bip39MnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string legacyMoneroMnemonicFromGlobalContext();
	std::string legacyMoneroMnemonicFromSeed(const ContextData& data);
	std::string legacyMoneroMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize);
}
