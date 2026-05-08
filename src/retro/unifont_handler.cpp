#include <cstring>
#include <string>

#include "unifont_handler.h"

#define READ_BUFFER_SIZE 128

#define REPLACEMENT_CHARACTER_CODE_POINT_VALUE 0xfffd

#define shiftNibbleLeft(value, offset) \
	((value) << (4*(offset)))

#define maskThenShiftRight(value, mask, shift) \
	(((mask) & (value)) >> (shift))

#define maskThenShiftLeft(value, mask, shift) \
	(((mask) & (value)) << (shift))

using namespace RetroCrypto;

char replacementCharacterBitmap[UNIFONT_8_WIDTH_BITMAP_SIZE] = { 0x00, 0x00, 0x00, 0x7E, 0x66, 0x5A, 0x5A, 0x7A, 0x76, 0x76, 0x7E, 0x76, 0x76, 0x7E, 0x00, 0x00 };

UnifontHandler::UnifontHandler()
{
	filePath = "";
	fileHandle =  nullptr;
	characterPositionsLoaded = false;
}

UnifontHandler::UnifontHandler(std::string inFilePath)
: UnifontHandler()
{
	filePath = inFilePath;
}

UnifontHandler::~UnifontHandler()
{
	if (fileHandle)
	{
		fclose(fileHandle);
	}
}

bool UnifontHandler::getCharacterPositionsLoaded()
{
	return characterPositionsLoaded;
}

void UnifontHandler::loadCharacterPositions()
{
	if (filePath == "" || characterPositionsLoaded)
		return;
	fileHandle = fopen(filePath.c_str(), "r");
	if (!fileHandle)
	{
		return;
	}
	char* buffer = (char*)malloc(READ_BUFFER_SIZE);
	uint32_t bufferOffset = 0;
	size_t nread;
	while ((nread = fread(buffer, sizeof(buffer[0]), READ_BUFFER_SIZE-1, fileHandle)) != 0)
	{
		buffer[nread] = '\0';
		char* substring = buffer;
		while (substring != nullptr)
		{
			char* lineEnd = strchr(substring, '\n');
			if (lineEnd == nullptr)
			{
				bufferOffset += (substring - buffer);
				fseek(fileHandle, substring - &buffer[nread], SEEK_CUR);
				break;
			}
			auto lineSize = lineEnd - substring;
			if (
			(lineSize == (UNIFONT_CODE_POINT_LENGTH+UNIFONT_COLON_LENGTH+UNIFONT_8_WIDTH_BITMAP_LENGTH)
			|| lineSize == (UNIFONT_CODE_POINT_LENGTH+UNIFONT_COLON_LENGTH+UNIFONT_16_WIDTH_BITMAP_LENGTH))
			&& substring[UNIFONT_CODE_POINT_LENGTH] == ':')
			{
				uint16_t codePoint = (uint16_t)strtol(substring, nullptr, 16);
				uint32_t bitmapPosition = bufferOffset + (substring - buffer) + UNIFONT_CODE_POINT_LENGTH + UNIFONT_COLON_LENGTH;
				characterPositions[codePoint] = bitmapPosition;
			}
			substring = lineEnd + 1;
		}
	}
	free(buffer);
	characterPositionsLoaded = true;
}

map<uint16_t, uint32_t> UnifontHandler::getCharacterPositions()
{
	return characterPositions;
}

uint8_t UnifontHandler::unicodeCodePointToUTF8Bytes(uint16_t codePoint, uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER])
{
	if (utf8 == nullptr)
		return 0;
	if (codePoint <= 0x007f)
	{
		utf8[0] = codePoint;
		return 1;
	}
	else if (codePoint >= 0x0080 && codePoint <= 0x07ff)
	{
		utf8[0] = 0xc0 | ((codePoint & 0xf00) >> 6) | ((codePoint & 0xf0) >> 6);
		utf8[1] = 0x80 | (codePoint & 0x30) | (codePoint & 0xf);
		return 2;
	}
	else if (codePoint >= 0x0800 && codePoint <= 0xffff)
	{
		utf8[0] = 0xe0 | ((codePoint & 0xf000) >> 12);
		utf8[1] = 0x80 | ((codePoint & 0xf00) >> 6) | ((codePoint & 0xf0) >> 6);
		utf8[2] = 0x80 | (codePoint & 0x30) | (codePoint & 0xf);
		return 3;
	}
	return 0;
}

uint16_t UnifontHandler::unicodeCodePointFromUTF8Bytes(const uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER])
{
	uint16_t codePoint = REPLACEMENT_CHARACTER_CODE_POINT_VALUE;
	if ((utf8[0] & 0x80) == 0x0)
	{
		codePoint = utf8[0];
	}
	else if ((utf8[0] & 0xe0) == 0xc0)
	{
		codePoint = shiftNibbleLeft(maskThenShiftRight(utf8[0], 0x1c, 2), 2) | shiftNibbleLeft(maskThenShiftLeft(utf8[0], 0x3, 2) | maskThenShiftRight(utf8[1], 0x30, 4), 1) | (0xf & utf8[1]);
	}
	else if ((utf8[0] & 0xf0) == 0xe0)
	{
		codePoint = shiftNibbleLeft(maskThenShiftLeft(utf8[0], 0xf, 0), 3) | shiftNibbleLeft(maskThenShiftRight(utf8[1], 0x3C, 2), 2) | shiftNibbleLeft(maskThenShiftLeft(utf8[1], 0x3, 2) | maskThenShiftRight(utf8[2], 0x30, 4), 1) | (0xf & utf8[2]);
	}
	else if ((utf8[0] & 0xf8) == 0xf0)
	{
		//Use a replacement character when font is over three bytes for now
		codePoint = REPLACEMENT_CHARACTER_CODE_POINT_VALUE;
	}
	return codePoint;
}

uint8_t UnifontHandler::getBitmapFromUTF8(const uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER], uint8_t bitmap[MAXIMUM_UNIFONT_BITMAP_SIZE])
{
	if (!fileHandle || !characterPositionsLoaded)
	{
		loadCharacterPositions();
		if (!fileHandle || !characterPositionsLoaded)
			return 0;
	}
	uint16_t codePoint = unicodeCodePointFromUTF8Bytes(utf8);
	if (characterPositions.count(codePoint) <= 0)
	{
		if (bitmap != nullptr)
			memcpy(bitmap, replacementCharacterBitmap, UNIFONT_8_WIDTH_BITMAP_LENGTH/2);
		return MAXIMUM_UNIFONT_CHARACTER_PIXEL_WIDTH/2;
	}
	else
	{
		fseek(fileHandle, characterPositions[codePoint], SEEK_SET);
		uint8_t bitmapBuffer[UNIFONT_16_WIDTH_BITMAP_LENGTH] = { 0 };
		size_t readCount = fread(bitmapBuffer, sizeof(bitmapBuffer[0]), UNIFONT_16_WIDTH_BITMAP_LENGTH, fileHandle);
		if (readCount != UNIFONT_16_WIDTH_BITMAP_LENGTH && readCount != UNIFONT_8_WIDTH_BITMAP_LENGTH)
		{
			if (bitmap != nullptr)
				memcpy(bitmap, replacementCharacterBitmap, UNIFONT_8_WIDTH_BITMAP_LENGTH/2);
			return MAXIMUM_UNIFONT_CHARACTER_PIXEL_WIDTH/2;
		}
		uint8_t characterWidth = 0;
		for (int i = 0; i < UNIFONT_16_WIDTH_BITMAP_LENGTH; i+=2)
		{
			if (i == UNIFONT_8_WIDTH_BITMAP_LENGTH-2)
				characterWidth = 8;
			else if (i == UNIFONT_16_WIDTH_BITMAP_LENGTH-2)
				characterWidth = 16;
			if (bitmapBuffer[i] == '\n')
			{
				break;
			}
			unsigned char bytesString[3] = { 0 };
			memcpy(bytesString, &bitmapBuffer[i], 2);
			if (bitmap != nullptr)
				bitmap[i/2] = (uint8_t)(0xff & strtol((const char*)bytesString, nullptr, 16));
		}
		return characterWidth;
	}
}

uint8_t UnifontHandler::getCharacterWidthFromUTF8(const uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER])
{
	return getBitmapFromUTF8(utf8, nullptr);
}

std::string UnifontHandler::getFilePath()
{
	return filePath;
}

bool UnifontHandler::setFilePath(string newFilePath)
{
	if (characterPositionsLoaded || fileHandle != nullptr)
		return false;
	filePath = newFilePath;
	return true;
}

