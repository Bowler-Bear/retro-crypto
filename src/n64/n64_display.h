#ifndef N64_DISPLAY_H
#define N64_DISPLAY_H

#include <surface.h>

#include "retro_display.h"

using namespace RetroCrypto;

class N64Display: public IDisplay
{
private:
	surface_t* currentFrame;
	uint8_t blinkFrameCount;
public:
	N64Display();
	~N64Display();
	virtual void redraw() override;
	virtual void clear() override;
	virtual void drawBox(const Box& box) override;
	virtual void drawTextBox(const TextBox& textBox) override;
	virtual void drawQrBox(const QrBox& qrBox) override;
	virtual bool isPositionVisible(const int x, const int y) override;
};
#endif
