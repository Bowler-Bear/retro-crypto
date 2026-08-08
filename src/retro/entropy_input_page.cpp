#include <cstring>
#include <math.h>

#include "entropy_input_page.h"
#include "context_update_functions.h"

#define DICE_INPUT_SIZE 100
#define DIRECTIONAL_INPUT_SIZE 128
#define COIN_INPUT_SIZE 255

static const char* directionalCharSet = "UDLR";

static const char* dieCharSet = "123456";

static const char* coinCharSet = "HT";

using namespace RetroCrypto;

EntropyInputPage::EntropyInputPage()
: InputPage()
{
	pageType = DICE;
	setStringSize(DICE_INPUT_SIZE);
	setUsedCharSet(dieCharSet);
}

EntropyInputPage::EntropyInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: InputPage(inTitle, inParent)
{
	pageType = DICE;
	setStringSize(DICE_INPUT_SIZE);
	setUsedCharSet(dieCharSet);
}

void EntropyInputPage::updateSelectedOption(InputType input)
{
	if (usedCharSet == nullptr)
		return;
	if (pageType != DIRECTIONAL)
		return InputPage::updateSelectedOption(input);
	switch (input)
	{
	case InputType::UP:
		inputString[selectedOptionIndex++] = 0;
		break;
	case InputType::DOWN:
		inputString[selectedOptionIndex++] = 1;
		break;
	case InputType::LEFT:
		inputString[selectedOptionIndex++] = 2;
		break;
	case InputType::RIGHT:
		inputString[selectedOptionIndex++] = 3;
		break;
	default:
		break;
	}
	if (selectedOptionIndex >= stringSize)
		selectedOptionIndex = 0;
}

void EntropyInputPage::draw(shared_ptr<IDisplay> display)
{
	InputPage::draw(display);
	drawCollisionOdds(display);
}

shared_ptr<MenuTreeObject> EntropyInputPage::getDestination()
{
	if (pageType != DIRECTIONAL)
		return InputPage::getDestination();
	if (selectedOptionIndex == 0 && inputString[selectedOptionIndex] == -1)
		return nullptr;
	return destination;
}

void EntropyInputPage::onForward()
{
	MenuTreeObject::onForward();
	uint8_t data[0xFF] = { 0 };
	for (uint8_t i = 0; i < stringSize; i++)
		if (inputString[i] == -1)
			break;
		else
			data[i] = usedCharSet[inputString[i]];
	setSeedFromHashedData(data, stringSize);
}

void EntropyInputPage::setEntropyInputType(EntropyType newPageType)
{
	pageType = newPageType;
	switch (pageType)
	{
	case DICE:
		setUsedCharSet(dieCharSet);
		setStringSize(DICE_INPUT_SIZE);
		break;
	case DIRECTIONAL:
		setUsedCharSet(directionalCharSet);
		setStringSize(DIRECTIONAL_INPUT_SIZE);
		break;
	case COIN:
		setUsedCharSet(coinCharSet);
		setStringSize(COIN_INPUT_SIZE);
		break;
	default:
		break;
	}
}

void EntropyInputPage::drawCollisionOdds(shared_ptr<IDisplay> display)
{
	if (usedCharSet == nullptr)
		return;
	if (selectedOptionIndex == 0 && inputString[selectedOptionIndex] == -1)
		return;
	std::string odds = "Current Entropy: ";
	uint8_t highestIndex = 1;
	for (uint8_t i = stringSize-1; i > 0; i--)
	{
		if (inputString[i] != -1)
		{
			highestIndex = i+1;
			break;
		}
	}
	int16_t entropy = -1;
	try
	{
		entropy = static_cast<int16_t>(highestIndex*log(std::strlen(usedCharSet))/log(2));
	}
	catch(...)
	{
	}
	odds += entropy > 0 ? std::to_string(min(entropy, (int16_t)256)) : "?";
	odds += " bits";
	TextBox oddsBox(odds);
	oddsBox.yPosition = BASE_BORDER_BOX_HEIGHT-6-4;
	oddsBox.xPosition = 2;
	oddsBox.width = BASE_BORDER_BOX_WIDTH-3;
	oddsBox.height = 5;
	oddsBox.setBordered();
	display->drawTextBox(oddsBox);
}
