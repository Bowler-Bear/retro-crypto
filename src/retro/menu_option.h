#ifndef MENU_OPTION_H
#define MENU_OPTION_H

#include <memory>

#include "menu_tree_object.h"

using namespace std;

namespace RetroCrypto
{
	class Menu;
	class MenuOption : public MenuTreeObject
	{
	private:
		string label;

		string description;

		shared_ptr<MenuTreeObject> destination;
	public:
		MenuOption(const shared_ptr<Menu> menu, const string& label, const string& description);
		virtual void add(shared_ptr<MenuTreeObject> destination) override;
	};
}
#endif
