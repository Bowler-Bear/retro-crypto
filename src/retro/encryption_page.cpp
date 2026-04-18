#include <cstring>

#include "core_system.h"
#include "encryption_page.h"
#include "menu.h"

extern "C"
{
#include "aes/aes.h"
#include "memzero.h"
}

typedef AES_RETURN (*EncryptionFunction)(const unsigned char*, unsigned char*, int, unsigned char*, aes_encrypt_ctx*);

AES_RETURN aes_ecb_encrypt_wrapper(const unsigned char *ibuf, unsigned char *obuf, int len, unsigned char* iv, aes_encrypt_ctx cx[1])
{
	return aes_ecb_encrypt(ibuf, obuf, len, cx);
}

AES_RETURN aes_cbc_encrypt_wrapper(const unsigned char *ibuf, unsigned char *obuf, int len, unsigned char* iv, aes_encrypt_ctx cx[1])
{
	return aes_cbc_encrypt(ibuf, obuf, len, iv, cx);
}

class MergedDecryptionContext
{
public:
	aes_decrypt_ctx* decryptContext;
	aes_encrypt_ctx* encryptContext;

public:
	MergedDecryptionContext()
	{
		decryptContext = nullptr;
		encryptContext = nullptr;
	}

	~MergedDecryptionContext()
	{
		if (decryptContext != nullptr)
		{
			memzero(decryptContext, sizeof(aes_decrypt_ctx));
			delete decryptContext;
		}
		if (encryptContext != nullptr)
		{
			memzero(encryptContext, sizeof(aes_encrypt_ctx));
			delete encryptContext;
		}
	}

	void useDecryptContext(const unsigned char* inputKey)
	{
		if (decryptContext == nullptr)
		{
			decryptContext = new aes_decrypt_ctx();
			memzero(decryptContext, sizeof(aes_decrypt_ctx));
			aes_decrypt_key256(inputKey, decryptContext);
		}
	}

	void useEncryptContext(const unsigned char* inputKey)
	{
		if (encryptContext == nullptr)
		{
			encryptContext = new aes_encrypt_ctx();
			memzero(encryptContext, sizeof(aes_encrypt_ctx));
			aes_encrypt_key256(inputKey, encryptContext);
		}
	}
};

typedef AES_RETURN (*DecryptionFunction)(const unsigned char*, unsigned char*, int, unsigned char*, MergedDecryptionContext*);

AES_RETURN aes_ecb_decrypt_wrapper(const unsigned char *ibuf, unsigned char *obuf, int len, unsigned char* iv, MergedDecryptionContext cx[1])
{
	return aes_ecb_decrypt(ibuf, obuf, len, cx->decryptContext);
}

AES_RETURN aes_cbc_decrypt_wrapper(const unsigned char *ibuf, unsigned char *obuf, int len, unsigned char* iv, MergedDecryptionContext cx[1])
{
	return aes_cbc_decrypt(ibuf, obuf, len, iv, cx->decryptContext);
}

AES_RETURN aes_cfb_decrypt_wrapper(const unsigned char *ibuf, unsigned char *obuf, int len, unsigned char* iv, MergedDecryptionContext cx[1])
{
	return aes_cfb_decrypt(ibuf, obuf, len, iv, cx->encryptContext);
}

AES_RETURN aes_ofb_decrypt_wrapper(const unsigned char *ibuf, unsigned char *obuf, int len, unsigned char* iv, MergedDecryptionContext cx[1])
{
	return aes_ofb_decrypt(ibuf, obuf, len, iv, cx->encryptContext);
}

using namespace RetroCrypto;

string encryptionModeToTitle(EncryptionMode mode)
{
	switch (mode)
	{
	case AES_256_EBC:
		return "AES-256 EBC";
	case AES_256_CBC:
		return "AES-256 CBC";
	case AES_256_CFB:
		return "AES-256 CFB";
	case AES_256_OFB:
		return "AES-256 OFB";
	default:
		return "Unkown";
	}
}

uint32_t encryptionModeIncrementSize(EncryptionMode mode)
{
	switch (mode)
	{
	default:
	case AES_256_EBC:
	case AES_256_CBC:
		return AES_BLOCK_SIZE;
	case AES_256_CFB:
	case AES_256_OFB:
		return 1;
	}
}

EncryptionPage::EncryptionPage()
: InputPage()
{
	willEncrypt = true;
	reset();
	modeSelectionMenu = make_shared<Menu>("Select Mode", nullptr);
	for (uint8_t i = 0; i < EncryptionMode::END_INDEX; i++)
	{
		shared_ptr<MenuOption> option = make_shared<MenuOption>(modeSelectionMenu, encryptionModeToTitle((EncryptionMode)i), encryptionModeToTitle((EncryptionMode)i)+" mode.");
		modeSelectionMenu->addOption(option);
	}
}

