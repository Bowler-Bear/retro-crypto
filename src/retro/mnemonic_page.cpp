#include "mnemonic_page.h"
#include "core_system.h"
#include "crypto_functions.h"
#include "crypto_definitions.h"

#define STRING(s) #s
#define S(s) STRING(s)

using namespace RetroCrypto;

MnemonicPage::MnemonicPage()
: Page()
{
	seed = "";
	mnemonic = "";
	mnemonicName = "";
}

MnemonicPage::MnemonicPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: MnemonicPage()
{
	title = inTitle;
	parent = inParent;
}

void MnemonicPage::draw(shared_ptr<IDisplay> display)
{
	Page::draw(display);
	drawSeed(display);
	drawMnemonic(display);
}

void MnemonicPage::onEnter()
{
	ContextData contextData = CoreSystem::getCoreSystem().getContextData();
	seed = contextData.getSeedAsHexString();
	switch(contextData.mnemonicType)
	{
	case RetroCrypto::MnemonicType::BIP39:
		mnemonicName = std::string("BIP39(24 words)");
		break;
	case RetroCrypto::MnemonicType::LEGACY_MONERO:
		mnemonicName = std::string("English Legacy Monero Phrase");
		break;
	default:
		mnemonicName = std::string("Seed Phrase");
		break;
	}
	mnemonic = mnemonicFromGlobalContext();
}

void MnemonicPage::onBackward()
{
	Page::onBackward();
	seed = "";
	mnemonic = "";
	mnemonicName = "";
}

void MnemonicPage::drawSeed(shared_ptr<IDisplay> display)
{
	TextBox seedTitleBox(string("Seed Bits"));
	seedTitleBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT;
	seedTitleBox.xPosition = (BASE_BORDER_BOX_WIDTH-seedTitleBox.text.size())/2;
	seedTitleBox.width = seedTitleBox.text.size();
	seedTitleBox.height = 3;
	seedTitleBox.setUnderlined();
	seedTitleBox.setBold();
	display->drawTextBox(seedTitleBox);

	TextBox seedBox(seed);
	seedBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT+3;
	seedBox.xPosition = 2;
	seedBox.width = BASE_BORDER_BOX_WIDTH-3;
	seedBox.height = 3;
	seedBox.setBordered();
	display->drawTextBox(seedBox);
}

void MnemonicPage::drawMnemonic(shared_ptr<IDisplay> display)
{
	TextBox mnemonicTitleBox(mnemonicName);
	mnemonicTitleBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT+5;
	mnemonicTitleBox.xPosition = (BASE_BORDER_BOX_WIDTH-mnemonicTitleBox.text.size())/2;
	mnemonicTitleBox.width = mnemonicTitleBox.text.size();
	mnemonicTitleBox.height = 3;
	mnemonicTitleBox.setUnderlined();
	mnemonicTitleBox.setBold();
	display->drawTextBox(mnemonicTitleBox);

	uint32_t splitPoints[] = { 0, 0, 0, 0 };
	const uint8_t lines = sizeof(splitPoints)/sizeof(uint32_t);
	const uint8_t phraseWordsCount = 24;
	const uint8_t wordsPerLine = phraseWordsCount/lines;
	uint8_t spaceCount = 0;
	for (uint32_t i = 0; i < mnemonic.size(); i++)
	{
		if (mnemonic[i] == ' ')
		{
			spaceCount++;
			if (spaceCount % wordsPerLine == 0)
				splitPoints[spaceCount/wordsPerLine] = i;
			if (spaceCount >= 3*wordsPerLine)
				break;
		}
	};
	for (uint8_t i = 0; i < lines; i++)
	{
		uint32_t splitLength = (i < lines-1 ? splitPoints[i+1] : mnemonic.size())-splitPoints[i];
		TextBox phraseBox(mnemonic.substr(splitPoints[i], splitLength));
		phraseBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT+8+2*i;
		phraseBox.xPosition = 2;
		phraseBox.width = BASE_BORDER_BOX_WIDTH-3;
		phraseBox.height = 3;
		phraseBox.setBordered();
		display->drawTextBox(phraseBox);
	}
}
