#include <cstring>

#include "core_system.h"
#include "encryption_page.h"

extern "C"
{
#include "aes/aes.h"
#include "memzero.h"
}

using namespace RetroCrypto;

EncryptionPage::EncryptionPage()
: InputPage()
{
	willEncrypt = true;
	reset();
}

EncryptionPage::EncryptionPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: InputPage(inTitle, inParent)
{
	willEncrypt = true;
	reset();
}

EncryptionPage::~EncryptionPage()
{
	reset();
}

bool EncryptionPage::canConsumeAllInputs()
{
	return true;
}

bool EncryptionPage::consumeInput(InputType input)
{
	switch (currentState)
	{
	default:
	case INPUT_DATA_SIZE:
		if (input == FORWARD)
		{
			try
			{
				if (!reallocateInputData())
				{
					setDescription("Failed to allocate memory for input data.");
					return true;
				}
			}
			catch(std::string s)
			{
				setDescription(s);
				return true;
			}
			setCurrentState(INPUT_DATA);
			return true;
		}
		return false;
	case INPUT_DATA:
		if (input == BACK)
		{
			freeInputData();
			setCurrentState(INPUT_DATA_SIZE);
			return true;
		}
		if (input == FORWARD)
		{
			setCurrentState(INPUT_KEY_SIZE);
			return true;
		}
		return false;
	case INPUT_KEY_SIZE:
		if (input == BACK)
		{
			setCurrentState(INPUT_DATA);
			return true;
		}
		if (input == FORWARD)
		{
			try
			{
				if (!reallocateInputKey())
				{
					setDescription("Failed to allocate memory for key.");
					return true;
				}
			}
			catch(std::string s)
			{
				setDescription(s);
				return true;
			}
			setCurrentState(INPUT_KEY);
			return true;
		}
		return false;
	case INPUT_KEY:
		if (input == BACK)
		{
			freeInputKey();
			setCurrentState(INPUT_KEY_SIZE);
			return true;
		}
		if (input == FORWARD)
		{
			try
			{
				if (!reallocateOutputData())
				{
					setDescription("Failed to allocate memory for output data.");
					return true;
				}
			}
			catch(std::string s)
			{
				setDescription(s);
				return true;
			}
			setCurrentState(PROCESSING);
			return true;
		}
		return false;
	case PROCESSING:
		if (input == BACK)
		{
			freeOutputData();
			setCurrentState(INPUT_KEY);
			return true;
		}
		if (input == FORWARD)
		{
			return true;
		}
		return false;
	case OUTPUT_DATA:
		if (input == BACK)
		{
			freeOutputData();
			setCurrentState(INPUT_KEY);
			return true;
		}
		return false;
	}
	return true;
}

void EncryptionPage::updateSelectedOption(InputType input)
{
	uint32_t maxIndex = 0;
	uint8_t* data = nullptr;
	uint32_t* sizePointer = nullptr;
	switch (currentState)
	{
	default:
	case INPUT_DATA_SIZE:
		sizePointer = &inputDataSize;
		break;
	case INPUT_DATA:
		maxIndex = (EP_CHARACTERS_PER_BYTE * inputDataSize) - 1;
		data = inputData;
		break;
	case INPUT_KEY_SIZE:
		sizePointer = &inputKeySize;
		break;
	case INPUT_KEY:
		maxIndex = (EP_CHARACTERS_PER_BYTE * inputKeySize) - 1;
		data = inputKey;
		break;
	case PROCESSING:
		break;
	case OUTPUT_DATA:
		maxIndex = (EP_CHARACTERS_PER_BYTE * inputDataSize) - 1;
		data = outputData;
		break;
	}

	switch (currentState)
	{
	case INPUT_DATA:
	case INPUT_KEY:
	case OUTPUT_DATA:
		if (modifiedDataIndex > maxIndex)
		{
			modifiedDataIndex = 0;
			return;
		}
		switch (input)
		{
		case InputType::UP:
			if (currentState != OUTPUT_DATA)
			{
				if (modifiedDataIndex % 2 == 0)
					data[modifiedDataIndex / 2] += (1 << 4);
				else
					data[modifiedDataIndex / 2] += 1;
			}
			break;
		case InputType::DOWN:
			if (currentState != OUTPUT_DATA)
			{
				if (modifiedDataIndex % 2 == 0)
					data[modifiedDataIndex / 2] -= (1 << 4);
				else
					data[modifiedDataIndex / 2] -= 1;
			}
			break;
		case InputType::LEFT:
			if (modifiedDataIndex == 0)
				modifiedDataIndex = maxIndex;
			else
				modifiedDataIndex -= 1;
			break;
		case InputType::RIGHT:
			if (modifiedDataIndex == maxIndex)
				modifiedDataIndex = 0;
			else
				modifiedDataIndex += 1;
			break;
		default:
			break;
		}
		break;
	default:
	case INPUT_DATA_SIZE:
		if (!sizePointer)
		{
			return;
		}

		switch (input)
		{
		case InputType::UP:
			*sizePointer += AES_BLOCK_SIZE;
			break;
		case InputType::DOWN:
			if (*sizePointer > AES_BLOCK_SIZE)
			{
				*sizePointer -= AES_BLOCK_SIZE;
			}
			break;
		default:
			break;
		}
		break;
	case INPUT_KEY_SIZE:
		//Key size is limited to 256 bits for now
	case PROCESSING:
		break;
	}
}