EncryptionPage::EncryptionPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: InputPage(inTitle, inParent)
{
	willEncrypt = true;
	reset();
	modeSelectionMenu = make_shared<Menu>("Select Mode", nullptr);
	for (uint8_t i = 0; i < EncryptionMode::END_INDEX; i++)
	{
		shared_ptr<MenuOption> option = make_shared<MenuOption>(modeSelectionMenu, encryptionModeToTitle((EncryptionMode)i), encryptionModeToTitle((EncryptionMode)i)+" mode.");
		modeSelectionMenu->addOption(option);
	}
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
	case SELECT_MODE:
		if (input == FORWARD)
		{
			currentMode = (EncryptionMode)modeSelectionMenu->getSelectedOption();
			setCurrentState(INPUT_DATA_SIZE);
			return true;
		}
		return false;
	case INPUT_DATA_SIZE:
		if (input == BACK)
		{
			setCurrentState(SELECT_MODE);
			return true;
		}
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
			switch (currentMode)
			{
			case AES_256_EBC:
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
				break;
			default:
			case AES_256_CBC:
			case AES_256_CFB:
			case AES_256_OFB:
				try
				{
					if (!reallocateInitializationVector())
					{
						setDescription("Failed to allocate memory for initialization vector.");
						return true;
					}
				}
				catch(std::string s)
				{
					setDescription(s);
					return true;
				}
				setCurrentState(INPUT_IV);
				break;
			}
			return true;
		}
		return false;
	case INPUT_IV:
		if (input == BACK)
		{
			freeInitializationVector();
			setCurrentState(INPUT_KEY);
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
			switch (currentMode)
			{
			default:
			case AES_256_EBC:
				setCurrentState(INPUT_KEY);
				break;
			case AES_256_CBC:
			case AES_256_CFB:
			case AES_256_OFB:
				setCurrentState(INPUT_IV);
				break;
			}
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
			switch (currentMode)
			{
			default:
			case AES_256_EBC:
				setCurrentState(INPUT_KEY);
				break;
			case AES_256_CBC:
			case AES_256_CFB:
			case AES_256_OFB:
				setCurrentState(INPUT_IV);
				break;
			}
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
	uint32_t sizeIncrement = encryptionModeIncrementSize(currentMode);
	switch (currentState)
	{
	default:
	case SELECT_MODE:
		break;
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
	case INPUT_IV:
		maxIndex = (EP_CHARACTERS_PER_BYTE * EP_INITIALIZATION_VECTOR) - 1;
		data = initializationVector;
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
	default:
	case SELECT_MODE:
		switch (input)
		{
		case InputType::UP:
		case InputType::DOWN:
			modeSelectionMenu->updateSelectedOption(input);
		default:
			break;
		}
		break;
	case INPUT_DATA:
	case INPUT_KEY:
	case INPUT_IV:
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
	case INPUT_DATA_SIZE:
		if (!sizePointer)
		{
			return;
		}

		switch (input)
		{
		case InputType::UP:
			*sizePointer += sizeIncrement;
			break;
		case InputType::DOWN:
			if (*sizePointer > sizeIncrement)
			{
				*sizePointer -= sizeIncrement;
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
	currentMode = AES_256_EBC;
	setCurrentState(SELECT_MODE);
	freeInputData();
	inputDataSize = EP_DEFAULT_DATA_SIZE;
	freeInputKey();
	inputKeySize = EP_DEFAULT_KEY_SIZE;
	freeInitializationVector();
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
			aes_encrypt_ctx* context = new aes_encrypt_ctx();
			if (!context)
			{
				setCurrentState(INPUT_KEY);
				setDescription("Failed to create encryption context.");
				return;
			}
			memzero(context, sizeof(aes_encrypt_ctx));
			aes_encrypt_key256(inputKey, context);

			EncryptionFunction encryptionFunction = nullptr;
			uint8_t* duplicateInitializationVector = nullptr;
			EncryptionState failureReturnState = INPUT_IV;

			switch (currentMode)
			{
			case AES_256_EBC:
				encryptionFunction = aes_ecb_encrypt_wrapper;
				failureReturnState = INPUT_KEY;
				break;
			case AES_256_CBC:
				encryptionFunction = aes_cbc_encrypt_wrapper;
				break;
			case AES_256_CFB:
				encryptionFunction = aes_cfb_encrypt;
				break;
			case AES_256_OFB:
				encryptionFunction = aes_ofb_encrypt;
				break;
			default:
				setCurrentState(failureReturnState);
				setDescription("Unknown Encryption Mode");
				memzero(context, sizeof(aes_encrypt_ctx));
				delete context;
				return;
			}
			if (encryptionFunction == nullptr)
			{
				setCurrentState(failureReturnState);
				setDescription("Encryption function was not properly assigned prior based on mode.");
				return;
			}
			if (failureReturnState == INPUT_IV)
			{
				duplicateInitializationVector = (uint8_t*)malloc(EP_INITIALIZATION_VECTOR);
				memcpy(duplicateInitializationVector, initializationVector, EP_INITIALIZATION_VECTOR);
			}
			if (encryptionFunction(inputData, outputData, inputDataSize, duplicateInitializationVector, context) != EXIT_SUCCESS)
			{
				setCurrentState(failureReturnState);
				if (duplicateInitializationVector != nullptr)
				{
					free(duplicateInitializationVector);
				}
				memzero(context, sizeof(aes_encrypt_ctx));
				delete context;
				setDescription("Encryption function failed.");
				return;
			}
			if (duplicateInitializationVector != nullptr)
			{
				free(duplicateInitializationVector);
			}
			memzero(context, sizeof(aes_encrypt_ctx));
			delete context;
		}
		else
		{
			MergedDecryptionContext* context = new MergedDecryptionContext();

			DecryptionFunction decryptionFunction = nullptr;
			uint8_t* duplicateInitializationVector = nullptr;
			EncryptionState failureReturnState = INPUT_IV;

			switch (currentMode)
			{
			case AES_256_EBC:
				decryptionFunction = aes_ecb_decrypt_wrapper;
				failureReturnState = INPUT_KEY;
				context->useDecryptContext(inputKey);
				break;
			case AES_256_CBC:
				decryptionFunction = aes_cbc_decrypt_wrapper;
				context->useDecryptContext(inputKey);
				break;
			case AES_256_CFB:
				decryptionFunction = aes_cfb_decrypt_wrapper;
				context->useEncryptContext(inputKey);
				break;
			case AES_256_OFB:
				decryptionFunction = aes_ofb_decrypt_wrapper;
				context->useEncryptContext(inputKey);
				break;
			default:
				delete context;
				setCurrentState(INPUT_IV);
				setDescription("Unknown Decryption Mode");
				return;
			}
			if (decryptionFunction == nullptr)
			{
				setCurrentState(failureReturnState);
				setDescription("Decryption function was not properly assigned prior based on mode.");
				return;
			}
			if (failureReturnState == INPUT_IV)
			{
				duplicateInitializationVector = (uint8_t*)malloc(EP_INITIALIZATION_VECTOR);
				memcpy(duplicateInitializationVector, initializationVector, EP_INITIALIZATION_VECTOR);
			}
			if (decryptionFunction(inputData, outputData, inputDataSize, duplicateInitializationVector, context) != EXIT_SUCCESS)
			{
				setCurrentState(failureReturnState);
				if (duplicateInitializationVector != nullptr)
				{
					free(duplicateInitializationVector);
				}
				delete context;
				setDescription("Decryption function failed.");
				return;
			}
			if (duplicateInitializationVector != nullptr)
			{
				free(duplicateInitializationVector);
			}
			delete context;
		}
		setCurrentState(OUTPUT_DATA);
	}
}

void EncryptionPage::drawInput(shared_ptr<IDisplay> display)
{
	switch (currentState)
	{
	default:
	case SELECT_MODE:
		drawModeSelect(display);
		break;
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
	case INPUT_IV:
		drawDataInput(display, initializationVector, EP_INITIALIZATION_VECTOR);
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

void EncryptionPage::drawTitle(shared_ptr<IDisplay> display)
{
	if (currentState != SELECT_MODE)
	{
		Page::drawTitle(display);
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

void EncryptionPage::drawModeSelect(shared_ptr<IDisplay> display)
{
	modeSelectionMenu->draw(display);
}

void EncryptionPage::setCurrentState(EncryptionState newState)
{
	currentState = newState;
	updateTitle();
	clearDescription();
	modifiedDataIndex = 0;
	if (currentState == SELECT_MODE)
	{
		inputDataSize = EP_DEFAULT_DATA_SIZE;
	}
}

void EncryptionPage::updateTitle()
{
	switch (currentState)
	{
	case SELECT_MODE:
		title = "Select Mode";
		break;
	case INPUT_DATA_SIZE:
		title = "Enter Data Size";
		break;
	case INPUT_DATA:
		title = encryptionModeToTitle(currentMode) + (willEncrypt ? " Plaintext Data" : " Ciphertext Data");
		break;
	case INPUT_KEY:
		title = "Enter Key Size";
		break;
	case INPUT_KEY_SIZE:
		title = "Enter Key Data";
		break;
	case INPUT_IV:
		title = "Enter Initialization Vector";
		break;
	case PROCESSING:
		title = "Processing Data";
		break;
	case OUTPUT_DATA:
		title = encryptionModeToTitle(currentMode) + (willEncrypt ? " Ciphertext Data" : " Plaintext Data");
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

void EncryptionPage::freeInitializationVector()
{
	freePointer(&initializationVector, EP_INITIALIZATION_VECTOR);
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

bool EncryptionPage::reallocateInitializationVector()
{
	freeInitializationVector();
	return reallocatePointer(&initializationVector, EP_INITIALIZATION_VECTOR);
}

bool EncryptionPage::reallocateOutputData()
{
	freeOutputData();
	return reallocatePointer(&outputData, inputDataSize);
}
