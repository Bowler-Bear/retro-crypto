#include "menu_tree.h"
#include "menu.h"

using namespace RetroCrypto;

MenuTree::MenuTree()
{
	shared_ptr<Menu> mainMenu = make_shared<Menu>("Main Menu");
	root = static_pointer_cast<MenuTreeObject>(mainMenu);

	shared_ptr<MenuOption> generateSeedOption = make_shared<MenuOption>(mainMenu, "Generate Seed", "Generate a random seed.");
	shared_ptr<MenuOption> generateAddressOption = make_shared<MenuOption>(mainMenu, "Generate Vanity Address", "Generate an address.");
	shared_ptr<MenuOption> seedRestoreOption = make_shared<MenuOption>(mainMenu, "Restore Seed", "Restore a seed from a mnemonic.");
	mainMenu->add(generateSeedOption);
	mainMenu->add(generateAddressOption);
	mainMenu->add(seedRestoreOption);

		shared_ptr<Menu> generateSeedMenu = make_shared<Menu>("Generate Seed", mainMenu);
		generateSeedOption->add(static_pointer_cast<MenuTreeObject>(generateSeedMenu));
		shared_ptr<MenuOption> generateSeedFromRngOption = make_shared<MenuOption>(generateSeedMenu, "From RNG", "Use random number generation.");
		shared_ptr<MenuOption> generateSeedFromDiceOption = make_shared<MenuOption>(generateSeedMenu, "From Dice", "Use physical dice to generate a seed.");
		shared_ptr<MenuOption> generateSeedFromInputOption = make_shared<MenuOption>(generateSeedMenu, "From Input", "Use user input.");
		generateSeedMenu->add(generateSeedFromRngOption);
		generateSeedMenu->add(generateSeedFromDiceOption);
		generateSeedMenu->add(generateSeedFromInputOption);

		shared_ptr<Menu> generateAddressMenu = make_shared<Menu>("Generate Vanity Address", mainMenu);
		generateAddressOption->add(static_pointer_cast<MenuTreeObject>(generateAddressMenu));
		shared_ptr<MenuOption> generateXMRAddressOption = make_shared<MenuOption>(generateAddressMenu, "XMR", "Generate a monero public address.");
		shared_ptr<MenuOption> generateBTCAddressOption = make_shared<MenuOption>(generateAddressMenu, "BTC", "Generate a bitcoin public address.");
		shared_ptr<MenuOption> generateETHAddressOption = make_shared<MenuOption>(generateAddressMenu, "ETH", "Generate a ethereum public address.");
		shared_ptr<MenuOption> generateETCAddressOption = make_shared<MenuOption>(generateAddressMenu, "ETH Classic", "Generate a ethereum public address.");
		shared_ptr<MenuOption> generateDOGEAddressOption = make_shared<MenuOption>(generateAddressMenu, "DOGE", "Generate a DOGE public address.");
		shared_ptr<MenuOption> generateNostrAddressOption = make_shared<MenuOption>(generateAddressMenu, "Nostr", "Generate a Nostr public keys.");
		generateAddressMenu->add(generateXMRAddressOption);
		generateAddressMenu->add(generateBTCAddressOption);
		generateAddressMenu->add(generateETHAddressOption);
		generateAddressMenu->add(generateETCAddressOption);
		generateAddressMenu->add(generateDOGEAddressOption);
		generateAddressMenu->add(generateNostrAddressOption);
}

shared_ptr<MenuTreeObject> MenuTree::getRoot()
{
	return root;
}
