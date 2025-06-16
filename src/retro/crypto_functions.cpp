#include "crypto_functions.h"
#include "core_system.h"

extern "C"
{
#include "monero/monero.h"
#include "bip32.h"
}

#define BITCOIN_ELLIPTIC_CURVE "secp256k1"
#define BITCOIN_MAXIMUM_ADDRESS_LENGTH ADDRESS_MAXLEN
#define BITCOIN_ADDRESS_VERSION_BYTE 0
#define BITCOIN_HD_MASTER_SEED_SIZE 64

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
		case RetroCrypto::CryptoType::BTC:
			return bitcoinAddressFromGlobalContext();
		case RetroCrypto::CryptoType::XMR:
			return moneroAddressFromGlobalContext();
		default:
			return std::string("Address generation for this crypto is currently not supported.");
		}
	}

	std::string bitcoinAddressFromGlobalContext()
	{
		return bitcoinAddressFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string bitcoinAddressFromSeed(const ContextData& data)
	{
		return bitcoinAddressFromSeed(data.seed, data.seedSize);
	}

	std::string bitcoinAddressFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		uint8_t masterNodeSeed[BITCOIN_HD_MASTER_SEED_SIZE] = { 0 };
		mnemonic_to_seed(mnemonic_from_data(seed, seedSize), "", masterNodeSeed, nullptr);
		mnemonic_clear();
		HDNode node;
		if (hdnode_from_seed(masterNodeSeed, BITCOIN_HD_MASTER_SEED_SIZE, BITCOIN_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		char btcAddress[BITCOIN_MAXIMUM_ADDRESS_LENGTH];
		hdnode_private_ckd_prime(&node, 0);
		hdnode_private_ckd_prime(&node, 0);
		hdnode_private_ckd_prime(&node, 0);
		if (hdnode_get_address(&node, BITCOIN_ADDRESS_VERSION_BYTE, (char*)&btcAddress, BITCOIN_MAXIMUM_ADDRESS_LENGTH) != 0)
			return string("Failed to generate address from HD node");
		return btcAddress;
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
