#include "crypto_functions.h"
#include "core_system.h"

namespace RetroCrypto
{
	std::string cryptoAddressFromGlobalContext()
	{
		const ContextData& data(CoreSystem::getCoreSystem().getContextData());
		switch(data.crypto)
		{
		default:
			return std::string("Address generation for this crypto is currently not supported.");
		}
	}
}
