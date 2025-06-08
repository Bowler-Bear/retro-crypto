#ifndef MENU_OPTION_H
#define MENU_OPTION_H

#include <memory>

#include "menu_tree_object.h"

using namespace std;

namespace RetroCrypto
{
	class Menu;
	class MenuOption
	{
	private:
		string label;

		string description;

		shared_ptr<MenuTreeObject> parent;

		shared_ptr<MenuTreeObject> destination;
	public:
		MenuOption(const shared_ptr<Menu> menu, const string& label, const string& description);
		void add(shared_ptr<MenuTreeObject> destination);
		string getLabel();
	};
}
#endif
