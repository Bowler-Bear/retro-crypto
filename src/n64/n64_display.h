#ifndef N64_DISPLAY_H
#define N64_DISPLAY_H

#include <surface.h>

#include "retro_display.h"
#include "unifont_handler.h"

using namespace RetroCrypto;

class N64Display: public IDisplay
{
private:
	surface_t* currentFrame;
	uint8_t blinkFrameCount;
	uint8_t characterScale;
	std::shared_ptr<UnifontHandler> unifont;
public:
	N64Display();
	~N64Display();
	virtual void redraw() override;
	virtual void clear() override;
	virtual void drawBox(const Box& box) override;
	virtual void drawTextBox(const TextBox& textBox) override;
	virtual void drawQrBox(const QrBox& qrBox) override;
	virtual bool isPositionVisible(const int x, const int y) override;
	void setUnifontHandler(std::shared_ptr<UnifontHandler> newUnifontHandler);
	void drawCharacter(const int x, const int y, const char character);
	int drawUnifontCharacter(const int x, const int y, const unsigned char utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER]);
	void increaseCharacterScale();
	void decreaseCharacterScale();
	void resetViewPortOffsets();
	void updateViewPortOffsetX(int addition);
	void updateViewPortOffsetY(int addition);
};
#endif
