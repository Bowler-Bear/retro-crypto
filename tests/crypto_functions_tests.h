#ifndef CRYPTO_FUNCTIONS_TESTS_H
#define CRYPTO_FUNCTIONS_TESTS_H

#define BOOST_TEST_MODULE crypto functions tests

#include <boost/test/included/unit_test.hpp>
#include "crypto_functions.h"

extern "C"
{
#include "base58.h"
#include "bip39.h"
}

using namespace RetroCrypto;

BOOST_AUTO_TEST_SUITE( crypto_functions_AddressInformation )

const uint8_t incremented_values[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };

BOOST_AUTO_TEST_CASE( getPrivateKeyAsHexString )
{
	AddressInformation addressInformation;
	memcpy(addressInformation.privateKey, incremented_values, 32);
	BOOST_REQUIRE( addressInformation.getPrivateKeyAsHexString() == std::string("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));
}

BOOST_AUTO_TEST_CASE( getPublicKeyAsHexString )
{
	AddressInformation addressInformation;
	memcpy(addressInformation.publicKey, incremented_values, 32);
	BOOST_REQUIRE( addressInformation.getPublicKeyAsHexString() == std::string("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f00"));
}

BOOST_AUTO_TEST_CASE( getPrivateSpendKeyAsHexString )
{
	AddressInformation addressInformation;
	memcpy(addressInformation.privateSpendKey, incremented_values, 32);
	BOOST_REQUIRE( addressInformation.getPrivateSpendKeyAsHexString() == std::string("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));
}

BOOST_AUTO_TEST_CASE( getPrivateViewKeyAsHexString )
{
	AddressInformation addressInformation;
	memcpy(addressInformation.privateViewKey, incremented_values, 32);
	BOOST_REQUIRE( addressInformation.getPrivateViewKeyAsHexString() == std::string("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( crypto_functions_AddressPath )

BOOST_AUTO_TEST_CASE( defaultAddressPathConstructor )
{
	AddressPath path;
	BOOST_REQUIRE( path.getIsValidPath() == false );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == false );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
}

BOOST_AUTO_TEST_CASE( stringAddressPathConstructor )
{
	AddressPath path("m/0/1'/2/3");
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 0 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 1 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 2 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == 3 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
}

BOOST_AUTO_TEST_CASE( masterOnlyAddressPath )
{
	AddressPath path("m");
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
}

BOOST_AUTO_TEST_CASE( masterOnlyWithSlashAddressPath )
{
	AddressPath path("m/");
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
}

BOOST_AUTO_TEST_CASE( endWithSlashAddressPath )
{
	AddressPath path("m/1/");
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == 1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
}

BOOST_AUTO_TEST_CASE( endWithHardenedAddressPath )
{
	AddressPath path("m/1'");
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == 1 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
}

BOOST_AUTO_TEST_CASE( endWithHardenedSlashAddressPath )
{
	AddressPath path("m/1'/");
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == 1 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
}

BOOST_AUTO_TEST_CASE( everySingleDigitAddressPath )
{
	AddressPath path("m/0/1/2/3/4/5/6/7/8/9");
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 0 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 2 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 3 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 4 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 5 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 6 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 7 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 8 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == 9 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
}

BOOST_AUTO_TEST_CASE( everySingleDigitHardenedAddressPath )
{
	AddressPath path("m/0'/1'/2'/3'/4'/5'/6'/7'/8'/9'");
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == -1 );
	BOOST_REQUIRE( path.getIsValueHardened() == false );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 0 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 1 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 2 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 3 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 4 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 5 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 6 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 7 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == true );
	BOOST_REQUIRE( path.getHeadValue() == 8 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
	path = path.getSubPath();
	BOOST_REQUIRE( path.getIsValidPath() == true );
	BOOST_REQUIRE( path.hasSubPath() == false );
	BOOST_REQUIRE( path.getHeadValue() == 9 );
	BOOST_REQUIRE( path.getIsValueHardened() == true );
}

BOOST_AUTO_TEST_CASE( invalidCharacterAtHeadOfAddressPath )
{
	for (char currentCharacter = 'a'; currentCharacter <= 'z'; currentCharacter++)
	{
		if (currentCharacter == 'm')
			continue;
		char pathString[10] = { 0 };
		sprintf(pathString, "%c/1'/56", currentCharacter);
		BOOST_REQUIRE( pathString[0] == currentCharacter );
		AddressPath path(pathString);
		BOOST_REQUIRE( path.getIsValidPath() == false );
	}
	for (char currentCharacter = 'A'; currentCharacter <= 'Z'; currentCharacter++)
	{
		char pathString[10] = { 0 };
		sprintf(pathString, "%c/1'/56", currentCharacter);
		BOOST_REQUIRE( pathString[0] == currentCharacter );
		AddressPath path(pathString);
		BOOST_REQUIRE( path.getIsValidPath() == false );
	}
}

BOOST_AUTO_TEST_CASE( invalidCharacterInAddressPath )
{
	for (char currentCharacter = 'a'; currentCharacter <= 'z'; currentCharacter++)
	{
		char pathString[10] = { 0 };
		sprintf(pathString, "m/%c/1'/", currentCharacter);
		BOOST_REQUIRE( pathString[2] == currentCharacter );
		AddressPath path(pathString);
		BOOST_REQUIRE( path.getIsValidPath() == false );
	}
	for (char currentCharacter = 'A'; currentCharacter <= 'Z'; currentCharacter++)
	{
		char pathString[10] = { 0 };
		sprintf(pathString, "m/%c/1'/", currentCharacter);
		BOOST_REQUIRE( pathString[2] == currentCharacter );
		AddressPath path(pathString);
		BOOST_REQUIRE( path.getIsValidPath() == false );
	}
}

BOOST_AUTO_TEST_CASE( doubleSlashInAddressPath )
{
	AddressPath path("m/0//1'/");
	BOOST_REQUIRE( path.getIsValidPath() == false );
}

BOOST_AUTO_TEST_CASE( doubleHardenInAddressPath )
{
	AddressPath path("m/0/1''/");
	BOOST_REQUIRE( path.getIsValidPath() == false );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( crypto_functions_moveToSubNode )

//https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki#user-content-Test_Vectors
BOOST_AUTO_TEST_CASE( moveToSubNode_bip32TestVector1 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeed[seedSize] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

	HDNode masterNode;
	BOOST_TEST( hdnode_from_seed(testSeed, seedSize, BITCOIN_ELLIPTIC_CURVE, &masterNode) == 1 );
	BOOST_TEST( hdnode_fill_public_key(&masterNode) == 0 );

	uint8_t expectedPublicBytes[78] = { 0x00 };
	base58_decode_check("xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(masterNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(masterNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	HDNode testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );

	base58_decode_check("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1") );

	base58_decode_check("xpub6ASuArnXKPbfEwhqN6e3mwBcDTgzisQN1wXN9BJcM47sSikHjJf3UFHKkNAWbWMiGj7Wf5uMash7SyYq527Hqck2AxYysAA7xmALppuCkwQ", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9wTYmMFdV23N2TdNG573QoEsfRrWKQgWeibmLntzniatZvR9BmLnvSxqu53Kw1UmYPxLgboyZQaXwTCg8MSY3H2EU4pWcQDnRnrVA1xe8fs", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1/2'") );

	base58_decode_check("xpub6D4BDPcP2GT577Vvch3R8wDkScZWzQzMMUm3PWbmWvVJrZwQY4VUNgqFJPMM3No2dFDFGTsxxpG5uJh7n7epu4trkrX7x7DogT5Uv6fcLW5", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9z4pot5VBttmtdRTWfWQmoH1taj2axGVzFqSb8C9xaxKymcFzXBDptWmT7FwuEzG3ryjH4ktypQSAewRiNMjANTtpgP4mLTj34bhnZX7UiM", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1/2'/2") );

	base58_decode_check("xpub6FHa3pjLCk84BayeJxFW2SP4XRrFd1JYnxeLeU8EqN3vDfZmbqBqaGJAyiLjTAwm6ZLRQUMv1ZACTj37sR62cfN7fe5JnJ7dh8zL4fiyLHV", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprvA2JDeKCSNNZky6uBCviVfJSKyQ1mDYahRjijr5idH2WwLsEd4Hsb2Tyh8RfQMuPh7f7RtyzTtdrbdqqsunu5Mm3wDvUAKRHSC34sJ7in334", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1/2'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1/2'/2/1000000000") );

	base58_decode_check("xpub6H1LXWLaKsWFhvm6RVpEL9P4KfRZSW7abD2ttkWP3SSQvnyA8FSVqNTEcYFgJS2UaFcxupHiYkro49S8yGasTvXEYBVPamhGW6cFJodrTHy", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprvA41z7zogVVwxVSgdKUHDy1SKmdb533PjDz7J6N6mV6uS3ze1ai8FHa8kmHScGpWmj4WggLyQjgPie1rFSruoUihUZREPSL39UNdE3BBDu76", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'/2/1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2'/2/1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2'/2/1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1/2'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2/1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1/2'/2") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2'/2/1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2'/2/1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2/1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'/2") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1/2'/2/1000000000") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( moveToSubNode_bip32TestVector2 )
{
	const uint8_t seedSize = 64;
	const uint8_t testSeed[seedSize] = { 0xff, 0xfc, 0xf9, 0xf6, 0xf3, 0xf0, 0xed, 0xea, 0xe7, 0xe4, 0xe1, 0xde, 0xdb, 0xd8, 0xd5, 0xd2, 0xcf, 0xcc, 0xc9, 0xc6, 0xc3, 0xc0, 0xbd, 0xba, 0xb7, 0xb4, 0xb1, 0xae, 0xab, 0xa8, 0xa5, 0xa2, 0x9f, 0x9c, 0x99, 0x96, 0x93, 0x90, 0x8d, 0x8a, 0x87, 0x84, 0x81, 0x7e, 0x7b, 0x78, 0x75, 0x72, 0x6f, 0x6c, 0x69, 0x66, 0x63, 0x60, 0x5d, 0x5a, 0x57, 0x54, 0x51, 0x4e, 0x4b, 0x48, 0x45, 0x42 };

	HDNode masterNode;
	BOOST_TEST( hdnode_from_seed(testSeed, seedSize, BITCOIN_ELLIPTIC_CURVE, &masterNode) == 1 );
	BOOST_TEST( hdnode_fill_public_key(&masterNode) == 0 );

	uint8_t expectedPublicBytes[78] = { 0x00 };
	base58_decode_check("xpub661MyMwAqRbcFW31YEwpkMuc5THy2PSt5bDMsktWQcFF8syAmRUapSCGu8ED9W6oDMSgv6Zz8idoc4a6mr8BDzTJY47LJhkJ8UB7WEGuduB", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(masterNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K31xYSDQpPDxsXRTUcvj2iNHm5NUtrGiGG5e2DtALGdso3pGz6ssrdK4PFmM8NSpSBHNqPqm55Qn3LqFtT2emdEXVYsCzC2U", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(masterNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	HDNode testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0") );

	base58_decode_check("xpub69H7F5d8KSRgmmdJg2KhpAK8SR3DjMwAdkxj3ZuxV27CprR9LgpeyGmXUbC6wb7ERfvrnKZjXoUmmDznezpbZb7ap6r1D3tgFxHmwMkQTPH", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9vHkqa6EV4sPZHYqZznhT2NPtPCjKuDKGY38FBWLvgaDx45zo9WQRUT3dKYnjwih2yJD9mkrocEZXo1ex8G81dwSM1fwqWpWkeS3v86pgKt", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'") );

	base58_decode_check("xpub6ASAVgeehLbnwdqV6UKMHVzgqAG8Gr6riv3Fxxpj8ksbH9ebxaEyBLZ85ySDhKiLDBrQSARLq1uNRts8RuJiHjaDMBU4Zn9h8LZNnBC5y4a", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9wSp6B7kry3Vj9m1zSnLvN3xH8RdsPP1Mh7fAaR7aRLcQMKTR2vidYEeEg2mUCTAwCd6vnxVrcjfy2kRgVsFawNzmjuHc2YmYRmagcEPdU9", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483647'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483647'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'/1") );

	base58_decode_check("xpub6DF8uhdarytz3FWdA8TvFSvvAh8dP3283MY7p2V4SeE2wyWmG5mg5EwVvmdMVCQcoNJxGoWaU9DCWh89LojfZ537wTfunKau47EL2dhHKon", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9zFnWC6h2cLgpmSA46vutJzBcfJ8yaJGg8cX1e5StJh45BBciYTRXSd25UEPVuesF9yog62tGAQtHjXajPPdbRCHuWS6T8XA2ECKADdw4Ef", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483647'/1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483647'/1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'/1/2147483646'") );

	base58_decode_check("xpub6ERApfZwUNrhLCkDtcHTcxd75RbzS1ed54G1LkBUHQVHQKqhMkhgbmJbZRkrgZw4koxb5JaHWkY4ALHY2grBGRjaDMzQLcgJvLJuZZvRcEL", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprvA1RpRA33e1JQ7ifknakTFpgNXPmW2YvmhqLQYMmrj4xJXXWYpDPS3xz7iAxn8L39njGVyuoseXzU6rcxFLJ8HFsTjSyQbLYnMpCqE2VbFWc", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1/2147483646'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483647'/1/2147483646'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2147483646'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483646'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483647'/1/2147483646'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2147483646'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483646'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1/2147483646'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'/1/2147483646'/2") );

	base58_decode_check("xpub6FnCn6nSzZAw5Tw7cgR9bi15UV96gLZhjDstkXXxvCLsUXBGXPdSnLFbdpq8p9HmGsApME5hQTZ3emM2rnY5agb9rXpVGyy3bdW6EEgAtqt", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprvA2nrNbFZABcdryreWet9Ea4LvTJcGsqrMzxHx98MMrotbir7yrKCEXw7nadnHM8Dq38EGfSh6dqA9QWTyefMLEcBYJUuekgW4BYPJcr9E7j", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1/2147483646'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483647'/1/2147483646'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2147483646'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483646'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0/2147483647'/1/2147483646'/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483647'/1/2147483646'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2147483646'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2147483646'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1/2147483646'/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0/2147483647'/1/2147483646'/2") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( moveToSubNode_bip32TestVector3 )
{
	const uint8_t seedSize = 64;
	const uint8_t testSeed[seedSize] = { 0x4b, 0x38, 0x15, 0x41, 0x58, 0x3b, 0xe4, 0x42, 0x33, 0x46, 0xc6, 0x43, 0x85, 0x0d, 0xa4, 0xb3, 0x20, 0xe4, 0x6a, 0x87, 0xae, 0x3d, 0x2a, 0x4e, 0x6d, 0xa1, 0x1e, 0xba, 0x81, 0x9c, 0xd4, 0xac, 0xba, 0x45, 0xd2, 0x39, 0x31, 0x9a, 0xc1, 0x4f, 0x86, 0x3b, 0x8d, 0x5a, 0xb5, 0xa0, 0xd0, 0xc6, 0x4d, 0x2e, 0x8a, 0x1e, 0x7d, 0x14, 0x57, 0xdf, 0x2e, 0x5a, 0x3c, 0x51, 0xc7, 0x32, 0x35, 0xbe };

	HDNode masterNode;
	BOOST_TEST( hdnode_from_seed(testSeed, seedSize, BITCOIN_ELLIPTIC_CURVE, &masterNode) == 1 );
	BOOST_TEST( hdnode_fill_public_key(&masterNode) == 0 );

	uint8_t expectedPublicBytes[78] = { 0x00 };
	base58_decode_check("xpub661MyMwAqRbcEZVB4dScxMAdx6d4nFc9nvyvH3v4gJL378CSRZiYmhRoP7mBy6gSPSCYk6SzXPTf3ND1cZAceL7SfJ1Z3GC8vBgp2epUt13", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(masterNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K25QhxbucbDDuQ4naNntJRi4KUfWT7xo4EKsHt2QJDu7KXp1A3u7Bi1j8ph3EGsZ9Xvz9dGuVrtHHs7pXeTzjuxBrCmmhgC6", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(masterNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	HDNode testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );

	base58_decode_check("xpub68NZiKmJWnxxS6aaHmn81bvJeTESw724CRDs6HbuccFQN9Ku14VQrADWgqbhhTHBaohPX4CjNLf9fq9MYo6oDaPPLPxSb7gwQN3ih19Zm4Y", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9uPDJpEQgRQfDcW7BkF7eTya6RPxXeJCqCJGHuCJ4GiRVLzkTXBAJMu2qaMWPrS7AANYqdq6vcBcBUdJCVVFceUvJFjaPdGZ2y9WACViL4L", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( moveToSubNode_bip32TestVector4 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeed[seedSize] = { 0x3d, 0xdd, 0x56, 0x02, 0x28, 0x58, 0x99, 0xa9, 0x46, 0x11, 0x45, 0x06, 0x15, 0x7c, 0x79, 0x97, 0xe5, 0x44, 0x45, 0x28, 0xf3, 0x00, 0x3f, 0x61, 0x34, 0x71, 0x21, 0x47, 0xdb, 0x19, 0xb6, 0x78 };

	HDNode masterNode;
	BOOST_TEST( hdnode_from_seed(testSeed, seedSize, BITCOIN_ELLIPTIC_CURVE, &masterNode) == 1 );
	BOOST_TEST( hdnode_fill_public_key(&masterNode) == 0 );

	uint8_t expectedPublicBytes[78] = { 0x00 };
	base58_decode_check("xpub661MyMwAqRbcGczjuMoRm6dXaLDEhW1u34gKenbeYqAix21mdUKJyuyu5F1rzYGVxyL6tmgBUAEPrEz92mBXjByMRiJdba9wpnN37RLLAXa", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(masterNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K48vGoLGRPxgo2JNkJ3J3fqkirQC2zVdk5Dgd5w14S7fRDyHH4dWNHUgkvsvNDCkvAwcSHNAQwhwgNMgZhLtQC63zxwhQmRv", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(masterNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	HDNode testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );

	base58_decode_check("xpub69AUMk3qDBi3uW1sXgjCmVjJ2G6WQoYSnNHyzkmdCHEhSZ4tBok37xfFEqHd2AddP56Tqp4o56AePAgCjYdvpW2PU2jbUPFKsav5ut6Ch1m", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9vB7xEWwNp9kh1wQRfCCQMnZUEG21LpbR9NPCNN1dwhiZkjjeGRnaALmPXCX7SgjFTiCTT6bXes17boXtjq3xLpcDjzEuGLQBM5ohqkao9G", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1'") );

	base58_decode_check("xpub6BJA1jSqiukeaesWfxe6sNK9CCGaujFFSJLomWHprUL9DePQ4JDkM5d88n49sMGJxrhpjazuXYWdMf17C9T5XnxkopaeS7jGk1GyyVziaMt", HASHER_SHA2D, expectedPublicBytes, 78);
	BOOST_REQUIRE( expectedPublicBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPublicBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPublicBytes[2] == 0xb2 );
	BOOST_REQUIRE( expectedPublicBytes[3] == 0x1e );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );

	base58_decode_check("xprv9xJocDuwtYCMNAo3Zw76WENQeAS6WGXQ55RCy7tDJ8oALr4FWkuVoHJeHVAcAqiZLE7Je3vZJHxspZdFHfnBEjHqU5hG1Jaj32dVoS6XLT1", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "0'/1'") );
	BOOST_REQUIRE( memcmp(testedSubNode.public_key, &expectedPublicBytes[45], PUBLIC_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(testedSubNode.private_key, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

//https://github.com/dogecoin/dogecoin/blob/master/src/wallet/test/wallet_tests.cpp
BOOST_AUTO_TEST_CASE( moveToSubNode_dogeTestVector )
{
	const uint8_t testPrivateKey[PRIVATE_KEY_BYTE_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
	const uint8_t testExpectedPublicKey[PUBLIC_KEY_BYTE_SIZE] = { 0x03, 0x05, 0xa0, 0x77, 0x19, 0x43, 0x00, 0xe2, 0x7d, 0x32, 0x0a, 0x95, 0x04, 0xf8, 0x08, 0xa1, 0x6f, 0x05, 0xb3, 0x8d, 0xab, 0xea, 0xd3, 0x1f, 0x10, 0x10, 0x4c, 0x07, 0x5d, 0x88, 0xf8, 0x1a, 0x38 };
	const char* testPath = "m/0'/3'/0'";

	HDNode masterNode;
	uint8_t chainCode[32] = { 1 };
	hdnode_from_seed(testPrivateKey, PRIVATE_KEY_BYTE_SIZE, BITCOIN_ELLIPTIC_CURVE, &masterNode);
	BOOST_REQUIRE( moveToSubNode(&masterNode, testPath) );
	BOOST_REQUIRE( memcmp(masterNode.public_key, testExpectedPublicKey, PUBLIC_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( crypto_functions_bitcoin )

//https://github.com/trezor/python-mnemonic/blob/master/vectors.json
BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector1 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeedBits[seedSize] = { 0x00 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3h3fDYiay8mocZ3afhfULfb5GX8kCBdno77K4HiA15Tg23wpbeF1pLfs1c5SPmYHrEpTuuRhxMwvKDwqdKiGJS9XFKzUsAF", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector2 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeedBits[seedSize] = { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K2gA81bYFHqU68xz1cX2APaSq5tt6MFSLeXnCKV1RVUJt9FWNTbrrryem4ZckN8k4Ls1H6nwdvDTvnV7zEXs2HgPezuVccsq", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector3 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeedBits[seedSize] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K2shfP28KM3nr5Ap1SXjz8gc2rAqqMEynmjt6o1qboCDpxckqXavCwdnYds6yBHZGKHv7ef2eTXy461PXUjBFQg6PrwY4Gzq", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector4 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeedBits[seedSize] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K2V4oox4M8Zmhi2Fjx5XK4Lf7GKRvPSgydU3mjZuKGCTg7UPiBUD7ydVPvSLtg9hjp7MQTYsW67rZHAXeccqYqrsx8LcXnyd", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector5 )
{
	const uint8_t seedSize = 24;
	const uint8_t testSeedBits[seedSize] = { 0x00 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3mEDrypcZ2usWqFgzKB6jBBx9B6GfC7fu26X6hPRzVjzkqkPvDqp6g5eypdk6cyhGnBngbjeHTe4LsuLG1cCmKJka5SMkmU", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector6 )
{
	const uint8_t seedSize = 24;
	const uint8_t testSeedBits[seedSize] = { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3Lv9MZLj16np5GzLe7tDKQfVusBni7toqJGcnKRtHSxUwbKUyUWiwpK55g1DUSsw76TF1T93VT4gz4wt5RM23pkaQLnvBh7", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector7 )
{
	const uint8_t seedSize = 24;
	const uint8_t testSeedBits[seedSize] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3VPCbxbUtpkh9pRG371UCLDz3BjceqP1jz7XZsQ5EnNkYAEkfeZp62cDNj13ZTEVG1TEro9sZ9grfRmcYWLBhCocViKEJae", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector8 )
{
	const uint8_t seedSize = 24;
	const uint8_t testSeedBits[seedSize] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K36Ao5jHRVhFGDbLP6FCx8BEEmpru77ef3bmA928BxsqvVM27WnvvyfWywiFN8K6yToqMaGYfzS6Db1EHAXT5TuyCLBXUfdm", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector9 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeedBits[seedSize] = { 0x00 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K32qBagUJAMU2LsHg3ka7jqMcV98Y7gVeVyNStwYS3U7yVVoDZ4btbRNf4h6ibWpY22iRmXq35qgLs79f312g2kj5539ebPM", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector10 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeedBits[seedSize] = { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3Y1sd2XVu9wtqxJRvybCfAetjUrMMco6r3v9qZTBeXiBZkS8JxWbcGJZyio8TrZtm6pkbzG8SYt1sxwNLh3Wx7to5pgiVFU", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector11 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeedBits[seedSize] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3CSnQNYC3MqAAqHwxeTLhDbhF43A4ss4ciWNmCY9zQGvAKUSqVUf2vPHBTSE1rB2pg4avopqSiLVzXEU8KziNnVPauTqLRo", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector12 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeedBits[seedSize] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K2WFF16X85T2QCpndrGwx6GueB72Zf3AHwHJaknRXNF37ZmDrtHrrLSHvbuRejXcnYxoZKvRquTPyp2JiNG3XcjQyzSEgqCB", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector13 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeedBits[seedSize] = { 0x9e, 0x88, 0x5d, 0x95, 0x2a, 0xd3, 0x62, 0xca, 0xeb, 0x4e, 0xfe, 0x34, 0xa8, 0xe9, 0x1b, 0xd2 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K2oZ9stBYpoaZ2ktHj7jLz7iMqpgg1En8kKFTXJHsjxry1JbKH19YrDTicVwKPehFKTbmaxgVEc5TpHdS1aYhB2s9aFJBeJH", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector14 )
{
	const uint8_t seedSize = 24;
	const uint8_t testSeedBits[seedSize] = { 0x66, 0x10, 0xb2, 0x59, 0x67, 0xcd, 0xcc, 0xa9, 0xd5, 0x98, 0x75, 0xf5, 0xcb, 0x50, 0xb0, 0xea, 0x75, 0x43, 0x33, 0x11, 0x86, 0x9e, 0x93, 0x0b };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3uT8eQowUjsxrmsA9YUuQQK1RLqFufzybxD6DH6gPY7NjJ5G3EPHjsWDrs9iivSbmvjc9DQJbJGatfa9pv4MZ3wjr8qWPAK", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector15 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeedBits[seedSize] = { 0x68, 0xa7, 0x9e, 0xac, 0xa2, 0x32, 0x48, 0x73, 0xea, 0xcc, 0x50, 0xcb, 0x9c, 0x6e, 0xca, 0x8c, 0xc6, 0x8e, 0xa5, 0xd9, 0x36, 0xf9, 0x87, 0x87, 0xc6, 0x0c, 0x7e, 0xbc, 0x74, 0xe6, 0xce, 0x7c };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K2XTAhys3pMNcGn261Fi5Ta2Pw8PwaVPhg3D8DWkzWQwjTJfskj8ofb81i9NP2cUNKxwjueJHHMQAnxtivTA75uUFqPFeWzk", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector16 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeedBits[seedSize] = { 0xc0, 0xba, 0x5a, 0x8e, 0x91, 0x41, 0x11, 0x21, 0x0f, 0x2b, 0xd1, 0x31, 0xf3, 0xd5, 0xe0, 0x8d };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3FperxDp8vFsFycKCRcJGAFmcV7umQmcnMZaLtZRt13QJDsoS5F6oYT6BB4sS6zmTmyQAEkJKxJ7yByDNtRe5asP2jFGhT6", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector17 )
{
	const uint8_t seedSize = 24;
	const uint8_t testSeedBits[seedSize] = { 0x6d, 0x9b, 0xe1, 0xee, 0x6e, 0xbd, 0x27, 0xa2, 0x58, 0x11, 0x5a, 0xad, 0x99, 0xb7, 0x31, 0x7b, 0x9c, 0x8d, 0x28, 0xb6, 0xd7, 0x64, 0x31, 0xc3 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3R1SfVZZLtVbXEB9ryVxmVtVMsMwmEyEvgXN6Q84LKkLRmf4ST6QrLeBm3jQsb9gx1uo23TS7vo3vAkZGZz71uuLCcywUkt", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector18 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeedBits[seedSize] = { 0x9f, 0x6a, 0x28, 0x78, 0xb2, 0x52, 0x07, 0x99, 0xa4, 0x4e, 0xf1, 0x8b, 0xc7, 0xdf, 0x39, 0x4e, 0x70, 0x61, 0xa2, 0x24, 0xd2, 0xc3, 0x3c, 0xd0, 0x15, 0xb1, 0x57, 0xd7, 0x46, 0x86, 0x98, 0x63 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K2WNnKmssvZYM96VAr47iHUQUTUyUXH3sAGNjhJANddnhw3i3y3pBbRAVk5M5qUGFr4rHbEWwXgX4qrvrceifCYQJbbFDems", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector19 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeedBits[seedSize] = { 0x23, 0xdb, 0x81, 0x60, 0xa3, 0x1d, 0x3e, 0x0d, 0xca, 0x36, 0x88, 0xed, 0x94, 0x1a, 0xdb, 0xf3 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K4G28omGMogEoYgDQuigBo8AFHAGDaJdqQ99QKMQ5J6fYTMfANTJy6xBmhvsNZ1CJzRZ64PWbnTFUn6CDV2FxoMDLXdk95DQ", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector20 )
{
	const uint8_t seedSize = 24;
	const uint8_t testSeedBits[seedSize] = { 0x81, 0x97, 0xa4, 0xa4, 0x7f, 0x04, 0x25, 0xfa, 0xea, 0xa6, 0x9d, 0xee, 0xbc, 0x05, 0xca, 0x29, 0xc0, 0xa5, 0xb5, 0xcc, 0x76, 0xce, 0xac, 0xc0 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3wtsvY8L2aZyxkiWULZH4vyQE5XkHTXkmx8gHo6RUEfH3Jyr6NwkJhvano7Xb2o6UqFKWHVo5scE31SGDCAUsgVhiUuUDyh", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector21 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeedBits[seedSize] = { 0x06, 0x6d, 0xca, 0x1a, 0x2b, 0xb7, 0xe8, 0xa1, 0xdb, 0x28, 0x32, 0x14, 0x8c, 0xe9, 0x93, 0x3e, 0xea, 0x0f, 0x3a, 0xc9, 0x54, 0x8d, 0x79, 0x31, 0x12, 0xd9, 0xa9, 0x5c, 0x94, 0x07, 0xef, 0xad };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K3rEfqSM4QZRVmiMuSWY9wugscmaCjYja3SbUD3KPEB1a7QXJoajyR2T1SiXU7rFVRXMV9XdYVSZe7JoUXdP4SRHTxsT1nzm", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector22 )
{
	const uint8_t seedSize = 16;
	const uint8_t testSeedBits[seedSize] = { 0xf3, 0x0f, 0x8c, 0x1d, 0xa6, 0x65, 0x47, 0x8f, 0x49, 0xb0, 0x01, 0xd9, 0x4c, 0x5f, 0xc4, 0x52 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K2QWV9Wn8Vvs6jbqfF1YbTCdURQW9dLFKDovpKaKrqS3SEWsXCu6ZNky9PSAENg6c9AQYHcg4PjopRGGKmdD313ZHszymnps", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector23 )
{
	const uint8_t seedSize = 24;
	const uint8_t testSeedBits[seedSize] = { 0xc1, 0x0e, 0xc2, 0x0d, 0xc3, 0xcd, 0x9f, 0x65, 0x2c, 0x7f, 0xac, 0x2f, 0x12, 0x30, 0xf7, 0xa3, 0xc8, 0x28, 0x38, 0x9a, 0x14, 0x39, 0x2f, 0x05 };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K4aERa2bq7559eMCCEs2QmmqVjUuzfy5eAeDX4mqZffkYwpzGQRE2YEEeLVRoH4CSHxianrFaVnMN2RYaPUZJhJx8S5j6puX", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_CASE( bitcoinAddressFromSeedBits_bip39TestVector24 )
{
	const uint8_t seedSize = 32;
	const uint8_t testSeedBits[seedSize] = { 0xf5, 0x85, 0xc1, 0x1a, 0xec, 0x52, 0x0d, 0xb5, 0x7d, 0xd3, 0x53, 0xc6, 0x95, 0x54, 0xb2, 0x1a, 0x89, 0xb2, 0x0f, 0xb0, 0x65, 0x09, 0x66, 0xfa, 0x0a, 0x9d, 0x6f, 0x74, 0xfd, 0x98, 0x9d, 0x8f };

	AddressInformation information = bitcoinAddressFromSeedBits(testSeedBits, seedSize, "m", "TREZOR");

	uint8_t expectedPrivateBytes[78] = { 0x00 };
	base58_decode_check("xprv9s21ZrQH143K39rnQJknpH1WEPFJrzmAqqasiDcVrNuk926oizzJDDQkdiTvNPr2FYDYzWgiMiC63YmfPAa2oPyNB23r2g7d1yiK6WpqaQS", HASHER_SHA2D, expectedPrivateBytes, 78);
	BOOST_REQUIRE( expectedPrivateBytes[0] == 0x04 );
	BOOST_REQUIRE( expectedPrivateBytes[1] == 0x88 );
	BOOST_REQUIRE( expectedPrivateBytes[2] == 0xad );
	BOOST_REQUIRE( expectedPrivateBytes[3] == 0xe4 );
	BOOST_REQUIRE( memcmp(&information.privateKey, &expectedPrivateBytes[46], PRIVATE_KEY_BYTE_SIZE) == 0 );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( crypto_functions_ethereum )

BOOST_AUTO_TEST_CASE( checksum_address_erc55_case1 )
{
	const uint8_t ethereumAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0x52, 0x90, 0x84, 0x00, 0x09, 0x85, 0x27, 0x88, 0x6E, 0x0F, 0x70, 0x30, 0x06, 0x98, 0x57, 0xD2, 0xE4, 0x16, 0x9E, 0xE7 };
	BOOST_REQUIRE( ethereumAddressBytesToChecksumAddress(ethereumAddressBytes) == std::string("0x52908400098527886E0F7030069857D2E4169EE7"));
}

BOOST_AUTO_TEST_CASE( checksum_address_erc55_case2 )
{
	const uint8_t ethereumAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0x86, 0x17, 0xE3, 0x40, 0xB3, 0xD0, 0x1F, 0xA5, 0xF1, 0x1F, 0x30, 0x6F, 0x40, 0x90, 0xFD, 0x50, 0xE2, 0x38, 0x07, 0x0D };
	BOOST_REQUIRE( ethereumAddressBytesToChecksumAddress(ethereumAddressBytes) == std::string("0x8617E340B3D01FA5F11F306F4090FD50E238070D"));
}

BOOST_AUTO_TEST_CASE( checksum_address_erc55_case3 )
{
	const uint8_t ethereumAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0xde, 0x70, 0x9f, 0x21, 0x02, 0x30, 0x62, 0x20, 0x92, 0x10, 0x60, 0x31, 0x47, 0x15, 0x62, 0x90, 0x80, 0xe2, 0xfb, 0x77 };
	BOOST_REQUIRE( ethereumAddressBytesToChecksumAddress(ethereumAddressBytes) == std::string("0xde709f2102306220921060314715629080e2fb77"));
}

BOOST_AUTO_TEST_CASE( checksum_address_erc55_case4 )
{
	const uint8_t ethereumAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0x27, 0xb1, 0xfd, 0xb0, 0x47, 0x52, 0xbb, 0xc5, 0x36, 0x00, 0x7a, 0x92, 0x0d, 0x24, 0xac, 0xb0, 0x45, 0x56, 0x1c, 0x26 };
	BOOST_REQUIRE( ethereumAddressBytesToChecksumAddress(ethereumAddressBytes) == std::string("0x27b1fdb04752bbc536007a920d24acb045561c26"));
}

BOOST_AUTO_TEST_CASE( checksum_address_erc55_case5 )
{
	const uint8_t ethereumAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0x5a, 0xAe, 0xb6, 0x05, 0x3F, 0x3E, 0x94, 0xC9, 0xb9, 0xA0, 0x9f, 0x33, 0x66, 0x94, 0x35, 0xE7, 0xEf, 0x1B, 0xeA, 0xed };
	BOOST_REQUIRE( ethereumAddressBytesToChecksumAddress(ethereumAddressBytes) == std::string("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"));
}

BOOST_AUTO_TEST_CASE( checksum_address_erc55_case6 )
{
	const uint8_t ethereumAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0xfB, 0x69, 0x16, 0x09, 0x5c, 0xa1, 0xdf, 0x60, 0xbB, 0x79, 0xCe, 0x92, 0xcE, 0x3E, 0xa7, 0x4c, 0x37, 0xc5, 0xd3, 0x59 };
	BOOST_REQUIRE( ethereumAddressBytesToChecksumAddress(ethereumAddressBytes) == std::string("0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359"));
}

BOOST_AUTO_TEST_CASE( checksum_address_erc55_case7 )
{
	const uint8_t ethereumAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0xdb, 0xF0, 0x3B, 0x40, 0x7c, 0x01, 0xE7, 0xcD, 0x3C, 0xBe, 0xa9, 0x95, 0x09, 0xd9, 0x3f, 0x8D, 0xDD, 0xC8, 0xC6, 0xFB };
	BOOST_REQUIRE( ethereumAddressBytesToChecksumAddress(ethereumAddressBytes) == std::string("0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB"));
}

BOOST_AUTO_TEST_CASE( checksum_address_erc55_case8 )
{
	const uint8_t ethereumAddressBytes[ETHEREUM_ADDRESS_BYTES] = { 0xD1, 0x22, 0x0A, 0x0c, 0xf4, 0x7c, 0x7B, 0x9B, 0xe7, 0xA2, 0xE6, 0xBA, 0x89, 0xF4, 0x29, 0x76, 0x2e, 0x7b, 0x9a, 0xDb };
	BOOST_REQUIRE( ethereumAddressBytesToChecksumAddress(ethereumAddressBytes) == std::string("0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb"));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( crypto_functions_nostr )

//https://github.com/nostr-protocol/nips
BOOST_AUTO_TEST_CASE( nips06_vector1 )
{
	const int seedSize = 16;
	uint8_t seedBits[seedSize] = { 0 };
	mnemonic_to_bits("leader monkey parrot ring guide accident before fence cannon height naive bean", seedBits);
	AddressInformation information = nostrAddressFromSeedBits(seedBits, seedSize, "m/44'/1237'/0'/0/0");
	uint8_t expectedPrivateKey[PRIVATE_KEY_BYTE_SIZE] = { 0x7f, 0x7f, 0xf0, 0x3d, 0x12, 0x37, 0x92, 0xd6, 0xac, 0x59, 0x4b, 0xfa, 0x67, 0xbf, 0x6d, 0x0c, 0x0a, 0xb5, 0x5b, 0x6b, 0x1f, 0xdb, 0x62, 0x49, 0x30, 0x3f, 0xe8, 0x61, 0xf1, 0xcc, 0xba, 0x9a };
	uint8_t expectedPublicKey[PUBLIC_KEY_BYTE_SIZE-1] = { 0x17, 0x16, 0x2c, 0x92, 0x1d, 0xc4, 0xd2, 0x51, 0x8f, 0x9a, 0x10, 0x1d, 0xb3, 0x36, 0x95, 0xdf, 0x1a, 0xfb, 0x56, 0xab, 0x82, 0xf5, 0xff, 0x3e, 0x5d, 0xa6, 0xee, 0xc3, 0xca, 0x5c, 0xd9, 0x17 };
	BOOST_REQUIRE( memcmp(information.privateKey, &expectedPrivateKey, PRIVATE_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(&information.publicKey[1], &expectedPublicKey, PUBLIC_KEY_BYTE_SIZE-1) == 0 );
	BOOST_REQUIRE( std::string("nsec10allq0gjx7fddtzef0ax00mdps9t2kmtrldkyjfs8l5xruwvh2dq0lhhkp") == nsecFromPrivateKey(information.privateKey) );
	BOOST_REQUIRE( std::string("npub1zutzeysacnf9rru6zqwmxd54mud0k44tst6l70ja5mhv8jjumytsd2x7nu") == information.address );
}

BOOST_AUTO_TEST_CASE( nips06_vector2 )
{
	const int seedSize = 32;
	uint8_t seedBits[seedSize] = { 0 };
	mnemonic_to_bits("what bleak badge arrange retreat wolf trade produce cricket blur garlic valid proud rude strong choose busy staff weather area salt hollow arm fade", seedBits);
	AddressInformation information = nostrAddressFromSeedBits(seedBits, seedSize, "m/44'/1237'/0'/0/0");
	uint8_t expectedPrivateKey[PRIVATE_KEY_BYTE_SIZE] = { 0xc1, 0x5d, 0x73, 0x98, 0x94, 0xc8, 0x1a, 0x2f, 0xcf, 0xd3, 0xa2, 0xdf, 0x85, 0xa0, 0xd2, 0xc0, 0xdb, 0xc4, 0x7a, 0x28, 0x0d, 0x09, 0x27, 0x99, 0xf1, 0x44, 0xd7, 0x3d, 0x7a, 0xe7, 0x8a, 0xdd };
	uint8_t expectedPublicKey[PUBLIC_KEY_BYTE_SIZE-1] = { 0xd4, 0x1b, 0x22, 0x89, 0x95, 0x49, 0xe1, 0xf3, 0xd3, 0x35, 0xa3, 0x10, 0x02, 0xcf, 0xd3, 0x82, 0x17, 0x40, 0x06, 0xe1, 0x66, 0xd3, 0xe6, 0x58, 0xe3, 0xa5, 0xee, 0xcd, 0xb6, 0x46, 0x35, 0x73 };
	BOOST_REQUIRE( memcmp(information.privateKey, &expectedPrivateKey, PRIVATE_KEY_BYTE_SIZE) == 0 );
	BOOST_REQUIRE( memcmp(&information.publicKey[1], &expectedPublicKey, PUBLIC_KEY_BYTE_SIZE-1) == 0 );
	BOOST_REQUIRE( std::string("nsec1c9wh8xy5eqdzln7n5t0ctgxjcrdug73gp5yj0x03gntn67h83twssdfhel") == nsecFromPrivateKey(information.privateKey) );
	BOOST_REQUIRE( std::string("npub16sdj9zv4f8sl85e45vgq9n7nsgt5qphpvmf7vk8r5hhvmdjxx4es8rq74h") == information.address );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( crypto_functions_moneroAddressFromSeed )

BOOST_AUTO_TEST_CASE( zero_32_byte_spend_key )
{
	const uint8_t seed[MONERO_PRIVATE_SPEND_KEY_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	AddressInformation addressInformation = moneroAddressFromSeed(seed, 32);

	const uint8_t expectedPrivateSpendKey[MONERO_PRIVATE_SPEND_KEY_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	BOOST_REQUIRE( memcmp(&expectedPrivateSpendKey, &addressInformation.privateSpendKey, MONERO_PRIVATE_SPEND_KEY_LENGTH) == 0 );

	const uint8_t expectedPrivateViewKey[MONERO_PRIVATE_VIEW_KEY_LENGTH] = { 0x9b, 0x15, 0x29, 0xac, 0xb6, 0x38, 0xf4, 0x97, 0xd0, 0x56, 0x77, 0xd7, 0x50, 0x5d, 0x35, 0x4b, 0x4b, 0xa6, 0xbc, 0x95, 0x48, 0x40, 0x08, 0xf6, 0x36, 0x2f, 0x93, 0x16, 0x0e, 0xf3, 0xe5, 0x03 };
	BOOST_REQUIRE( memcmp(&expectedPrivateViewKey, &addressInformation.privateViewKey, MONERO_PRIVATE_VIEW_KEY_LENGTH) == 0 );

	const std::string expectedAddress = std::string("41fJjQDhryD11111111111111111111111111111111112N1GuTZeagfRbbKcALdcZev4QXGGuoLh2x36LhaxLSxCc2YDhi");
	BOOST_REQUIRE( expectedAddress == addressInformation.address );
}

BOOST_AUTO_TEST_CASE( ab_32_byte_spend_key )
{
	const uint8_t seed[MONERO_PRIVATE_SPEND_KEY_LENGTH] = { 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab };
	AddressInformation addressInformation = moneroAddressFromSeed(seed, 32);

	const uint8_t expectedPrivateSpendKey[MONERO_PRIVATE_SPEND_KEY_LENGTH] = { 0x69, 0x64, 0x11, 0x0a, 0xa4, 0xcc, 0xf3, 0x3a, 0x4c, 0x8b, 0xff, 0x4d, 0xf9, 0xe8, 0xf5, 0xda, 0xaa, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0x0b };
	BOOST_REQUIRE( memcmp(&expectedPrivateSpendKey, &addressInformation.privateSpendKey, MONERO_PRIVATE_SPEND_KEY_LENGTH) == 0 );

	const uint8_t expectedPrivateViewKey[MONERO_PRIVATE_VIEW_KEY_LENGTH] = { 0xf3, 0x6c, 0x07, 0xd5, 0x70, 0xc3, 0x76, 0x45, 0x04, 0xf9, 0x70, 0x49, 0x72, 0x6d, 0xce, 0xc0, 0x72, 0x19, 0x3a, 0x85, 0xbc, 0x2d, 0x2b, 0xa0, 0x2d, 0x58, 0x86, 0x41, 0x82, 0x80, 0xc1, 0x0c };
	BOOST_REQUIRE( memcmp(&expectedPrivateViewKey, &addressInformation.privateViewKey, MONERO_PRIVATE_VIEW_KEY_LENGTH) == 0 );

	const std::string expectedAddress = std::string("46qNWGaUNT2jFiNwExbebhYmwgdHehTLiBVSXmaKvv9N88Kw8CeXTcTSFZjETMFkDNjmSTdv9B2JLGtQDBpdE847PF4VQLy");
	BOOST_REQUIRE( expectedAddress == addressInformation.address );
}

BOOST_AUTO_TEST_CASE( reduced_ab_32_byte_spend_key )
{
	uint8_t seed[MONERO_PRIVATE_SPEND_KEY_LENGTH] = { 0x69, 0x64, 0x11, 0x0a, 0xa4, 0xcc, 0xf3, 0x3a, 0x4c, 0x8b, 0xff, 0x4d, 0xf9, 0xe8, 0xf5, 0xda, 0xaa, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0x0b };
	AddressInformation addressInformation = moneroAddressFromSeed(seed, 32);

	const uint8_t expectedPrivateSpendKey[MONERO_PRIVATE_SPEND_KEY_LENGTH] = { 0x69, 0x64, 0x11, 0x0a, 0xa4, 0xcc, 0xf3, 0x3a, 0x4c, 0x8b, 0xff, 0x4d, 0xf9, 0xe8, 0xf5, 0xda, 0xaa, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0x0b };
	BOOST_REQUIRE( memcmp(&expectedPrivateSpendKey, &addressInformation.privateSpendKey, MONERO_PRIVATE_SPEND_KEY_LENGTH) == 0 );

	const uint8_t expectedPrivateViewKey[MONERO_PRIVATE_VIEW_KEY_LENGTH] = { 0xf3, 0x6c, 0x07, 0xd5, 0x70, 0xc3, 0x76, 0x45, 0x04, 0xf9, 0x70, 0x49, 0x72, 0x6d, 0xce, 0xc0, 0x72, 0x19, 0x3a, 0x85, 0xbc, 0x2d, 0x2b, 0xa0, 0x2d, 0x58, 0x86, 0x41, 0x82, 0x80, 0xc1, 0x0c };
	BOOST_REQUIRE( memcmp(&expectedPrivateViewKey, &addressInformation.privateViewKey, MONERO_PRIVATE_VIEW_KEY_LENGTH) == 0 );

	const std::string expectedAddress = std::string("46qNWGaUNT2jFiNwExbebhYmwgdHehTLiBVSXmaKvv9N88Kw8CeXTcTSFZjETMFkDNjmSTdv9B2JLGtQDBpdE847PF4VQLy");
	BOOST_REQUIRE( expectedAddress == addressInformation.address );
}

BOOST_AUTO_TEST_CASE( seed_sizes_not_32_bytes )
{
	const uint8_t spendKey[64] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for (int i = 0; i < 65; i++)
	{
		if (i == 32)
			continue;
		AddressInformation addressInformation = moneroAddressFromSeed(spendKey, i);
		std::string expectedAddressMemberVariable = std::string("Invalid Seed Size.");
		BOOST_REQUIRE( expectedAddressMemberVariable == addressInformation.address );
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif
