#include <stdexcept>
#include <cstring>
#include <string>

#include "unifont_handler.h"

#define READ_BUFFER_SIZE 128

using namespace RetroCrypto;

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

uint16_t UnifontHandler::unicodeCodePointFromUTF8Bytes(const uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER])
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

uint8_t UnifontHandler::getBitmapFromUTF8(const uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER], uint8_t bitmap[MAXIMUM_UNIFONT_BITMAP_SIZE])
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
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

