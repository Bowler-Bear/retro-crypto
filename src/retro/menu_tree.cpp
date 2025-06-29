#include "menu_tree.h"
#include "menu.h"
#include "prompt.h"
#include "address_page.h"
#include "context_update_functions.h"

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

			shared_ptr<Prompt> generateSeedFromRNGPrompt = make_shared<Prompt>("Generate Seed From RNG", generateSeedMenu);
			generateSeedFromRngOption->add(static_pointer_cast<MenuTreeObject>(generateSeedFromRNGPrompt));
			generateSeedFromRNGPrompt->setDescription("You are about to generate a seed using random number generation(RNG).");
			generateSeedFromRNGPrompt->setForwardAction(&setRandom256BitSeed);

				shared_ptr<Menu> showSeedOptionsMenu = make_shared<Menu>("Seed Options", generateSeedFromRNGPrompt);
				generateSeedFromRNGPrompt->setDestination(static_pointer_cast<MenuTreeObject>(showSeedOptionsMenu));
				shared_ptr<MenuOption> showAddressesOption = make_shared<MenuOption>(showSeedOptionsMenu, "Show Addresses", "Show addresses from this seed.");
				shared_ptr<MenuOption> showSeedPhrasesOption = make_shared<MenuOption>(showSeedOptionsMenu, "Show Seed Phrases", "Show seed phrases for this seed.");
				showSeedOptionsMenu->add(showAddressesOption);
				showSeedOptionsMenu->add(showSeedPhrasesOption);

					shared_ptr<Menu> showAddressMenu = make_shared<Menu>("Show An Address", showSeedOptionsMenu);
					showAddressesOption->add(static_pointer_cast<MenuTreeObject>(showAddressMenu));
					shared_ptr<MenuOption> showBTCAddressOption = make_shared<MenuOption>(showAddressMenu, "BTC", "Show bitcoin public address from seed.");
					showBTCAddressOption->setOnSelectedFunction(&setBitcoinContext);
					shared_ptr<MenuOption> showDOGEAddressOption = make_shared<MenuOption>(showAddressMenu, "DOGE", "show DOGE public address from seed.");
					showDOGEAddressOption->setOnSelectedFunction(&setDogeContext);
					shared_ptr<MenuOption> showETCAddressOption = make_shared<MenuOption>(showAddressMenu, "ETC", "Show ethereum classic public address from seed.");
					showETCAddressOption->setOnSelectedFunction(&setEthereumClassicContext);
					shared_ptr<MenuOption> showETHAddressOption = make_shared<MenuOption>(showAddressMenu, "ETH", "Show ethereum public address from seed.");
					showETHAddressOption->setOnSelectedFunction(&setEthereumContext);
					shared_ptr<MenuOption> showNOSTRAddressOption = make_shared<MenuOption>(showAddressMenu, "Nostr", "show Nostr public keys from seed.");
					showNOSTRAddressOption->setOnSelectedFunction(&setNostrContext);
					shared_ptr<MenuOption> showXMRAddressOption = make_shared<MenuOption>(showAddressMenu, "XMR", "Show monero public address from seed.");
					showXMRAddressOption->setOnSelectedFunction(&setMoneroContext);
					showAddressMenu->add(showBTCAddressOption);
					showAddressMenu->add(showDOGEAddressOption);
					showAddressMenu->add(showETCAddressOption);
					showAddressMenu->add(showETHAddressOption);
					showAddressMenu->add(showNOSTRAddressOption);
					showAddressMenu->add(showXMRAddressOption);

						shared_ptr<AddressPage> addressPage = make_shared<AddressPage>("Seed Address", showAddressMenu);
						addressPage->setBackwardAction(&clearCryptoContext);
						showBTCAddressOption->add(static_pointer_cast<MenuTreeObject>(addressPage));
						showDOGEAddressOption->add(static_pointer_cast<MenuTreeObject>(addressPage));
						showETCAddressOption->add(static_pointer_cast<MenuTreeObject>(addressPage));
						showETHAddressOption->add(static_pointer_cast<MenuTreeObject>(addressPage));
						showNOSTRAddressOption->add(static_pointer_cast<MenuTreeObject>(addressPage));
						showXMRAddressOption->add(static_pointer_cast<MenuTreeObject>(addressPage));

					shared_ptr<Menu> showPhrasesMenu = make_shared<Menu>("Show A Phrase", showSeedOptionsMenu);
					showSeedPhrasesOption->add(static_pointer_cast<MenuTreeObject>(showPhrasesMenu));
					shared_ptr<MenuOption> showBIP39Option = make_shared<MenuOption>(showPhrasesMenu, "BIP-39", "Show Bitcoin Improvement Proposal 39 seed phrase for this seed(bits).");
					showBIP39Option->setOnSelectedFunction(&setBIP39MnemonicContext);
					shared_ptr<MenuOption> showMoneroPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero", "Show legacy monero seed phrase for this seed.");
					showMoneroPhraseOption->setOnSelectedFunction(&setLegacyMoneroMnemonicContext);
					showPhrasesMenu->add(showBIP39Option);
					showPhrasesMenu->add(showMoneroPhraseOption);

		shared_ptr<Menu> generateAddressMenu = make_shared<Menu>("Generate Vanity Address", mainMenu);
		generateAddressOption->add(static_pointer_cast<MenuTreeObject>(generateAddressMenu));
		shared_ptr<MenuOption> generateXMRAddressOption = make_shared<MenuOption>(generateAddressMenu, "XMR", "Generate a monero public address.");
		shared_ptr<MenuOption> generateBTCAddressOption = make_shared<MenuOption>(generateAddressMenu, "BTC", "Generate a bitcoin public address.");
		shared_ptr<MenuOption> generateETHAddressOption = make_shared<MenuOption>(generateAddressMenu, "ETH", "Generate a ethereum public address.");
		shared_ptr<MenuOption> generateETCAddressOption = make_shared<MenuOption>(generateAddressMenu, "ETC", "Generate a ethereum classic public address.");
		shared_ptr<MenuOption> generateDOGEAddressOption = make_shared<MenuOption>(generateAddressMenu, "DOGE", "Generate a DOGE public address.");
		shared_ptr<MenuOption> generateNostrAddressOption = make_shared<MenuOption>(generateAddressMenu, "Nostr", "Generate a Nostr public keys.");
		generateAddressMenu->add(generateXMRAddressOption);
		generateAddressMenu->add(generateBTCAddressOption);
		generateAddressMenu->add(generateETHAddressOption);
		generateAddressMenu->add(generateETCAddressOption);
		generateAddressMenu->add(generateDOGEAddressOption);
		generateAddressMenu->add(generateNostrAddressOption);

		shared_ptr<Menu> pickPhraseTypeMenu = make_shared<Menu>("Pick Seed Phrase Type", mainMenu);
		seedRestoreOption->add(static_pointer_cast<MenuTreeObject>(pickPhraseTypeMenu));
		shared_ptr<MenuOption> restoreFromBIP39Option = make_shared<MenuOption>(pickPhraseTypeMenu, "BIP-39", "Restore seed bits from Bitcoin Improvement Proposal 39 seed phrase.");
		restoreFromBIP39Option->setOnSelectedFunction(&setBIP39MnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero", "Restore seed bit from legacy monero seed phrase.");
		restoreFromLegacyMoneroPhraseOption->setOnSelectedFunction(&setLegacyMoneroMnemonicContext);
		pickPhraseTypeMenu->add(restoreFromBIP39Option);
		pickPhraseTypeMenu->add(restoreFromLegacyMoneroPhraseOption);

}

shared_ptr<MenuTreeObject> MenuTree::getRoot()
{
	return root;
}
