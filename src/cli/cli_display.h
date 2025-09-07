#ifndef CLI_DISPLAY_H
#define CLI_DISPLAY_H

#include "retro_display.h"

using namespace RetroCrypto;

class CLIDisplay: public IDisplay
{
public:
	CLIDisplay();
	~CLIDisplay();
	virtual void redraw() override;
	virtual void clear() override;
	virtual void drawBox(const Box& box) override;
	virtual void drawTextBox(const TextBox& textBox) override;
	virtual void drawQrBox(const QrBox& qrBox) override;
};
#endif
