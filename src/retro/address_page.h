#ifndef ADDRESS_PAGE_H
#define ADDRESS_PAGE_H

#include "menu_tree_object.h"

#define PAGE_TITLE_BOX_Y_POSITION 2
#define PAGE_TITLE_BOX_HEIGHT 5

using namespace std;

namespace RetroCrypto
{
	class AddressPage : public MenuTreeObject
	{
	private:
		string title;
		string address;
		string addressType;
		string seed;

	public:
		AddressPage();
		AddressPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void add(std::shared_ptr<MenuTreeObject> child) override;
		virtual int getNewSelectedOption(int selectedOptionIndex, InputType input) override;
		virtual void draw(shared_ptr<IDisplay> display, int selectedOptionIndex) override;
		virtual shared_ptr<MenuTreeObject> getDestination(int selectedOptionIndex) override;
		virtual void onEnter() override;
		virtual void onExit() override;
		void onBackward(int selectedOptionIndex);
		void drawTitle(shared_ptr<IDisplay> display);
		void drawSeed(shared_ptr<IDisplay> display);
		void drawAddress(shared_ptr<IDisplay> display);
	};
}
#endif
