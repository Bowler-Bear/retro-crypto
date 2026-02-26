#ifndef ENCRYPTION_PAGE_H
#define ENCRYPTION_PAGE_H

#define EP_VISIBLE_LINES 8
#define EP_BYTES_PER_LINE 16
#define EP_CHARACTERS_PER_BYTE 2
#define EP_SPACE_AFTER_BYTE 1
#define EP_BYTE_SCALING (EP_CHARACTERS_PER_BYTE + EP_SPACE_AFTER_BYTE)
#define EP_DEFAULT_DATA_SIZE AES_BLOCK_SIZE
#define EP_DEFAULT_KEY_SIZE 256 / 8

#include "input_page.h"

using namespace std;

namespace RetroCrypto
{
	enum EncryptionState
	{
		INPUT_DATA_SIZE,
		INPUT_DATA,
		INPUT_KEY_SIZE,
		INPUT_KEY,
		PROCESSING,
		OUTPUT_DATA
	};

	class EncryptionPage : public InputPage
	{
	private:
		bool willEncrypt;
		EncryptionState currentState;
		uint32_t modifiedDataIndex;
		uint8_t* inputData;
		uint32_t inputDataSize;
		uint8_t* inputKey;
		uint32_t inputKeySize;
		uint8_t* outputData;

		void freePointer(uint8_t** pointer, uint32_t size);
		void freeInputData();
		void freeInputKey();
		void freeOutputData();
		bool reallocatePointer(uint8_t** pointer, uint32_t size);
		bool reallocateInputData();
		bool reallocateInputKey();
		bool reallocateOutputData();

	public:
		EncryptionPage();
		EncryptionPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		~EncryptionPage();
		virtual bool canConsumeAllInputs() override;
		virtual bool consumeInput(InputType input) override;
		virtual void updateSelectedOption(InputType input) override;
		virtual shared_ptr<MenuTreeObject> getDestination() override;
		virtual void reset() override;
		virtual void onEnter() override;
		virtual void onExit() override;
		virtual void tick() override;
		virtual void drawInput(shared_ptr<IDisplay> display) override;
		void drawDataInput(shared_ptr<IDisplay> display, uint8_t* data = nullptr, uint32_t dataSize = 0);
		void drawSizeInput(shared_ptr<IDisplay> display, uint32_t* size = nullptr);
		void setCurrentState(EncryptionState newState);
		void updateTitle();
		void clearDescription();
		void stateUpdate();
		void setWillDecrypt();
	};
}
#endif
