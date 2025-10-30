#include <cstring>

#include "seed_phrase_input_page.h"
#include "core_system.h"
#include "context_update_functions.h"

extern "C"
{
#include "bip39.h"
#include "legacy_monero_mnemonic.h"
#include "monero_chinese_words.h"
}

#define MAXIMUM_WORD_LENGTH MONERO_MAXIMUM_WORD_LENGTH

static const char* alphabetCharSet = "abcdefghijklmnopqrstuvwxyz";
static const char* germanCharSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWYZÄÖÜäöüß";
static const char* spanishCharSet = "abcdefghijklmnopqrstuvwxyzáíéóúñ";
static const char* russianCharSet = "абвьыъгдежзийклюмнопрстуфхцчшщэя";
static const char* japaneseCharSet = "あいぃえおうかがきぎけげこごくぐさざしじせぜそぞすずただちつづってでとどなにねのぬまみめもむらりれろるはばぱひびぴへべぺほぼぽふぶぷやゃゆゅよょんわ";
static char chineseCharSet[3*MONERO_WORDLIST_WORD_COUNT] = { '\0' };
static const char* lojbanCharSet = "abcdefghijklmnopqrstuvwxyz\'";

using namespace RetroCrypto;

int32_t getCharacterIndex(char utf8Character[4], const char* usedCharSet)
{
	uint32_t charactersToCompare = strlen(utf8Character);
	for (uint32_t i = 0; i < strlen(usedCharSet); i++)
	{
		for (uint32_t j = 0; j < charactersToCompare; j++)
		{
			if (i+j >= strlen(usedCharSet))
				break;
			if (usedCharSet[i+j] == utf8Character[j])
			{
				if (j == charactersToCompare-1)
					return i;
				continue;
			}
			break;
		}
	}
	return -1;
}

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

