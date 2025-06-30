#include "crypto_functions.h"
#include "core_system.h"
#include "crypto_definitions.h"

extern "C"
{
#include "monero/monero.h"
#include "bip32.h"
#include "bip39.h"
#include "segwit_addr.h"
#include "legacy_monero_mnemonic.h"
}

namespace RetroCrypto
{
	std::string cryptoAddressFromGlobalContext()
	{
		const ContextData& data(CoreSystem::getCoreSystem().getContextData());
		switch(data.crypto)
		{
		case RetroCrypto::CryptoType::BTC:
			return bitcoinAddressFromGlobalContext();
		case RetroCrypto::CryptoType::DOGE:
			return dogeAddressFromGlobalContext();
		case RetroCrypto::CryptoType::ETC:
			return ethereumClassicAddressFromGlobalContext();
		case RetroCrypto::CryptoType::ETH:
			return ethereumAddressFromGlobalContext();
		case RetroCrypto::CryptoType::NOSTR:
			return nostrAddressFromGlobalContext();
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
		hdnode_private_ckd_prime(&node, BITCOIN_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, BITCOIN_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, BITCOIN_PATH_ACCOUNT);
		if (hdnode_get_address(&node, BITCOIN_ADDRESS_VERSION_BYTE, (char*)&btcAddress, BITCOIN_MAXIMUM_ADDRESS_LENGTH) != 0)
			return string("Failed to generate address from HD node");
		return btcAddress;
	}

	std::string dogeAddressFromGlobalContext()
	{
		return dogeAddressFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string dogeAddressFromSeed(const ContextData& data)
	{
		return dogeAddressFromSeed(data.seed, data.seedSize);
	}

	std::string dogeAddressFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		HDNode node;
		if (hdnode_from_seed(seed, seedSize, BITCOIN_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		hdnode_private_ckd_prime(&node, DOGE_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, DOGE_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, DOGE_PATH_ACCOUNT);
		hdnode_fill_public_key(&node);
		char dogeAddress[BITCOIN_MAXIMUM_ADDRESS_LENGTH];
		if (hdnode_get_address(&node, DOGE_ADDRESS_VERSION_BYTE, (char*)&dogeAddress, BITCOIN_MAXIMUM_ADDRESS_LENGTH) != 0)
			return string("Failed to generate address from HD node");
		return dogeAddress;
	}

	std::string ethereumClassicAddressFromGlobalContext()
	{
		return ethereumClassicAddressFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string ethereumClassicAddressFromSeed(const ContextData& data)
	{
		return ethereumClassicAddressFromSeed(data.seed, data.seedSize);
	}

	std::string ethereumClassicAddressFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		uint8_t masterNodeSeed[BITCOIN_HD_MASTER_SEED_SIZE] = { 0 };
		mnemonic_to_seed(mnemonic_from_data(seed, seedSize), "", masterNodeSeed, nullptr);
		mnemonic_clear();

		HDNode node;
		if (hdnode_from_seed(masterNodeSeed, BITCOIN_HD_MASTER_SEED_SIZE, ETHEREUM_CLASSIC_HD_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		hdnode_private_ckd_prime(&node, ETHEREUM_CLASSIC_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, ETHEREUM_CLASSIC_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, ETHEREUM_CLASSIC_PATH_ACCOUNT);
		hdnode_fill_public_key(&node);
		uint8_t ethAddressBytes[ETHEREUM_CLASSIC_ADDRESS_BYTES] = { 0 };
		if (hdnode_get_ethereum_pubkeyhash(&node, ethAddressBytes) != 1)
			return string("Failed to generate address from HD node");

		uint8_t publicKeyHash[sha3_256_hash_size] = { 0 };
		SHA3_CTX ctx = {0};
		sha3_256_Init(&ctx);
		for (int i = 0; i < ETHEREUM_CLASSIC_ADDRESS_BYTES; i++)
		{
			char hexString[3] = { 0 };
			std::sprintf(hexString, "%02x", ethAddressBytes[i]);
			sha3_Update(&ctx, (const unsigned char*)hexString, 2);
		}
		keccak_Final(&ctx, publicKeyHash);

		char ethAddress[ETHEREUM_CLASSIC_ADDRESS_LENGTH] = "0x";
		for (int i = 0; i < ETHEREUM_CLASSIC_ADDRESS_BYTES; i++)
		{
			char hexValue[3] = { 0 };
			std::sprintf(hexValue, "%02x", ethAddressBytes[i]);
			for(int j = 0; j < 2; j++)
			{
				if (!((hexValue[j] >= 'A' && hexValue[j] <= 'Z') || (hexValue[j] >= 'a' && hexValue[j] <= 'z')))
				{
					ethAddress[2+2*i+j] = hexValue[j];
					continue;
				}
				if (((publicKeyHash[i] >> 4*(1-j)) & 0xF) >= 8)
					ethAddress[2+2*i+j] = hexValue[j] - 0x20;
				else
					ethAddress[2+2*i+j] = hexValue[j];
			}
		}
		return ethAddress;
	}

	std::string ethereumAddressFromGlobalContext()
	{
		return ethereumAddressFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string ethereumAddressFromSeed(const ContextData& data)
	{
		return ethereumAddressFromSeed(data.seed, data.seedSize);
	}

	std::string ethereumAddressFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		uint8_t masterNodeSeed[BITCOIN_HD_MASTER_SEED_SIZE] = { 0 };
		mnemonic_to_seed(mnemonic_from_data(seed, seedSize), "", masterNodeSeed, nullptr);
		mnemonic_clear();

		HDNode node;
		if (hdnode_from_seed(masterNodeSeed, BITCOIN_HD_MASTER_SEED_SIZE, ETHEREUM_HD_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		hdnode_private_ckd_prime(&node, ETHEREUM_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, ETHEREUM_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, ETHEREUM_PATH_ACCOUNT);
		hdnode_fill_public_key(&node);
		uint8_t ethAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0 };
		if (hdnode_get_ethereum_pubkeyhash(&node, ethAddressBytes) != 1)
			return string("Failed to generate address from HD node");

		uint8_t publicKeyHash[sha3_256_hash_size] = { 0 };
		SHA3_CTX ctx = {0};
		sha3_256_Init(&ctx);
		for (int i = 0; i < ETHEREUM_ADDRESS_BYTES; i++)
		{
			char hexString[3] = { 0 };
			std::sprintf(hexString, "%02x", ethAddressBytes[i]);
			sha3_Update(&ctx, (const unsigned char*)hexString, 2);
		}
		keccak_Final(&ctx, publicKeyHash);

		char ethAddress[ETHEREUM_ADDRESS_LENGTH] = "0x";
		for (int i = 0; i < ETHEREUM_ADDRESS_BYTES; i++)
		{
			char hexValue[3] = { 0 };
			std::sprintf(hexValue, "%02x", ethAddressBytes[i]);
			for(int j = 0; j < 2; j++)
			{
				if (!((hexValue[j] >= 'A' && hexValue[j] <= 'Z') || (hexValue[j] >= 'a' && hexValue[j] <= 'z')))
				{
					ethAddress[2+2*i+j] = hexValue[j];
					continue;
				}
				if (((publicKeyHash[i] >> 4*(1-j)) & 0xF) >= 8)
					ethAddress[2+2*i+j] = hexValue[j] - 0x20;
				else
					ethAddress[2+2*i+j] = hexValue[j];
			}
		}
		return ethAddress;
	}

	std::string nostrAddressFromGlobalContext()
	{
		return nostrAddressFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string nostrAddressFromSeed(const ContextData& data)
	{
		return nostrAddressFromSeed(data.seed, data.seedSize);
	}

	std::string nostrAddressFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		uint8_t masterNodeSeed[BITCOIN_HD_MASTER_SEED_SIZE] = { 0 };
		mnemonic_to_seed(mnemonic_from_data(seed, seedSize), "", masterNodeSeed, nullptr);
		mnemonic_clear();

		HDNode node;
		if (hdnode_from_seed(masterNodeSeed, BITCOIN_HD_MASTER_SEED_SIZE, BITCOIN_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		hdnode_private_ckd_prime(&node, NOSTR_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, NOSTR_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, NOSTR_PATH_ACCOUNT);
		hdnode_private_ckd(&node, NOSTR_PATH_CHANGE);
		hdnode_private_ckd(&node, NOSTR_PATH_INDEX);
		hdnode_fill_public_key(&node);
		uint8_t data[NOSTR_PUBLIC_KEY_SIZE*8/5+1] = {0};

		size_t dataLength = 0;
		if (convert_bits_wrapper(data, &dataLength, 5, (const uint8_t*)(node.public_key+1), NOSTR_PUBLIC_KEY_SIZE, 8, 1) != 1)
			return std::string("Failed to convert public key from 8 bit array to 5 bit array.");
		const char nostrHrp[NOSTR_PUBLIC_ADDRESS_HRP_SIZE] =  "npub\0";
		char nostrAddress[NOSTR_PUBLIC_ADDRESS_HRP_SIZE+NOSTR_PUBLIC_KEY_SIZE*8/5+1+8] = { 0 };
		if (bech32_encode(nostrAddress, nostrHrp, data, dataLength, BECH32_ENCODING_BECH32) != 1)
			return string("Error encoding nostr public key.");
		return string(nostrAddress);
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

	std::string mnemonicFromGlobalContext()
	{
		const ContextData& data(CoreSystem::getCoreSystem().getContextData());
		switch(data.mnemonicType)
		{
		case RetroCrypto::MnemonicType::BIP39:
			return bip39MnemonicFromGlobalContext();
		case RetroCrypto::MnemonicType::LEGACY_MONERO:
			return legacyMoneroMnemonicFromGlobalContext();
		default:
			return std::string("This seed phrase is currently not supported.");
		}
	}

	std::string bip39MnemonicFromGlobalContext()
	{
		return bip39MnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string bip39MnemonicFromSeed(const ContextData& data)
	{
		return bip39MnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string bip39MnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		std::string mnemonic(mnemonic_from_data(seed, seedSize));
		mnemonic_clear();
		return mnemonic;
	}

	std::string legacyMoneroMnemonicFromGlobalContext()
	{
		return legacyMoneroMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		std::string mnemonic(legacy_monero_mnemonic_from_seed(seed, seedSize, MoneroEnglish));
		clear_legacy_monero_mnemonic();
		return mnemonic;
	}
}
