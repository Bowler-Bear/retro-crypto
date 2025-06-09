#ifndef MENU_H
#define MENU_H

#include "menu_tree_object.h"
#include "menu_option.h"

using namespace std;

namespace RetroCrypto
{
	class Menu : public MenuTreeObject
	{
	private:
		string title;
	public:
		void setTitle(string title);

		string getTitle();

		virtual void add(shared_ptr<MenuTreeObject> option) override;

		virtual void draw(shared_ptr<IDisplay> display, int optionIndex) override;

		void add(shared_ptr<MenuOption> option);
	};
}
#endif