shared_ptr<MenuTreeObject> EncryptionPage::getDestination()
{
	return parent;
}

void EncryptionPage::reset()
{
	setCurrentState(INPUT_DATA_SIZE);
	freeInputData();
	inputDataSize = EP_DEFAULT_DATA_SIZE;
	freeInputKey();
	inputKeySize = EP_DEFAULT_KEY_SIZE;
	freeOutputData();
}

void EncryptionPage::onEnter()
{
	reset();
}

void EncryptionPage::onExit()
{
	reset();
}

void EncryptionPage::tick()
{
	if (currentState == PROCESSING)
	{
		if (willEncrypt)
		{
			aes_encrypt_ctx context;
			memzero(&context, sizeof(aes_encrypt_ctx));
			aes_encrypt_key256(inputKey, &context);
			if(aes_ecb_encrypt(inputData, outputData, inputDataSize, &context) != EXIT_SUCCESS)
			{
				setCurrentState(INPUT_KEY);
				return;
			}
			memzero(&context, sizeof(aes_encrypt_ctx));
		}
		else
		{
			aes_decrypt_ctx context;
			memzero(&context, sizeof(aes_decrypt_ctx));
			aes_decrypt_key256(inputKey, &context);
			if(aes_ecb_decrypt(inputData, outputData, inputDataSize, &context) != EXIT_SUCCESS)
			{
				setCurrentState(INPUT_KEY);
				return;
			}
			memzero(&context, sizeof(aes_decrypt_ctx));
		}
		setCurrentState(OUTPUT_DATA);
	}
}

void EncryptionPage::drawInput(shared_ptr<IDisplay> display)
{
	switch (currentState)
	{
	default:
	case INPUT_DATA_SIZE:
		drawSizeInput(display, &inputDataSize);
		break;
	case INPUT_DATA:
		drawDataInput(display, inputData, inputDataSize);
		break;
	case INPUT_KEY_SIZE:
		drawSizeInput(display, &inputKeySize);
		break;
	case INPUT_KEY:
		drawDataInput(display, inputKey, inputKeySize);
		break;
	case PROCESSING:
		break;
	case OUTPUT_DATA:
		drawDataInput(display, outputData, inputDataSize);
		break;
	}
}

void EncryptionPage::drawDataInput(shared_ptr<IDisplay> display, uint8_t* data, uint32_t dataSize)
{
	TextBox inputBox;
	inputBox.text.assign(EP_BYTE_SCALING*EP_BYTES_PER_LINE+11, ' ');
	inputBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+7;
	inputBox.xPosition = 1;
	inputBox.width = BASE_BORDER_BOX_WIDTH-3;
	inputBox.height = 1;

	if (!data)
	{
		inputBox.text = "Data block not allocated.";
		display->drawTextBox(inputBox);
		return;
	}

	char temp[11] = { 0 };
	for (uint8_t j = 0; j < EP_BYTES_PER_LINE; j++)
	{
		std::sprintf(temp, "%02x ", j);
		inputBox.text.replace(11+EP_BYTE_SCALING*j, 3, temp, 3);
	}
	inputBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+7;
	display->drawTextBox(inputBox);
	inputBox.yPosition += 2;

	const uint32_t totalLineCount = ((dataSize-1) / EP_BYTES_PER_LINE)+1;
	const uint32_t currentLine = modifiedDataIndex/(EP_CHARACTERS_PER_BYTE*EP_BYTES_PER_LINE);
	const uint32_t startIndex = min(max((int32_t)0, (int32_t)currentLine-EP_VISIBLE_LINES/2), max((int32_t)0, (int32_t)totalLineCount-EP_VISIBLE_LINES));
	const uint32_t endIndex = startIndex+(totalLineCount < EP_VISIBLE_LINES ? totalLineCount : EP_VISIBLE_LINES);
	for(uint32_t i = startIndex; i < endIndex; i++)
	{
		inputBox.text.replace(0, string::npos, EP_BYTE_SCALING*EP_BYTES_PER_LINE+11, ' ');
		std::sprintf(temp, "0x%08lx", (long unsigned int)i*EP_BYTES_PER_LINE);
		inputBox.text.replace(0, 10, temp, 10);

		const uint32_t lineEnd = (i == totalLineCount-1 ? dataSize-(EP_BYTES_PER_LINE*(totalLineCount-1)) : EP_BYTES_PER_LINE);
		for (uint8_t j = 0; j < lineEnd; j++)
		{
			std::sprintf(temp, "%02X ", data[EP_BYTES_PER_LINE*i+j]);
			inputBox.text.replace(11+EP_BYTE_SCALING*j, 3, temp, 3);
		}
		inputBox.text = inputBox.text;
		display->drawTextBox(inputBox);
		inputBox.yPosition += 2;

		inputBox.text.replace(0, string::npos, EP_BYTE_SCALING*EP_BYTES_PER_LINE+11, ' ');
		if (i == currentLine)
		{
			TextBox asterisk(std::string(EP_BYTE_SCALING*EP_BYTES_PER_LINE+11, ' '));
			asterisk.yPosition = inputBox.yPosition-1;
			asterisk.xPosition = 1;
			asterisk.width = BASE_BORDER_BOX_WIDTH-3;
			asterisk.height = 1;
			asterisk.text[11+(modifiedDataIndex % (EP_CHARACTERS_PER_BYTE*EP_BYTES_PER_LINE))+((modifiedDataIndex % (EP_CHARACTERS_PER_BYTE*EP_BYTES_PER_LINE)) / EP_CHARACTERS_PER_BYTE)] = '*';
			display->drawTextBox(asterisk);
			asterisk.yPosition -= 2;
			display->drawTextBox(asterisk);
		}
	}
}

