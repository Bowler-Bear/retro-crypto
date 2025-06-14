#include "menu_tree.h"
#include "menu.h"

using namespace RetroCrypto;

MenuTree::MenuTree()
{
	shared_ptr<Menu> mainMenu = make_shared<Menu>("Main Menu");
	root = static_pointer_cast<MenuTreeObject>(mainMenu);

	shared_ptr<MenuOption> generateSeedOption = make_shared<MenuOption>(mainMenu, "Generate Seed", "Generate a random seed.");
	mainMenu->add(generateSeedOption);
	shared_ptr<MenuOption> generateAddressOption = make_shared<MenuOption>(mainMenu, "Generate Address", "Generate an address.");
	mainMenu->add(generateAddressOption);

	shared_ptr<Menu> generateAddressMenu = make_shared<Menu>("Generate Address", mainMenu);
	generateAddressOption->add(static_pointer_cast<MenuTreeObject>(generateAddressMenu));
}

shared_ptr<MenuTreeObject> MenuTree::getRoot()
{
	return root;
}
