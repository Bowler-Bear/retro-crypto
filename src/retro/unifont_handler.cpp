#include <stdexcept>
#include <string>

#include "unifont_handler.h"

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

bool UnifontHandler::getCharacterPositionsLoaded()
{
	return characterPositionsLoaded;
}

void UnifontHandler::loadCharacterPositions()
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

map<uint16_t, uint32_t> UnifontHandler::getCharacterPositions()
{
	return characterPositions;
}

uint8_t UnifontHandler::getBitmapFromUTF8(const uint8_t utf8[MAXIMUM_UNIFONT_CODE_POINTS_PER_CHARACTER], uint8_t bitmap[MAXIMUM_UNIFONT_BITMAP_SIZE])
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

