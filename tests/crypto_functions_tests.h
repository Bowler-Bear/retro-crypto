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

#endif
