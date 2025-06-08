#ifndef CONTROL_SEQUENCES_H
#define CONTROL_SEQUENCES_H

#include <string>

using namespace std;

struct TerminalColor
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	TerminalColor(uint8_t r, uint8_t g, uint8_t b)
	: red(r), green(g), blue(b)
	{
	}
};

struct ColorSettings
{
	TerminalColor background;
	TerminalColor foreground;

	ColorSettings(TerminalColor bg, TerminalColor fg)
	: background(bg), foreground(fg)
	{
	}
};

class ControlSequences
{
public:
	static string sequenceIntroducer;

	static void flush()
	{
		cout.flush();
	}

	static void sendSequence(string sequence)
	{
		cout << sequence;
	}

	static string setCursorPostion(int y, int x)
	{
		string sequence(sequenceIntroducer);
		sequence += to_string(y);
		sequence += ";";
		sequence += to_string(x);
		sequence += "H";
		return sequence;
	}

	static void sendSetCursorPostion(int y, int x)
	{
		sendSequence(setCursorPostion(y, x));
	}

	static string eraseDisplay(int option)
	{
		string sequence(sequenceIntroducer);
		sequence += to_string(option);
		sequence += "J";
		return sequence;
	}

	static void sendEraseDisplay(int option)
	{
		sendSequence(eraseDisplay(option));
	}

	static void sendEraseShown()
	{
		sendEraseDisplay(2);
	}

	static void sendClearScrollback()
	{
		sendEraseDisplay(3);
	}

	static void sendClearDisplay()
	{
		sendEraseShown();
		sendClearScrollback();
	}

	static string singleParameterGraphicRendition(uint8_t parameter)
	{
		string sequence(sequenceIntroducer);
		sequence += to_string(parameter);
		sequence += "m";
		return sequence;
	}

	static string resetGraphicRendition()
	{
		return singleParameterGraphicRendition(0);
	}

	static void sendResetGraphicRendition()
	{
		sendSequence(resetGraphicRendition());
	}

	static string setForegroundColor(uint8_t red, uint8_t green, uint8_t blue)
	{
		string sequence(sequenceIntroducer);
		sequence += "38;2;";
		sequence += to_string(red);
		sequence += ";";
		sequence += to_string(green);
		sequence += ";";
		sequence += to_string(blue);
		sequence += "m";
		return sequence;
	}

	static string setForegroundColor(TerminalColor color)
	{
		return setForegroundColor(color.red, color.green, color.blue);
	}

	static void sendSetForegroundColor(uint8_t red, uint8_t green, uint8_t blue)
	{
		sendSequence(setForegroundColor(red, green, blue));
	}

	static void sendSetForegroundColor(TerminalColor color)
	{
		sendSequence(setForegroundColor(color));
	}

	static string setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue)
	{
		string sequence(sequenceIntroducer);
		sequence += "48;2;";
		sequence += to_string(red);
		sequence += ";";
		sequence += to_string(green);
		sequence += ";";
		sequence += to_string(blue);
		sequence += "m";
		return sequence;
	}

	static string setBackgroundColor(TerminalColor color)
	{
		return setBackgroundColor(color.red, color.green, color.blue);
	}

	static void sendSetBackgroundColor(uint8_t red, uint8_t green, uint8_t blue)
	{
		sendSequence(setBackgroundColor(red, green, blue));
	}

	static void sendSetBackgroundColor(TerminalColor color)
	{
		sendSequence(setBackgroundColor(color));
	}

	static void sendRestoreColorSettings(ColorSettings settings)
	{
		sendSetBackgroundColor(settings.background);
		sendSetForegroundColor(settings.foreground);
	}

	static string setBold()
	{
		return singleParameterGraphicRendition(1);
	}

	static void sendSetBold()
	{
		sendSequence(setBold());
	}

	static string unsetBold()
	{
		return singleParameterGraphicRendition(22);
	}

	static void sendUnsetBold()
	{
		sendSequence(unsetBold());
	}

	static string setItalic()
	{
		return singleParameterGraphicRendition(3);
	}

	static void sendSetItalic()
	{
		sendSequence(setItalic());
	}

	static string unsetItalic()
	{
		return singleParameterGraphicRendition(23);
	}

	static void sendUnsetItalic()
	{
		sendSequence(unsetItalic());
	}

	static string setUnderlined()
	{
		return singleParameterGraphicRendition(4);
	}

	static void sendSetUnderlined()
	{
		sendSequence(setUnderlined());
	}

	static string unsetUnderlined()
	{
		return singleParameterGraphicRendition(24);
	}

	static void sendUnsetUnderlined()
	{
		sendSequence(unsetUnderlined());
	}

	static string setOverlined()
	{
		return singleParameterGraphicRendition(53);
	}

	static void sendSetOverlined()
	{
		sendSequence(setOverlined());
	}

	static string unsetOverlined()
	{
		return singleParameterGraphicRendition(55);
	}

	static void sendUnsetOverlined()
	{
		sendSequence(unsetOverlined());
	}

	static string setSlowBlink()
	{
		return singleParameterGraphicRendition(5);
	}

	static void sendSetSlowBlink()
	{
		sendSequence(setSlowBlink());
	}

	static string unsetSlowBlink()
	{
		return singleParameterGraphicRendition(25);
	}

	static void sendUnsetSlowBlink()
	{
		sendSequence(unsetSlowBlink());
	}

	static void sendText(string text, uint8_t textProperties = 0)
	{
		if (textProperties & TextProperties::TEXT_BOLD)
			sendSetBold();
		if (textProperties & TextProperties::TEXT_ITALIC)
			sendSetItalic();
		if (textProperties & TextProperties::TEXT_UNDERLINED)
			sendSetUnderlined();
		if (textProperties & TextProperties::TEXT_OVERLINED)
			sendSetOverlined();
		if (textProperties & TextProperties::TEXT_BLINK)
			sendSetSlowBlink();
		cout << text;
		if (textProperties & TextProperties::TEXT_BOLD)
			sendUnsetBold();
		if (textProperties & TextProperties::TEXT_ITALIC)
			sendUnsetItalic();
		if (textProperties & TextProperties::TEXT_UNDERLINED)
			sendUnsetUnderlined();
		if (textProperties & TextProperties::TEXT_OVERLINED)
			sendUnsetOverlined();
		if (textProperties & TextProperties::TEXT_BLINK)
			sendUnsetSlowBlink();
	}

	static void sendText(TextBox textBox)
	{
		if (textBox.isBold())
			sendSetBold();
		if (textBox.isItalic())
			sendSetItalic();
		if (textBox.isUnderlined())
			sendSetUnderlined();
		if (textBox.isOverlined())
			sendSetOverlined();
		if (textBox.isBlinking())
			sendSetSlowBlink();
		cout << textBox.text;
		if (textBox.isBold())
			sendUnsetBold();
		if (textBox.isItalic())
			sendUnsetItalic();
		if (textBox.isUnderlined())
			sendUnsetUnderlined();
		if (textBox.isOverlined())
			sendUnsetOverlined();
		if (textBox.isBlinking())
			sendUnsetSlowBlink();
	}
};

string ControlSequences::sequenceIntroducer = "\x1b[";
#endif
