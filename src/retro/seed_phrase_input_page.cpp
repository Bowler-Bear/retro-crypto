#include <cstring>

#include "seed_phrase_input_page.h"
#include "core_system.h"
#include "context_update_functions.h"

extern "C"
{
#include "bip39.h"
#include "legacy_monero_mnemonic.h"
}

#define MAXIMUM_WORD_LENGTH 20

static const char* alphabetCharSet = "abcdefghijklmnopqrstuvwxyz";

using namespace RetroCrypto;

SeedPhraseInputPage::SeedPhraseInputPage()
: InputPage()
{
	previousWord = "";
	predictedWord = "";
	setStringSize(MAXIMUM_WORD_LENGTH);
	setUsedCharSet(alphabetCharSet);
}

SeedPhraseInputPage::SeedPhraseInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: InputPage(inTitle, inParent)
{
	previousWord = "";
	predictedWord = "";
	setStringSize(MAXIMUM_WORD_LENGTH);
	setUsedCharSet(alphabetCharSet);
}

void SeedPhraseInputPage::updateSelectedOption(InputType input)
{
	InputPage::updateSelectedOption(input);
	if (selectedOptionIndex < 3)
	{
		predictedWord = "";
		return;
	}
	char currentWord[MAXIMUM_WORD_LENGTH] = { '\0' };
	uint8_t lastIndex = selectedOptionIndex + (inputString[selectedOptionIndex] == -1 ? 0 : 1);
	for (uint8_t i = 0; i < lastIndex; i++)
	{
		currentWord[i] = usedCharSet[inputString[i]];
	}
	const char* foundWord = nullptr;
	ContextData contextData = CoreSystem::getCoreSystem().getContextData();
	switch (contextData.mnemonicType)
	{
	case MnemonicType::LEGACY_MONERO:
		foundWord = get_monero_mnemonic_word_from_list(monero_mnemonic_find_word_index_allowing_partial_word(currentWord, MoneroEnglish, true), MoneroEnglish);
		break;
	case MnemonicType::BIP39:
	case MnemonicType::NONE:
	default:
		foundWord = mnemonic_complete_word(currentWord, lastIndex);
		break;
	}
	if (foundWord)
	{
		predictedWord = foundWord;
		for (int i = selectedOptionIndex+1; i < std::strlen(foundWord); i++)
			if (usedCharSet == alphabetCharSet)
				inputString[i] = foundWord[i]-0x61;
		for (int i = std::strlen(foundWord); i < MAXIMUM_WORD_LENGTH; i++)
			inputString[i] = -1;
	}
	if (!previousWord.empty())
		description = std::string("Enter word ") + std::to_string(contextData.getMnemonicWordCount()+1) + std::string(" of the phrase. Your previous word was ") + previousWord + std::string(".");
}

void SeedPhraseInputPage::draw(shared_ptr<IDisplay> display)
{
	InputPage::draw(display);
	drawPredictedWord(display);
}

shared_ptr<MenuTreeObject> SeedPhraseInputPage::getDestination()
{
	int32_t foundWordIndex = -1;
	char currentWord[MAXIMUM_WORD_LENGTH] = { '\0' };
	for (uint8_t i = 0; i < MAXIMUM_WORD_LENGTH; i++)
	{
		int32_t charIndex = inputString[i];
		if (charIndex < 0)
			break;
		currentWord[i] = usedCharSet[charIndex];
	}
	ContextData contextData = CoreSystem::getCoreSystem().getContextData();
	switch (contextData.mnemonicType)
	{
	case MnemonicType::LEGACY_MONERO:
		foundWordIndex = monero_mnemonic_find_word_index(currentWord, MoneroEnglish);
		if (foundWordIndex == -1)
		{
			description = "INVALID WORD ENETERED.";
			return nullptr;
		}
		if (contextData.getMnemonicWordCount() >= 1)
			contextData.mnemonic += " ";
		contextData.mnemonic += std::string(currentWord);
		setMnemonicContext(contextData.mnemonic);
		if (contextData.getMnemonicWordCount() == 25)
		{
			if (legacy_monero_mnemonic_check(contextData.mnemonic.c_str(), MoneroEnglish) > 0)
			{
				contextData.seedSize = 32;
				legacy_monero_mnemonic_to_seed(contextData.mnemonic.c_str(), contextData.seed, MoneroEnglish);
				CoreSystem::getCoreSystem().updateContextData(ContextUpdate::SEED | ContextUpdate::SEED_SIZE, contextData);
				return destination;
			}
			else
			{
				reset();
				description = "INVALID PHRASE ENTERED. CHECK IT THEN RE-ENTER.";
				return nullptr;
			}
		}
		break;
	case MnemonicType::BIP39:
	case MnemonicType::NONE:
	default:
		foundWordIndex = mnemonic_find_word(currentWord);
		if (foundWordIndex == -1)
		{
			description = "INVALID WORD ENETERED.";
			return nullptr;
		}
		if (contextData.getMnemonicWordCount() >= 1)
			contextData.mnemonic += " ";
		contextData.mnemonic += std::string(currentWord);
		setMnemonicContext(contextData.mnemonic);
		contextData = CoreSystem::getCoreSystem().getContextData();
		if (contextData.getMnemonicWordCount() == 24)
		{
			if(mnemonic_check(contextData.mnemonic.c_str()) > 0)
			{
				ContextData newContext;
				newContext.seedSize = 32;
				mnemonic_to_bits(contextData.mnemonic.c_str(), newContext.seed);
				CoreSystem::getCoreSystem().updateContextData(ContextUpdate::SEED | ContextUpdate::SEED_SIZE, newContext);
				return destination;
			}
			else
			{
				reset();
				description = "INVALID PHRASE ENTERED. CHECK IT THEN RE-ENTER.";
				return nullptr;
			}
		}
		break;
	}
	previousWord = currentWord;
	InputPage::reset();
	description = std::string("Enter word ") + std::to_string(contextData.getMnemonicWordCount()+1) + std::string(" of the phrase. Your previous word was ") + previousWord + std::string(".");
	predictedWord = "";
	return nullptr;
}

void SeedPhraseInputPage::reset()
{
	InputPage::reset();
	description = "Enter the first word of your seed phrase.";
	previousWord = "";
	predictedWord = "";
	clearMnemonicContext();
}

void SeedPhraseInputPage::onEnter()
{
	reset();
}

void SeedPhraseInputPage::drawPredictedWord(shared_ptr<IDisplay> display)
{
	if (predictedWord.empty())
		return;
	std::string predictedWordString = "Predicted Word: ";
	predictedWordString += predictedWord;
	TextBox predictedWordBox(predictedWordString);
	predictedWordBox.yPosition = BASE_BORDER_BOX_HEIGHT-6-4;
	predictedWordBox.xPosition = 2;
	predictedWordBox.width = BASE_BORDER_BOX_WIDTH-3;
	predictedWordBox.height = 5;
	predictedWordBox.setBordered();
	display->drawTextBox(predictedWordBox);
}
