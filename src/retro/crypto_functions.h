#include <string>

#include "core_context.h"

namespace RetroCrypto
{
	std::string cryptoAddressFromGlobalContext();
	std::string moneroAddressFromGlobalContext();
	std::string moneroAddressFromSeed(const ContextData& data);
	std::string moneroAddressFromSeed(const uint8_t* seed, const uint8_t seedSize);
}
