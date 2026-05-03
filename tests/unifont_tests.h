#ifndef UNIFONT_TESTS_H
#define UNIFONT_TESTS_H

#define BOOST_TEST_MODULE unifont tests

#include <boost/test/included/unit_test.hpp>
#include "unifont_handler.h"

using namespace RetroCrypto;

BOOST_AUTO_TEST_SUITE( unifont_handler )

const char* testFile = "./unifont_test.hex";

BOOST_AUTO_TEST_CASE( getters_setters )
{
	UnifontHandler handler(testFile);
	BOOST_REQUIRE( handler.getFilePath() == testFile );
	const char* newFilename = "abc.txt";
	BOOST_REQUIRE( handler.setFilePath(newFilename) );
	BOOST_REQUIRE( handler.getFilePath() == newFilename );
	BOOST_REQUIRE( handler.getCharacterPositionsLoaded() == false );
}

BOOST_AUTO_TEST_CASE( no_file_provided )
{
	UnifontHandler handler;
	BOOST_REQUIRE( handler.getFilePath() == "" );
	handler.loadCharacterPositions();
	BOOST_REQUIRE( handler.getCharacterPositionsLoaded() == false );
}

BOOST_AUTO_TEST_CASE( no_file_set_after_load )
{
	UnifontHandler handler(testFile);
	BOOST_REQUIRE( handler.getFilePath() == testFile );
	BOOST_REQUIRE( handler.getCharacterPositionsLoaded() == false );
	handler.loadCharacterPositions();
	BOOST_REQUIRE( handler.getCharacterPositionsLoaded() == true );
	const char* newFilename = "abc.txt";
	BOOST_REQUIRE( !handler.setFilePath(newFilename) );
}

BOOST_AUTO_TEST_CASE( get_specific_bitmaps )
{
	UnifontHandler handler(testFile);
	BOOST_REQUIRE( handler.getCharacterPositionsLoaded() == false );

	uint8_t bitmap[MAXIMUM_UNIFONT_BITMAP_SIZE] = { 0 };
	unsigned char testCharacter[MAXIMUM_UNIFONT_CODE_POINTS_PER_CHARACTER] = "ó\0";
	BOOST_REQUIRE( handler.getBitmapFromUTF8(testCharacter, bitmap) == 8 );
	BOOST_REQUIRE( handler.getCharacterPositionsLoaded() == true );
	const uint8_t expectedBitmap8[MAXIMUM_UNIFONT_BITMAP_SIZE] = { 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00 };
	BOOST_REQUIRE( memcmp(bitmap, expectedBitmap8, 16) == 0 );

	memcpy(testCharacter, "げ", sizeof("げ"));
	BOOST_REQUIRE( handler.getBitmapFromUTF8(testCharacter, bitmap) == 16 );
	const uint8_t expectedBitmap16[MAXIMUM_UNIFONT_BITMAP_SIZE] = { 0x00, 0x00, 0x01, 0x08, 0x40, 0xA4, 0x20, 0x50, 0x20, 0x40, 0x20, 0x78, 0x47, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x30, 0x40, 0x20, 0x80, 0x20, 0x80, 0x01, 0x00, 0x00, 0x00 };
	BOOST_REQUIRE( memcmp(bitmap, expectedBitmap16, 32) == 0 );

	memcpy(testCharacter, "\xff\xff\xff\xff", 4);
	BOOST_REQUIRE( handler.getBitmapFromUTF8(testCharacter, bitmap) == 8 );
	const uint8_t expectedReplacementBitmap[MAXIMUM_UNIFONT_BITMAP_SIZE] = { 0x00, 0x00, 0x00, 0x7E, 0x66, 0x5A, 0x5A, 0x7A, 0x76, 0x76, 0x7E, 0x76, 0x76, 0x7E, 0x00, 0x00 };
	BOOST_REQUIRE( memcmp(bitmap, expectedReplacementBitmap, 16) == 0 );
}

BOOST_AUTO_TEST_CASE( get_all_bitmap_positions )
{
	UnifontHandler handler(testFile);
	map<uint16_t, uint32_t> returnedMap = handler.getCharacterPositions();
	BOOST_REQUIRE( returnedMap.size() == 0 );
	handler.loadCharacterPositions();
	returnedMap = handler.getCharacterPositions();
	BOOST_REQUIRE( returnedMap.size() == 15 );
	BOOST_REQUIRE( returnedMap.count(0x00f6) == 1 );
	BOOST_REQUIRE( returnedMap[0x00f6] == 43 );
	BOOST_REQUIRE( returnedMap.count(0x3053) == 1 );
	BOOST_REQUIRE( returnedMap[0x3053] == 542 );
}

BOOST_AUTO_TEST_SUITE_END()

#endif
