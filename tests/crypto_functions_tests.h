#ifndef CRYPTO_FUNCTIONS_TESTS_H
#define CRYPTO_FUNCTIONS_TESTS_H

#define BOOST_TEST_MODULE crypto functions tests

#include <boost/test/included/unit_test.hpp>
#include "crypto_functions.h"

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

#endif
