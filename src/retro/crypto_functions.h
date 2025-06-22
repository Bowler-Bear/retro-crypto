#include <string>

#include "core_context.h"

namespace RetroCrypto
{
	std::string cryptoAddressFromGlobalContext();
	std::string bitcoinAddressFromGlobalContext();
	std::string bitcoinAddressFromSeed(const ContextData& data);
	std::string bitcoinAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string dogeAddressFromGlobalContext();
	std::string dogeAddressFromSeed(const ContextData& data);
	std::string dogeAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string nostrAddressFromGlobalContext();
	std::string nostrAddressFromSeed(const ContextData& data);
	std::string nostrAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
	std::string moneroAddressFromGlobalContext();
	std::string moneroAddressFromSeed(const ContextData& data);
	std::string moneroAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
}
