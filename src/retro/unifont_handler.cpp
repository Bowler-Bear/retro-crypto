#include <stdexcept>
#include <string>

#include "unifont_handler.h"

using namespace RetroCrypto;


UnifontHandler::UnifontHandler()
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

UnifontHandler::UnifontHandler(std::string inFilePath)
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

bool UnifontHandler::getCharacterPositionsLoaded()
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

void UnifontHandler::loadCharacterPositions()
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

map<uint16_t, uint32_t> UnifontHandler::getCharacterPositions()
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

uint8_t UnifontHandler::getBitmapFromUTF8(const uint8_t utf8[MAXIMUM_UNIFONT_CODE_POINTS_PER_CHARACTER], uint8_t bitmap[MAXIMUM_UNIFONT_BITMAP_SIZE])
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

std::string UnifontHandler::getFilePath()
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

bool UnifontHandler::setFilePath(string newFilePath)
{
	throw std::logic_error(std::string(__func__)+": Not Implemented.");
}

