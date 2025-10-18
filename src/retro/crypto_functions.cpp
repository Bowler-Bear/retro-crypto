#include "crypto_functions.h"
#include "core_system.h"
#include "crypto_definitions.h"

extern "C"
{
#include "monero/monero.h"
#include "base58.h"
#include "bip32.h"
#include "bip39.h"
#include "segwit_addr.h"
#include "legacy_monero_mnemonic.h"
}

namespace RetroCrypto
{
	AddressInformation cryptoAddressFromContextData(const ContextData& data)
	{
		switch(data.crypto)
		{
		case RetroCrypto::CryptoType::BTC:
			return bitcoinAddressFromSeedBits(data);
		case RetroCrypto::CryptoType::DOGE:
			return dogeAddressFromSeedBits(data);
		case RetroCrypto::CryptoType::ETC:
			return ethereumClassicAddressFromSeedBits(data);
		case RetroCrypto::CryptoType::ETH:
			return ethereumAddressFromSeedBits(data);
		case RetroCrypto::CryptoType::NOSTR:
			return nostrAddressFromSeedBits(data);
		case RetroCrypto::CryptoType::XMR:
			return moneroAddressFromSeed(data);
		default:
			return std::string("Address generation for this crypto is currently not supported.");
		}
	}

	AddressInformation cryptoAddressFromGlobalContext()
	{
		return cryptoAddressFromContextData(CoreSystem::getCoreSystem().getContextData());
	}

	AddressInformation bitcoinAddressFromGlobalContext()
	{
		return bitcoinAddressFromSeedBits(CoreSystem::getCoreSystem().getContextData());
	}

	AddressInformation bitcoinAddressFromSeedBits(const ContextData& data)
	{
		return bitcoinAddressFromSeedBits(data.seed, data.seedSize);
	}

