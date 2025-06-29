#ifndef MENU_H
#define MENU_H

#include "menu_tree_object.h"
#include "menu_option.h"

#define MENU_BOX_X_POSITION BASE_BORDER_BOX_X_POSITION
#define MENU_BOX_Y_POSITION BASE_BORDER_BOX_Y_POSITION
#define MENU_BOX_WIDTH BASE_BORDER_BOX_WIDTH
#define MENU_BOX_HEIGHT BASE_BORDER_BOX_HEIGHT
#define MENU_TITLE_BOX_Y_POSITION 3
#define MENU_TITLE_BOX_HEIGHT 3

using namespace std;

namespace RetroCrypto
{
	class Menu : public MenuTreeObject
	{
	private:
		string title;
	public:
		Menu();
		Menu(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		void setTitle(string title);

		string getTitle();

		virtual void add(shared_ptr<MenuTreeObject> option) override;

		virtual void drawTitle(shared_ptr<IDisplay> display);

		virtual void drawOptions(shared_ptr<IDisplay> display);

		virtual void drawDescription(shared_ptr<IDisplay> display);

		virtual void draw(shared_ptr<IDisplay> display) override;

		virtual std::shared_ptr<MenuTreeObject> getDestination() override;

		virtual void updateSelectedOption(InputType input);

		virtual void onForward() override;

		void add(shared_ptr<MenuOption> option);
	};
}
#endif
