#ifndef MENU_TREE_SYSTEM_H
#define MENU_TREE_SYSTEM_H

#include <memory>
#include "menu.h"

using namespace std;

namespace RetroCrypto
{
	class MenuTree
	{
	private:
		shared_ptr<MenuTreeObject> root;
		bool initialized;
	public:
		MenuTree()
		{
			root = nullptr;
			initialized = false;
		}

		void init()
		{
			if (initialized)
				return;
			shared_ptr<Menu> mainMenu = make_shared<Menu>();
			root = static_pointer_cast<MenuTreeObject>(mainMenu);
			mainMenu->setTitle("Main Menu");
			shared_ptr<MenuOption> generateSeedOption = make_shared<MenuOption>(mainMenu, "Generate Seed", "Generate a random seed.");
			shared_ptr<MenuOption> generateAddressOption = make_shared<MenuOption>(mainMenu, "Generate Address", "Generate an address.");
			initialized = true;
		}

		shared_ptr<MenuTreeObject> getRoot()
		{
			return root;
		}
	};
}
#endif
