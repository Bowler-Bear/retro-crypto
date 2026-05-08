#ifndef UNIFONT_TESTS_H
#define UNIFONT_TESTS_H

#define BOOST_TEST_MODULE unifont tests

#include <boost/test/included/unit_test.hpp>
#include "unifont_handler.h"

using namespace RetroCrypto;

BOOST_AUTO_TEST_SUITE( unifont_handler )

const char* testFile = "./tests/unifont_test.hex";

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
	unsigned char testCharacter[MAXIMUM_UTF8_BYTES_PER_CHARACTER] = "ó\0";
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

BOOST_AUTO_TEST_CASE( utf8_bytes_to_code_point )
{
	UnifontHandler handler(testFile);
	uint16_t codePoint = handler.unicodeCodePointFromUTF8Bytes((const uint8_t*)"\u0060\0\0");
	BOOST_REQUIRE( codePoint == 0x0060 );
	codePoint = handler.unicodeCodePointFromUTF8Bytes((const uint8_t*)"\u00f3\0\0");
	BOOST_REQUIRE( codePoint == 0x00f3 );
	codePoint = handler.unicodeCodePointFromUTF8Bytes((const uint8_t*)"\u00f4\0\0");
	BOOST_REQUIRE( codePoint == 0x00f4 );
	codePoint = handler.unicodeCodePointFromUTF8Bytes((const uint8_t*)"\u00f5\0\0");
	BOOST_REQUIRE( codePoint == 0x00f5 );
	codePoint = handler.unicodeCodePointFromUTF8Bytes((const uint8_t*)"\u01f5\0\0");
	BOOST_REQUIRE( codePoint == 0x01f5 );
	codePoint = handler.unicodeCodePointFromUTF8Bytes((const uint8_t*)"\u03f5");
	BOOST_REQUIRE( codePoint == 0x03f5 );
	codePoint = handler.unicodeCodePointFromUTF8Bytes((const uint8_t*)"\u04f5");
	BOOST_REQUIRE( codePoint == 0x04f5 );
	codePoint = handler.unicodeCodePointFromUTF8Bytes((const uint8_t*)"\u3055");
	BOOST_REQUIRE( codePoint == 0x3055 );
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

BOOST_AUTO_TEST_CASE( unicodeCodePointToUTF8Bytes_null_pointer )
{
	UnifontHandler handler(testFile);
	uint8_t outBytes = handler.unicodeCodePointToUTF8Bytes(0x00f3, nullptr);
	BOOST_REQUIRE( outBytes == 0);
}

BOOST_AUTO_TEST_CASE( code_points_to_utf8_bytes )
{
	UnifontHandler handler(testFile);
	uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER] = { 0 };
	uint8_t outBytes = handler.unicodeCodePointToUTF8Bytes(0x00f3, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u00f3", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x00f6, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u00f6", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x00fa, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u00fa", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x00fc, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u00fc", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x0430, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u0430", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x0431, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u0431", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x0432, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u0432", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x0433, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u0433", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x0434, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u0434", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x3052, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u3052", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x3053, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u3053", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x3054, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u3054", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x3055, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u3055", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x3058, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u3058", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
	outBytes = handler.unicodeCodePointToUTF8Bytes(0x3059, utf8);
	BOOST_REQUIRE( memcmp( utf8, "\u3059", outBytes) == 0);
	memset(utf8, 0, MAXIMUM_UTF8_BYTES_PER_CHARACTER);
}

BOOST_AUTO_TEST_SUITE_END()

#endif
