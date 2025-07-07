#ifndef RETRO_DISPLAY_H
#define RETRO_DISPLAY_H

#include <cstdint>
#include <string>

#include "drawable.h"

namespace RetroCrypto
{
	namespace TextProperties
	{
		static uint8_t TEXT_BORDERED = 0x01;
		static uint8_t TEXT_BOLD = 0x01 << 1;
		static uint8_t TEXT_ITALIC = 0x01 << 2;
		static uint8_t TEXT_UNDERLINED = 0x01 << 3;
		static uint8_t TEXT_OVERLINED = 0x01 << 4;
		static uint8_t TEXT_BLINK = 0x01 << 5;
	}

	struct Box
	{
		int xPosition;
		int yPosition;
		int width;
		int height;
	};

	struct TextBox : public Box
	{
		std::string text;
		uint8_t textProperties;

		TextBox(uint8_t properties = 0)
		: textProperties(properties)
		{
		}

		TextBox(const std::string& text)
		: TextBox(0)
		{
			this->text = text;
		}

		bool isBordered() const
		{
			return textProperties & TextProperties::TEXT_BORDERED;
		}

		void setBordered()
		{
			textProperties |= TextProperties::TEXT_BORDERED;
		}

		bool isBold() const
		{
			return textProperties & TextProperties::TEXT_BOLD;
		}

		void setBold()
		{
			textProperties |= TextProperties::TEXT_BOLD;
		}

		bool isItalic() const
		{
			return textProperties & TextProperties::TEXT_ITALIC;
		}

		void setItalic()
		{
			textProperties |= TextProperties::TEXT_ITALIC;
		}

		bool isUnderlined() const
		{
			return textProperties & TextProperties::TEXT_UNDERLINED;
		}

		void setUnderlined()
		{
			textProperties |= TextProperties::TEXT_UNDERLINED;
		}

		bool isOverlined() const
		{
			return textProperties & TextProperties::TEXT_OVERLINED;
		}

		void setOverlined()
		{
			textProperties |= TextProperties::TEXT_OVERLINED;
		}

		bool isBlinking() const
		{
			return textProperties & TextProperties::TEXT_BLINK;
		}

		void setBlinking()
		{
			textProperties |= TextProperties::TEXT_BLINK;
		}
	};

	class IDisplay: public IDrawable
	{
	public:
		IDisplay()
		{
		}
		virtual void redraw() override = 0;
		virtual void drawBox(const Box& box) = 0;
		virtual void drawTextBox(const TextBox& textBox) = 0;
		virtual void clear() override = 0;
	};
}
#endif
