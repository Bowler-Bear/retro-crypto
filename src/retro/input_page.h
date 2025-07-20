#ifndef INPUT_PAGE_H
#define INPUT_PAGE_H

#include "page.h"

using namespace std;

namespace RetroCrypto
{
	class InputPage : public Page
	{
	protected:
		string description;
		vector<int32_t> inputString;
		uint8_t stringSize;
		shared_ptr<MenuTreeObject> destination;
		const char* usedCharSet;
	public:
		InputPage();
		InputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void updateSelectedOption(InputType input) override;
		virtual void draw(shared_ptr<IDisplay> display) override;
		void setStringSize(uint8_t size);
		void setUsedCharSet(const char* charSet);
		void setDescription(string newDescription);
		void setDestination(shared_ptr<MenuTreeObject> newDestination);
		virtual shared_ptr<MenuTreeObject> getDestination() override;
		virtual void onEnter() override;
		virtual void onForward() override;
		virtual void drawInput(shared_ptr<IDisplay> display);
		virtual void drawDescription(shared_ptr<IDisplay> display);
	};
}
#endif
