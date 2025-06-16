#include "crypto_functions.h"
#include "core_system.h"

extern "C"
{
#include "monero/monero.h"
}

#define MONERO_PRIVATE_SPEND_KEY_LENGTH 32
#define MONERO_PUBLIC_SPEND_KEY_LENGTH 32
#define MONERO_PUBLIC_VIEW_KEY_LENGTH 32
#define MONERO_PUBLIC_KEYS_LENGTH MONERO_PUBLIC_SPEND_KEY_LENGTH+MONERO_PUBLIC_VIEW_KEY_LENGTH
#define MONERO_MAXIMUM_ADDRESS_LENGTH 128
#define MONERO_MAINNET_MAGIC_BYTE 18

namespace RetroCrypto
{
	std::string cryptoAddressFromGlobalContext()
	{
		const ContextData& data(CoreSystem::getCoreSystem().getContextData());
		switch(data.crypto)
		{
		case RetroCrypto::CryptoType::XMR:
			return moneroAddressFromGlobalContext();
			break;
		default:
			return std::string("Address generation for this crypto is currently not supported.");
		}
	}

	std::string moneroAddressFromGlobalContext()
	{
		return moneroAddressFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string moneroAddressFromSeed(const ContextData& data)
	{
		return moneroAddressFromSeed(data.seed, data.seedSize);
	}

	std::string moneroAddressFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		if (seedSize != MONERO_PRIVATE_SPEND_KEY_LENGTH)
			return std::string("Invalid Seed Size.");
		ContextData data = CoreSystem::getCoreSystem().getContextData();
		bignum256modm seedAsBigNum;
		expand256_modm(seedAsBigNum, (const unsigned char*)&data.seed, MONERO_PRIVATE_SPEND_KEY_LENGTH);

		ge25519 publicSpendKey;
		ge25519_scalarmult_base_wrapper(&publicSpendKey, seedAsBigNum);

		bignum256modm viewSecretAsBigNum;
		xmr_hash_to_scalar(viewSecretAsBigNum, (const unsigned char*)&data.seed, MONERO_PRIVATE_SPEND_KEY_LENGTH);

		ge25519 publicViewKey;
		ge25519_scalarmult_base_wrapper(&publicViewKey, viewSecretAsBigNum);

		uint8_t publicBytes[MONERO_PUBLIC_KEYS_LENGTH];
		ge25519_pack(publicBytes, &publicSpendKey);
		ge25519_pack(publicBytes + MONERO_PUBLIC_SPEND_KEY_LENGTH, &publicViewKey);

		char xmrAddress[MONERO_MAXIMUM_ADDRESS_LENGTH];
		xmr_base58_addr_encode_check(MONERO_MAINNET_MAGIC_BYTE, publicBytes, MONERO_PUBLIC_KEYS_LENGTH, (char*)&xmrAddress, MONERO_MAXIMUM_ADDRESS_LENGTH);
		return std::string(xmrAddress);
	}
}
