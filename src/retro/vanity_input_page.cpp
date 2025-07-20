#include "vanity_input_page.h"
#include "core_system.h"
#include "crypto_definitions.h"

extern "C"
{
#include "base58.h"
}

static const char* hexCharSet = "0123456789abcdef";

static const char* bech32CharSet = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

using namespace RetroCrypto;

VanityInputPage::VanityInputPage()
: InputPage()
{
	prefix = "";
}

VanityInputPage::VanityInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: InputPage(inTitle, inParent)
{
	prefix = "";
}

void VanityInputPage::draw(shared_ptr<IDisplay> display)
{
	Page::draw(display);
	drawInput(display);
}

void VanityInputPage::onEnter()
{
	ContextData contextData = CoreSystem::getCoreSystem().getContextData();
	switch (contextData.crypto)
	{
	case CryptoType::BTC:
	case CryptoType::DOGE:
	//MONERO_MAXIMUM_ADDRESS_LENGTH is longer than the default page width limit monero addresses to bitcoin length for now
	case CryptoType::XMR:
		setUsedCharSet(b58digits_ordered);
		setStringSize(BITCOIN_MAXIMUM_ADDRESS_LENGTH-1);
		break;
	case CryptoType::ETC:
	case CryptoType::ETH:
		setUsedCharSet(hexCharSet);
		setStringSize(2*ETHEREUM_ADDRESS_BYTES);
		break;
	case CryptoType::NOSTR:
		setUsedCharSet(bech32CharSet);
		setStringSize((2*NOSTR_PUBLIC_KEY_SIZE)-1);
		break;
	default:
		setStringSize(1);
		break;
	}
	switch (contextData.crypto)
	{
	case CryptoType::BTC:
		prefix = "1";
		break;
	case CryptoType::DOGE:
		prefix = "D";
		break;
	case CryptoType::ETC:
	case CryptoType::ETH:
		prefix = "0x";
		break;
	case CryptoType::NOSTR:
		prefix = "npub1";
		break;
	case CryptoType::XMR:
		prefix = "4";
		break;
	default:
		prefix = "";
		break;
	}
	InputPage::onEnter();
}

void VanityInputPage::drawInput(shared_ptr<IDisplay> display)
{
	if (usedCharSet == nullptr)
		return;
	TextBox prefixBox(prefix);
	prefixBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8;
	prefixBox.xPosition = (BASE_BORDER_BOX_WIDTH-stringSize)/2-prefix.size();
	prefixBox.width = prefix.size();
	prefixBox.height = 1;
	display->drawTextBox(prefixBox);
	for(size_t i = 0; i < inputString.size(); i++)
	{
		int32_t charIndex = inputString[i];
		TextBox inputBox;
		inputBox.text = string(1, charIndex == -1 ? '-' : usedCharSet[charIndex]);
		inputBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8;
		inputBox.xPosition = (BASE_BORDER_BOX_WIDTH-stringSize)/2+i;
		inputBox.width = 1;
		inputBox.height = 1;
		if (i == selectedOptionIndex)
		{
			inputBox.setBold();
			TextBox asteriskBox("*");
			asteriskBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8-1;
			asteriskBox.xPosition = (BASE_BORDER_BOX_WIDTH-stringSize)/2+i;
			asteriskBox.width = 1;
			asteriskBox.height = 1;
			display->drawTextBox(asteriskBox);
			asteriskBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8+1;
			display->drawTextBox(asteriskBox);
		}
		display->drawTextBox(inputBox);
	}
}