MoneroLanguage getMoneroLanguage(ContextData data)
{
	switch(data.mnemonicType)
	{
	default:
	case RetroCrypto::MnemonicType::LEGACY_MONERO_ENGLISH:
		return MoneroEnglish;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_GERMAN:
		return MoneroGerman;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_SPANISH:
		return MoneroSpanish;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_FRENCH:
		return MoneroFrench;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_ITALIAN:
		return MoneroItalian;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_DUTCH:
		return MoneroDutch;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_PORTUGUESE:
		return MoneroPortuguese;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_RUSSIAN:
		return MoneroRussian;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_JAPANESE:
		return MoneroJapanese;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_CHINESE:
		return MoneroChinese;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_ESPERANTO:
		return MoneroEsperanto;
	case RetroCrypto::MnemonicType::LEGACY_MONERO_LOJBAN:
		return MoneroLojban;
	}
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
	loadCurrentInputWord(currentWord);
	const char* foundWord = nullptr;
	ContextData contextData = CoreSystem::getCoreSystem().getContextData();
	switch (contextData.mnemonicType)
	{
	case MnemonicType::LEGACY_MONERO_ENGLISH:
	case MnemonicType::LEGACY_MONERO_GERMAN:
	case MnemonicType::LEGACY_MONERO_SPANISH:
	case MnemonicType::LEGACY_MONERO_FRENCH:
	case MnemonicType::LEGACY_MONERO_ITALIAN:
	case MnemonicType::LEGACY_MONERO_DUTCH:
	case MnemonicType::LEGACY_MONERO_PORTUGUESE:
	case MnemonicType::LEGACY_MONERO_RUSSIAN:
	case MnemonicType::LEGACY_MONERO_JAPANESE:
	case MnemonicType::LEGACY_MONERO_CHINESE:
	case MnemonicType::LEGACY_MONERO_ESPERANTO:
	case MnemonicType::LEGACY_MONERO_LOJBAN:
	{
		MoneroLanguage usedMoneroLanguage = getMoneroLanguage(contextData);
		foundWord = get_monero_mnemonic_word_from_list(monero_mnemonic_find_word_index_allowing_partial_word(currentWord, usedMoneroLanguage, true), usedMoneroLanguage);
		break;
	}
	case MnemonicType::BIP39:
		foundWord = mnemonic_complete_word(currentWord, selectedOptionIndex + (inputString[selectedOptionIndex] == -1 ? 0 : 1));
		break;
	case MnemonicType::NONE:
	default:
		break;
	}
	if (foundWord)
	{
		predictedWord = foundWord;
		uint8_t characters = 0;
		for (uint8_t i = 0; i < std::strlen(foundWord); i++)
		{
			if (usedCharSet == alphabetCharSet)
			{
				characters++;
				if (i < selectedOptionIndex+1)
					continue;
				inputString[i] = foundWord[i]-0x61;
				continue;
			}
			uint8_t codePoints = getCodePointCount(foundWord[i]);
			if (characters < selectedOptionIndex+1)
			{
				characters++;
				i += codePoints;
				continue;
			}
			else
			{
				char currentCharacter[4] = { '\0' };
				memcpy(currentCharacter, foundWord + i, codePoints+1);
				inputString[characters++] = getCharacterIndex(currentCharacter, usedCharSet);
				i += codePoints;
			}
		}
		for (size_t i = characters; i < stringSize; i++)
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
	return destination;
}

bool SeedPhraseInputPage::canConsumeAllInputs()
{
	return true;
}

bool SeedPhraseInputPage::consumeInput(InputType input)
{
	switch (input)
	{
	case InputType::FORWARD:
		{
		int32_t foundWordIndex = -1;
		char currentWord[MAXIMUM_WORD_LENGTH] = { '\0' };
		loadCurrentInputWord(currentWord, false);
		ContextData contextData = CoreSystem::getCoreSystem().getContextData();
		switch (contextData.mnemonicType)
		{
		case MnemonicType::LEGACY_MONERO_ENGLISH:
		case MnemonicType::LEGACY_MONERO_GERMAN:
		case MnemonicType::LEGACY_MONERO_SPANISH:
		case MnemonicType::LEGACY_MONERO_FRENCH:
		case MnemonicType::LEGACY_MONERO_ITALIAN:
		case MnemonicType::LEGACY_MONERO_DUTCH:
		case MnemonicType::LEGACY_MONERO_PORTUGUESE:
		case MnemonicType::LEGACY_MONERO_RUSSIAN:
		case MnemonicType::LEGACY_MONERO_JAPANESE:
		case MnemonicType::LEGACY_MONERO_CHINESE:
		case MnemonicType::LEGACY_MONERO_ESPERANTO:
		case MnemonicType::LEGACY_MONERO_LOJBAN:
		{
			MoneroLanguage usedMoneroLanguage = getMoneroLanguage(contextData);
			foundWordIndex = monero_mnemonic_find_word_index(currentWord, usedMoneroLanguage);
			if (foundWordIndex == -1)
			{
				description = "INVALID WORD ENTERED.";
				return true;
			}
			if (contextData.getMnemonicWordCount() >= 1)
				contextData.mnemonic += " ";
			contextData.mnemonic += std::string(currentWord);
			setMnemonicContext(contextData.mnemonic);
			contextData = CoreSystem::getCoreSystem().getContextData();
			if (contextData.getMnemonicWordCount() >= 25)
			{
				if (legacy_monero_mnemonic_check(contextData.mnemonic.c_str(), usedMoneroLanguage) > 0)
				{
					contextData.seedSize = 32;
					legacy_monero_mnemonic_to_seed(contextData.mnemonic.c_str(), contextData.seed, usedMoneroLanguage);
					CoreSystem::getCoreSystem().updateContextData(ContextUpdate::SEED | ContextUpdate::SEED_SIZE, contextData);
					return false;
				}
				else
				{
					reset();
					description = "INVALID PHRASE ENTERED. CHECK IT THEN RE-ENTER.";
					return true;
				}
			}
			break;
		}
		case MnemonicType::BIP39:
			foundWordIndex = mnemonic_find_word(currentWord);
			if (foundWordIndex == -1)
			{
				description = "INVALID WORD ENTERED.";
				return true;
			}
			if (contextData.getMnemonicWordCount() >= 1)
				contextData.mnemonic += " ";
			contextData.mnemonic += std::string(currentWord);
			setMnemonicContext(contextData.mnemonic);
			contextData = CoreSystem::getCoreSystem().getContextData();
			if (contextData.getMnemonicWordCount() >= 24)
			{
				if (mnemonic_check(contextData.mnemonic.c_str()) > 0)
				{
					contextData.seedSize = 32;
					mnemonic_to_bits(contextData.mnemonic.c_str(), contextData.seed);
					CoreSystem::getCoreSystem().updateContextData(ContextUpdate::SEED | ContextUpdate::SEED_SIZE, contextData);
					return false;
				}
				else
				{
					reset();
					description = "INVALID PHRASE ENTERED. CHECK IT THEN RE-ENTER.";
					return true;
				}
			}
			break;
		case MnemonicType::NONE:
		default:
			reset();
			description = std::string("ERROR: INVALID MNEMONIC TYPE");
			return true;
		}
		previousWord = currentWord;
		InputPage::reset();
		description = std::string("Enter word ") + std::to_string(contextData.getMnemonicWordCount()+1) + std::string(" of the phrase. Your previous word was '") + previousWord + std::string("'.");
		predictedWord = "";
		return true;
		}
		break;
	case InputType::BACK:
		{
		ContextData contextData = CoreSystem::getCoreSystem().getContextData();
		if (contextData.getMnemonicWordCount() >= 1)
		{
			if (contextData.getMnemonicWordCount() == 1)
			{
				contextData.mnemonic = "";
				previousWord = "";
				description = "Enter the first word of your seed phrase.";
			}
			else
			{
				contextData.mnemonic = contextData.mnemonic.substr(0, contextData.mnemonic.find_last_of(' '));
				previousWord = contextData.mnemonic.substr(contextData.mnemonic.find_last_of(' ')+1);
				description = std::string("Enter word ") + std::to_string(contextData.getMnemonicWordCount()+1) + std::string(" of the phrase. Your previous word was '") + previousWord + std::string("'.");
			}
			setMnemonicContext(contextData.mnemonic);
			InputPage::reset();
			predictedWord = "";
			return true;
		}
		return false;
		}
		break;
	default:
		break;
	}
	return true;
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
	switch (CoreSystem::getCoreSystem().getContextData().mnemonicType)
	{
	case MnemonicType::LEGACY_MONERO_GERMAN:
		setStringSize(8);
		setUsedCharSet(germanCharSet);
		setCharWidth(1);
		break;
	case MnemonicType::LEGACY_MONERO_SPANISH:
		setStringSize(8);
		setUsedCharSet(spanishCharSet);
		setCharWidth(1);
		break;
	case MnemonicType::LEGACY_MONERO_RUSSIAN:
		setStringSize(8);
		setUsedCharSet(russianCharSet);
		setCharWidth(1);
		break;
	case MnemonicType::LEGACY_MONERO_JAPANESE:
		setStringSize(7);
		setUsedCharSet(japaneseCharSet);
		setCharWidth(2);
		break;
	case MnemonicType::LEGACY_MONERO_CHINESE:
		setStringSize(1);
		setUsedCharSet(chineseCharSet);
		setCharWidth(2);
		for (uint32_t i = 0; i < MONERO_WORDLIST_WORD_COUNT; i++)
		{
			memcpy(&chineseCharSet[3*i], monero_chinese_words[i], 3);
		}
		break;
	case MnemonicType::LEGACY_MONERO_LOJBAN:
		setStringSize(13);
		setUsedCharSet(lojbanCharSet);
		setCharWidth(1);
		break;
	default:
		setStringSize(MAXIMUM_WORD_LENGTH);
		setUsedCharSet(alphabetCharSet);
		setCharWidth(1);
		break;
	}
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

void SeedPhraseInputPage::loadCurrentInputWord(char* currentWord, bool onlyUpToCursor)
{
	uint8_t lastIndex = max(0, selectedOptionIndex + (inputString[selectedOptionIndex] == -1 ? -1 : 0));
	if (!onlyUpToCursor)
	{
		for (uint8_t i = 0; i < inputString.size(); i++)
			if (inputString[i] == -1)
				break;
			else
				lastIndex = i;
	}
	for (uint8_t i = 0; i < lastIndex+1; i++)
	{
		uint8_t codePoints = getCodePointCount(usedCharSet[inputString[i]]);
		for (uint8_t j = 0; j <= codePoints; j++)
		{
			if (inputString[i]+j >= 0 && inputString[i]+j < (int32_t)strlen(usedCharSet))
				currentWord[strlen(currentWord)] = usedCharSet[inputString[i]+j];
		}
	}
}
