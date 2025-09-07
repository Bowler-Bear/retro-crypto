#include "vanity_input_page.h"
#include "core_system.h"
#include "crypto_definitions.h"
#include "crypto_functions.h"
#include "context_update_functions.h"

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
	currentState = INPUT;
}

VanityInputPage::VanityInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: InputPage(inTitle, inParent)
{
	prefix = "";
	currentState = INPUT;
}

bool VanityInputPage::canConsumeAllInputs()
{
	return true;
}

bool VanityInputPage::consumeInput(InputType input)
{

	switch (currentState)
	{
	case GENERATING:
		if (input != BACK)
			return false;
		generationData.currentAttempt = 0;
		currentState = INPUT;
		return true;
	case FOUND:
		return false;
	case INPUT:
	default:
		if (input != FORWARD)
			return false;
		while (selectedOptionIndex != 0 && inputString[selectedOptionIndex] == -1)
		{
			updateSelectedOption(InputType::LEFT);
		}
		if (selectedOptionIndex == 0 && inputString[selectedOptionIndex] == -1)
			break;
		generationData.matchString = prefix;
		for (size_t i = 0; i < inputString.size(); i++)
		{
			if (inputString[i] == -1)
				break;
			generationData.matchString += usedCharSet[inputString[i]];
		}
		currentState = GENERATING;
	}
	return true;
}

void VanityInputPage::reset()
{
	InputPage::reset();
	prefix = "";
	currentState = INPUT;
	generationData = GenerationData();
	generationData.seedData.crypto = CoreSystem::getCoreSystem().getContextData().crypto;
}

void VanityInputPage::onForward()
{
	MenuTreeObject::onForward();
	CoreSystem::getCoreSystem().updateContextData(ContextUpdate::SEED | ContextUpdate::SEED_SIZE, generationData.seedData);
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
	reset();
	switch (contextData.crypto)
	{
	case CryptoType::BTC:
		description = "Enter the bitcoin address you want to generate.";
		prefix = "1";
		break;
	case CryptoType::DOGE:
		description = "Enter the doge address you want to generate.";
		prefix = "D";
		break;
	case CryptoType::ETC:
	case CryptoType::ETH:
		description = "Enter the ethereum address you want to generate.";
		prefix = "0x";
		break;
	case CryptoType::NOSTR:
		description = "Enter the nostr public key you want to generate.";
		prefix = "npub1";
		break;
	case CryptoType::XMR:
		description = "Enter the monero address you want to generate.";
		prefix = "4";
		break;
	default:
		prefix = "";
		break;
	}
}

void VanityInputPage::onExit()
{
	clearCryptoContext();
}

void VanityInputPage::draw(shared_ptr<IDisplay> display)
{
	switch (currentState)
	{
	case GENERATING:
	case FOUND:
		drawBorder(display);
		drawGenerationPage(display);
		break;
	case INPUT:
	default:
		Page::draw(display);
		drawInput(display);
		drawDescription(display);
		break;
	}
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

void VanityInputPage::tick()
{
	switch (currentState)
	{
	case GENERATING:
		{
		shared_ptr<IRandomNumberGenerator> generator = CoreSystem::getCoreSystem().getRandomNumberGenerator();
		for (uint8_t i = 0; i < DEFAULT_SEED_SIZE; i++)
			generationData.seedData.seed[i] = generator->getRandom8();
		generationData.address = cryptoAddressFromContextData(generationData.seedData).address;
		generationData.currentAttempt++;
		if (generationData.seedData.crypto == CryptoType::ETC || generationData.seedData.crypto == CryptoType::ETH)
			for (uint8_t i = 0; i < generationData.address.size(); i++)
				if (generationData.address[i] >= 'A' && generationData.address[i] <= 'Z')
					generationData.address[i] = generationData.address[i] - ('Z' - 'z');
		if (generationData.address.find(generationData.matchString) == 0)
		{
			currentState = FOUND;
		}
		}
		break;
	case FOUND:
	case INPUT:
	default:
		return;
	}
}

void VanityInputPage::drawGenerationPage(shared_ptr<IDisplay> display)
{
	string title = currentState == FOUND ? "Address Found" : "Generating Address";
	TextBox titleBox(title);
	titleBox.yPosition = PAGE_TITLE_BOX_Y_POSITION;
	titleBox.xPosition = (BASE_BORDER_BOX_WIDTH-titleBox.text.size())/2;
	titleBox.width = titleBox.text.size()+1;
	titleBox.height = PAGE_TITLE_BOX_HEIGHT;
	titleBox.setUnderlined();
	titleBox.setBold();
	titleBox.setBordered();
	display->drawTextBox(titleBox);

	TextBox attemptBox(std::string("Attempt: ")+std::to_string(generationData.currentAttempt+1));
	attemptBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT;
	attemptBox.xPosition = (BASE_BORDER_BOX_WIDTH-attemptBox.text.size())/2;
	attemptBox.width = attemptBox.text.size()+1;
	attemptBox.height = PAGE_TITLE_BOX_HEIGHT;
	attemptBox.setBordered();
	display->drawTextBox(attemptBox);

	TextBox matchStringBox(std::string("Matching: ")+generationData.matchString);
	matchStringBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+2*PAGE_TITLE_BOX_HEIGHT;
	matchStringBox.xPosition = (BASE_BORDER_BOX_WIDTH-matchStringBox.text.size())/2;
	matchStringBox.width = matchStringBox.text.size()+1;
	matchStringBox.height = PAGE_TITLE_BOX_HEIGHT;
	matchStringBox.setBordered();
	display->drawTextBox(matchStringBox);

	string addressTitle = currentState == FOUND ? "Found Address: " : "Current Address: ";
	TextBox addressBox(addressTitle+generationData.address);
	addressBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+3*PAGE_TITLE_BOX_HEIGHT;
	addressBox.xPosition = (BASE_BORDER_BOX_WIDTH-addressBox.text.size())/2;
	addressBox.width = addressBox.text.size()+1;
	addressBox.height = PAGE_TITLE_BOX_HEIGHT;
	addressBox.setBordered();
	display->drawTextBox(addressBox);
}

shared_ptr<MenuTreeObject> VanityInputPage::getDestination()
{
	return destination;
}