void EncryptionPage::drawSizeInput(shared_ptr<IDisplay> display, uint32_t* size)
{
	if (size == nullptr)
		return;
	char stringBuffer[13+2*10] = { 0 };
	//The format has to use 'lu' to work with the n64 as the underlying data type for uint32_t is long unsigned int
	//These are cast to long unsigned int to suppress a build warning for the cli build due to the format string using 'ul' despite being provided an unsigned int
	std::sprintf(stringBuffer, "%lu bytes(%lu bits)", (long unsigned int)*size, (long unsigned int)(8*(*size)));
	TextBox inputBox(stringBuffer);
	inputBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8;
	inputBox.xPosition = 1;
	inputBox.width = BASE_BORDER_BOX_WIDTH-3;
	inputBox.height = 1;
	display->drawTextBox(inputBox);
}

void EncryptionPage::setCurrentState(EncryptionState newState)
{
	currentState = newState;
	updateTitle();
	clearDescription();
	modifiedDataIndex = 0;
}

void EncryptionPage::updateTitle()
{
	switch (currentState)
	{
	case INPUT_DATA_SIZE:
		title = "Enter Data Size";
		break;
	case INPUT_DATA:
		title = willEncrypt ? "Enter Plaintext Data" : "Enter Ciphertext Data";
		break;
	case INPUT_KEY:
		title = "Enter Key Size";
		break;
	case INPUT_KEY_SIZE:
		title = "Enter Key Data";
		break;
	case PROCESSING:
		title = "Processing Data";
		break;
	case OUTPUT_DATA:
		title =  willEncrypt ? "Ciphertext Data" : "Plaintext Data";
		break;
	default:
		title = "Unknown State";
		break;
	}
}

void EncryptionPage::clearDescription()
{
	setDescription("");
}

void EncryptionPage::setWillDecrypt()
{
	willEncrypt = false;
}

void EncryptionPage::freePointer(uint8_t** pointer, uint32_t size)
{
	if (*pointer != nullptr)
	{
		memzero(*pointer, size);
		free(*pointer);
		*pointer = nullptr;
	}
}

void EncryptionPage::freeInputData()
{
	freePointer(&inputData, inputDataSize);
}

void EncryptionPage::freeInputKey()
{
	freePointer(&inputKey, inputKeySize);
}

void EncryptionPage::freeOutputData()
{
	freePointer(&outputData, inputDataSize);
}

bool EncryptionPage::reallocatePointer(uint8_t** pointer, uint32_t size)
{
	if (size <= 0)
	{
		return false;
	}
	*pointer = (uint8_t*)malloc(size);
	if (*pointer != nullptr)
	{
		memzero(*pointer, size);
	}
	return *pointer != nullptr;
}

bool EncryptionPage::reallocateInputData()
{
	freeInputData();
	return reallocatePointer(&inputData, inputDataSize);
}

bool EncryptionPage::reallocateInputKey()
{
	freeInputKey();
	return reallocatePointer(&inputKey, inputKeySize);
}

bool EncryptionPage::reallocateOutputData()
{
	freeOutputData();
	return reallocatePointer(&outputData, inputDataSize);
}
