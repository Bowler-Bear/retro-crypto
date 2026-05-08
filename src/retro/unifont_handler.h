#ifndef UNIFONT_HANDLER_H
#define UNIFONT_HANDLER_H

#include <cstdint>
#include <map>

#define MAXIMUM_UNIFONT_CHARACTER_PIXEL_WIDTH 16
#define UNIFONT_CHARACTER_PIXEL_HEIGHT 16
#define MAXIMUM_UNIFONT_BITMAP_SIZE (MAXIMUM_UNIFONT_CHARACTER_PIXEL_WIDTH/8)*UNIFONT_CHARACTER_PIXEL_HEIGHT
#define MAXIMUM_UTF8_BYTES_PER_CHARACTER 4
#define UNIFONT_CODE_POINT_LENGTH 4
#define UNIFONT_COLON_LENGTH 1
#define UNIFONT_8_WIDTH_BITMAP_LENGTH 32
#define UNIFONT_8_WIDTH_BITMAP_SIZE (UNIFONT_8_WIDTH_BITMAP_LENGTH/2)
#define UNIFONT_16_WIDTH_BITMAP_LENGTH 64
#define UNIFONT_16_WIDTH_BITMAP_SIZE (UNIFONT_16_WIDTH_BITMAP_LENGTH/2)

using namespace std;

namespace RetroCrypto
{
	class UnifontHandler
	{
	private:
		string filePath;
		FILE* fileHandle;
		map<uint16_t, uint32_t> characterPositions;
		bool characterPositionsLoaded;
	public:
		UnifontHandler();
		UnifontHandler(string inFilePath);
		~UnifontHandler();
		bool getCharacterPositionsLoaded();
		void loadCharacterPositions();
		map<uint16_t, uint32_t> getCharacterPositions();
		uint8_t unicodeCodePointToUTF8Bytes(uint16_t codePoint, uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER]);
		uint16_t unicodeCodePointFromUTF8Bytes(const uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER]);
		uint8_t getBitmapFromUTF8(const uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER], uint8_t bitmap[MAXIMUM_UNIFONT_BITMAP_SIZE]);
		uint8_t getCharacterWidthFromUTF8(const uint8_t utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER]);
		string getFilePath();
		bool setFilePath(string newFilePath);
	};
}
#endif
