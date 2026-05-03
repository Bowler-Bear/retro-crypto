#ifndef UNIFONT_HANDLER_H
#define UNIFONT_HANDLER_H

#include <map>

#define MAXIMUM_UNIFONT_CHARACTER_PIXEL_WIDTH 16
#define UNIFONT_CHARACTER_PIXEL_HEIGHT 16
#define MAXIMUM_UNIFONT_BITMAP_SIZE (MAXIMUM_UNIFONT_CHARACTER_PIXEL_WIDTH/8)*UNIFONT_CHARACTER_PIXEL_HEIGHT
#define MAXIMUM_UNIFONT_CODE_POINTS_PER_CHARACTER 4

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
		bool getCharacterPositionsLoaded();
		void loadCharacterPositions();
		map<uint16_t, uint32_t> getCharacterPositions();
		uint8_t getBitmapFromUTF8(const uint8_t utf8[MAXIMUM_UNIFONT_CODE_POINTS_PER_CHARACTER], uint8_t bitmap[MAXIMUM_UNIFONT_BITMAP_SIZE]);
		string getFilePath();
		bool setFilePath(string newFilePath);
	};
}
#endif
