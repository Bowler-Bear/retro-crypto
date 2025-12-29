#ifndef CRYPTO_FUNCTIONS_TESTS_H
#define CRYPTO_FUNCTIONS_TESTS_H

#define BOOST_TEST_MODULE crypto functions tests

#include <boost/test/included/unit_test.hpp>
#include "crypto_functions.h"

extern "C"
{
#include "base58.h"
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
	/*BOOST_TEST( information.publicKey[0] == expectedPublicBytes[45] );
	for (int i = 0; i < PUBLIC_KEY_BYTE_SIZE; i++)
	{
		BOOST_TEST( masterNode.public_key[0+i] == expectedPublicBytes[45+i] );
	}*/
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


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "1/2'") );

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


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2'/2") );

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


	memset(expectedPublicBytes, 0, 78);
	memset(expectedPrivateBytes, 0, 78);
	testedSubNode = masterNode;
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "m/0'/1/2'/") );
	BOOST_REQUIRE( moveToSubNode(&testedSubNode, "2/1000000000") );

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
}

BOOST_AUTO_TEST_SUITE_END()

#endif
