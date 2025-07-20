#include "seed_gen_input_page.h"
#include "context_update_functions.h"

#define DEFAULT_INPUT_SIZE 35

static const char* directionalCharSet = "UDLR";

static const char* dieCharSet = "123456";

using namespace RetroCrypto;

SeedGenInputPage::SeedGenInputPage()
: InputPage()
{
	pageType = DICE;
	setStringSize(DEFAULT_INPUT_SIZE);
	setUsedCharSet(dieCharSet);
}

SeedGenInputPage::SeedGenInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: InputPage(inTitle, inParent)
{
	pageType = DICE;
	setStringSize(DEFAULT_INPUT_SIZE);
	setUsedCharSet(dieCharSet);
}

void SeedGenInputPage::updateSelectedOption(InputType input)
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

shared_ptr<MenuTreeObject> SeedGenInputPage::getDestination()
{
	if (pageType != DIRECTIONAL)
		return InputPage::getDestination();
	if (selectedOptionIndex == 0 && inputString[selectedOptionIndex] == -1)
		return nullptr;
	return destination;
}

void SeedGenInputPage::onForward()
{
	uint8_t data[0xFF] = { 0 };
	for (uint8_t i = 0; i < stringSize; i++)
		data[i] = usedCharSet[inputString[i]];
	setSeedFromHashedData(data, stringSize);
}

void SeedGenInputPage::setSeedGenInputType(SeedGenPageType newPageType)
{
	pageType = newPageType;
	switch (pageType)
	{
	case DICE:
		setUsedCharSet(dieCharSet);
		break;
	case DIRECTIONAL:
		setUsedCharSet(directionalCharSet);
		break;
	default:
		break;
	}
}