	AddressInformation bitcoinAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize)
	{
		if (seedSize != 16 && seedSize != 24 && seedSize != 32)
			return string("Invalid seed bits size for this address.");
		uint8_t masterNodeSeed[BITCOIN_HD_MASTER_SEED_SIZE] = { 0 };
		mnemonic_to_seed(mnemonic_from_data(seedBits, seedSize), "", masterNodeSeed, nullptr);
		mnemonic_clear();
		HDNode node;
		if (hdnode_from_seed(masterNodeSeed, BITCOIN_HD_MASTER_SEED_SIZE, BITCOIN_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		char btcAddress[BITCOIN_MAXIMUM_ADDRESS_LENGTH];
		hdnode_private_ckd_prime(&node, BITCOIN_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, BITCOIN_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, BITCOIN_PATH_ACCOUNT);

		AddressInformation AddressInformationrmation;
		memcpy(AddressInformationrmation.privateKey, node.private_key, PRIVATE_KEY_BYTE_SIZE);
		hdnode_fill_public_key(&node);
		memcpy(AddressInformationrmation.publicKey, node.public_key, PUBLIC_KEY_BYTE_SIZE);

		if (hdnode_get_address(&node, BITCOIN_ADDRESS_VERSION_BYTE, (char*)&btcAddress, BITCOIN_MAXIMUM_ADDRESS_LENGTH) != 0)
			return string("Failed to generate address from HD node");
		AddressInformationrmation.address = btcAddress;
		return AddressInformationrmation;
	}

	std::string privateKeyAsBitcoinWIF(const uint8_t* privateKey)
	{
		char wif[MAX_WIF_SIZE];
		ecdsa_get_wif(privateKey, BITCOIN_WIF_VERSION_BYTE, HASHER_SHA2D, wif, MAX_WIF_SIZE);
               return wif;
	}

	AddressInformation dogeAddressFromGlobalContext()
	{
		return dogeAddressFromSeedBits(CoreSystem::getCoreSystem().getContextData());
	}

	AddressInformation dogeAddressFromSeedBits(const ContextData& data)
	{
		return dogeAddressFromSeedBits(data.seed, data.seedSize);
	}

	AddressInformation dogeAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize)
	{
		HDNode node;
		if (hdnode_from_seed(seedBits, seedSize, BITCOIN_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		hdnode_private_ckd_prime(&node, DOGE_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, DOGE_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, DOGE_PATH_ACCOUNT);

		AddressInformation AddressInformationrmation;
		memcpy(AddressInformationrmation.privateKey, node.private_key, PRIVATE_KEY_BYTE_SIZE);
		hdnode_fill_public_key(&node);
		memcpy(AddressInformationrmation.publicKey, node.public_key, PUBLIC_KEY_BYTE_SIZE);

		char dogeAddress[BITCOIN_MAXIMUM_ADDRESS_LENGTH];
		if (hdnode_get_address(&node, DOGE_ADDRESS_VERSION_BYTE, (char*)&dogeAddress, BITCOIN_MAXIMUM_ADDRESS_LENGTH) != 0)
			return string("Failed to generate address from HD node");
		AddressInformationrmation.address = dogeAddress;
		return AddressInformationrmation;
	}

	std::string privateKeyAsDogeImportKey(const uint8_t* privateKey)
	{
		char wif[MAX_WIF_SIZE];
		ecdsa_get_wif(privateKey, DOGE_SECRET_KEY_VERSION_BYTE, HASHER_SHA2D, wif, MAX_WIF_SIZE);
               return wif;
	}

	AddressInformation ethereumClassicAddressFromGlobalContext()
	{
		return ethereumClassicAddressFromSeedBits(CoreSystem::getCoreSystem().getContextData());
	}

	AddressInformation ethereumClassicAddressFromSeedBits(const ContextData& data)
	{
		return ethereumClassicAddressFromSeedBits(data.seed, data.seedSize);
	}

	AddressInformation ethereumClassicAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize)
	{
		if (seedSize != 16 && seedSize != 24 && seedSize != 32)
			return string("Invalid seed bits size for this address.");
		uint8_t masterNodeSeed[BITCOIN_HD_MASTER_SEED_SIZE] = { 0 };
		mnemonic_to_seed(mnemonic_from_data(seedBits, seedSize), "", masterNodeSeed, nullptr);
		mnemonic_clear();

		HDNode node;
		if (hdnode_from_seed(masterNodeSeed, BITCOIN_HD_MASTER_SEED_SIZE, ETHEREUM_CLASSIC_HD_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		hdnode_private_ckd_prime(&node, ETHEREUM_CLASSIC_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, ETHEREUM_CLASSIC_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, ETHEREUM_CLASSIC_PATH_ACCOUNT);

		AddressInformation AddressInformationrmation;
		memcpy(AddressInformationrmation.privateKey, node.private_key, PRIVATE_KEY_BYTE_SIZE);
		hdnode_fill_public_key(&node);
		memcpy(AddressInformationrmation.publicKey, node.public_key, PUBLIC_KEY_BYTE_SIZE);

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
		AddressInformationrmation.address = ethAddress;
		return AddressInformationrmation;
	}

	AddressInformation ethereumAddressFromGlobalContext()
	{
		return ethereumAddressFromSeedBits(CoreSystem::getCoreSystem().getContextData());
	}

	AddressInformation ethereumAddressFromSeedBits(const ContextData& data)
	{
		return ethereumAddressFromSeedBits(data.seed, data.seedSize);
	}

	AddressInformation ethereumAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize)
	{
		if (seedSize != 16 && seedSize != 24 && seedSize != 32)
			return string("Invalid seed bits size for this address.");
		uint8_t masterNodeSeed[BITCOIN_HD_MASTER_SEED_SIZE] = { 0 };
		mnemonic_to_seed(mnemonic_from_data(seedBits, seedSize), "", masterNodeSeed, nullptr);
		mnemonic_clear();

		HDNode node;
		if (hdnode_from_seed(masterNodeSeed, BITCOIN_HD_MASTER_SEED_SIZE, ETHEREUM_HD_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		hdnode_private_ckd_prime(&node, ETHEREUM_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, ETHEREUM_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, ETHEREUM_PATH_ACCOUNT);

		AddressInformation AddressInformationrmation;
		memcpy(AddressInformationrmation.privateKey, node.private_key, PRIVATE_KEY_BYTE_SIZE);
		hdnode_fill_public_key(&node);
		memcpy(AddressInformationrmation.publicKey, node.public_key, PUBLIC_KEY_BYTE_SIZE);

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
		AddressInformationrmation.address = ethAddress;
		return AddressInformationrmation;
	}

	AddressInformation nostrAddressFromGlobalContext()
	{
		return nostrAddressFromSeedBits(CoreSystem::getCoreSystem().getContextData());
	}

	AddressInformation nostrAddressFromSeedBits(const ContextData& data)
	{
		return nostrAddressFromSeedBits(data.seed, data.seedSize);
	}

	AddressInformation nostrAddressFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize)
	{
		if (seedSize != 16 && seedSize != 24 && seedSize != 32)
			return string("Invalid seed bits size for this address.");
		uint8_t masterNodeSeed[BITCOIN_HD_MASTER_SEED_SIZE] = { 0 };
		mnemonic_to_seed(mnemonic_from_data(seedBits, seedSize), "", masterNodeSeed, nullptr);
		mnemonic_clear();

		HDNode node;
		if (hdnode_from_seed(masterNodeSeed, BITCOIN_HD_MASTER_SEED_SIZE, BITCOIN_ELLIPTIC_CURVE, &node) != 1)
			return string("Error generating master HD node.");
		hdnode_private_ckd_prime(&node, NOSTR_PATH_PURPOSE);
		hdnode_private_ckd_prime(&node, NOSTR_PATH_COIN_TYPE);
		hdnode_private_ckd_prime(&node, NOSTR_PATH_ACCOUNT);
		hdnode_private_ckd(&node, NOSTR_PATH_CHANGE);
		hdnode_private_ckd(&node, NOSTR_PATH_INDEX);

		AddressInformation AddressInformationrmation;
		memcpy(AddressInformationrmation.privateKey, node.private_key, PRIVATE_KEY_BYTE_SIZE);
		hdnode_fill_public_key(&node);
		memcpy(AddressInformationrmation.publicKey, node.public_key, PUBLIC_KEY_BYTE_SIZE);

		uint8_t data[NOSTR_PUBLIC_KEY_SIZE*8/5+1] = {0};

		size_t dataLength = 0;
		if (convert_bits_wrapper(data, &dataLength, 5, (const uint8_t*)(node.public_key+1), NOSTR_PUBLIC_KEY_SIZE, 8, 1) != 1)
			return std::string("Failed to convert public key from 8 bit array to 5 bit array.");
		const char nostrHrp[NOSTR_PUBLIC_ADDRESS_HRP_SIZE] =  "npub\0";
		char nostrAddress[NOSTR_PUBLIC_ADDRESS_HRP_SIZE+NOSTR_PUBLIC_KEY_SIZE*8/5+1+8] = { 0 };
		if (bech32_encode(nostrAddress, nostrHrp, data, dataLength, BECH32_ENCODING_BECH32) != 1)
			return string("Error encoding nostr public key.");
		AddressInformationrmation.address = nostrAddress;
		return AddressInformationrmation;
	}

	std::string nsecFromPrivateKey(const uint8_t* privateKey)
	{
		uint8_t data[NOSTR_PUBLIC_KEY_SIZE*8/5+1] = {0};

		size_t dataLength = 0;
		if (convert_bits_wrapper(data, &dataLength, 5, privateKey, NOSTR_PRIVATE_KEY_SIZE, 8, 1) != 1)
			return std::string("Failed to convert private key from 8 bit array to 5 bit array.");
		const char nsecHrp[NOSTR_SECRET_ADDRESS_HRP_SIZE] =  "nsec\0";
		char nsec[NOSTR_SECRET_ADDRESS_HRP_SIZE+NOSTR_PRIVATE_KEY_SIZE*8/5+1+8] = { 0 };
		if (bech32_encode(nsec, nsecHrp, data, dataLength, BECH32_ENCODING_BECH32) != 1)
			return string("Error encoding nostr secret key.");
		return nsec;
	}

	AddressInformation moneroAddressFromGlobalContext()
	{
		return moneroAddressFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	AddressInformation moneroAddressFromSeed(const ContextData& data)
	{
		return moneroAddressFromSeed(data.seed, data.seedSize);
	}

	AddressInformation moneroAddressFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		if (seedSize != MONERO_PRIVATE_SPEND_KEY_LENGTH)
			return std::string("Invalid Seed Size.");
		bignum256modm seedAsBigNum;
		expand256_modm(seedAsBigNum, seed, MONERO_PRIVATE_SPEND_KEY_LENGTH);

		ge25519 publicSpendKey;
		ge25519_scalarmult_base_wrapper(&publicSpendKey, seedAsBigNum);

		bignum256modm viewSecretAsBigNum;
		xmr_hash_to_scalar(viewSecretAsBigNum, seed, MONERO_PRIVATE_SPEND_KEY_LENGTH);

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
		case RetroCrypto::MnemonicType::LEGACY_MONERO_ENGLISH:
			return legacyMoneroEnglishMnemonicFromGlobalContext();
		default:
			return std::string("This seed phrase is currently not supported.");
		}
	}

	std::string bip39MnemonicFromGlobalContext()
	{
		return bip39MnemonicFromSeedBits(CoreSystem::getCoreSystem().getContextData());
	}

	std::string bip39MnemonicFromSeedBits(const ContextData& data)
	{
		return bip39MnemonicFromSeedBits(data.seed, data.seedSize);
	}

	std::string bip39MnemonicFromSeedBits(const uint8_t* seedBits, const uint8_t seedSize)
	{
		std::string mnemonic(mnemonic_from_data(seedBits, seedSize));
		mnemonic_clear();
		return mnemonic;
	}

	std::string legacyMoneroEnglishMnemonicFromGlobalContext()
	{
		return legacyMoneroEnglishMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroEnglishMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroEnglishMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroEnglishMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		std::string mnemonic(legacy_monero_mnemonic_from_seed(seed, seedSize, MoneroEnglish));
		clear_legacy_monero_mnemonic();
		return mnemonic;
	}

	std::string legacyMoneroGermanMnemonicFromGlobalContext()
	{
		return legacyMoneroGermanMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroGermanMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroGermanMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroGermanMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroSpanishMnemonicFromGlobalContext()
	{
		return legacyMoneroSpanishMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroSpanishMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroSpanishMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroSpanishMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroFrenchMnemonicFromGlobalContext()
	{
		return legacyMoneroFrenchMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroFrenchMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroFrenchMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroFrenchMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroItalianMnemonicFromGlobalContext()
	{
		return legacyMoneroItalianMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroItalianMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroItalianMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroItalianMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroDutchMnemonicFromGlobalContext()
	{
		return legacyMoneroDutchMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroDutchMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroDutchMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroDutchMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroPortugueseMnemonicFromGlobalContext()
	{
		return legacyMoneroPortugueseMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroPortugueseMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroPortugueseMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroPortugueseMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroRussianMnemonicFromGlobalContext()
	{
		return legacyMoneroRussianMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroRussianMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroRussianMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroRussianMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroJapaneseMnemonicFromGlobalContext()
	{
		return legacyMoneroJapaneseMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroJapaneseMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroJapaneseMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroJapaneseMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroChineseMnemonicFromGlobalContext()
	{
		return legacyMoneroChineseMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroChineseMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroChineseMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroChineseMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroEsperantoMnemonicFromGlobalContext()
	{
		return legacyMoneroEsperantoMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroEsperantoMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroEsperantoMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroEsperantoMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}

	std::string legacyMoneroLojbanMnemonicFromGlobalContext()
	{
		return legacyMoneroLojbanMnemonicFromSeed(CoreSystem::getCoreSystem().getContextData());
	}

	std::string legacyMoneroLojbanMnemonicFromSeed(const ContextData& data)
	{
		return legacyMoneroLojbanMnemonicFromSeed(data.seed, data.seedSize);
	}

	std::string legacyMoneroLojbanMnemonicFromSeed(const uint8_t* seed, const uint8_t seedSize)
	{
		return std::string("This seed phrase is currently not supported.");
	}
}
