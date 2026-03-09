#include "menu_tree.h"
#include "menu.h"
#include "prompt.h"
#include "address_page.h"
#include "mnemonic_page.h"
#include "vanity_input_page.h"
#include "seed_gen_input_page.h"
#include "seed_phrase_input_page.h"
#include "seed_qr_page.h"
#include "context_update_functions.h"
#include "encryption_page.h"
#include "welcome_prompt.h"

using namespace RetroCrypto;

MenuTree::MenuTree()
{
	shared_ptr<WelcomePrompt> welcomePrompt = make_shared<WelcomePrompt>("Welcome");
	root = static_pointer_cast<MenuTreeObject>(welcomePrompt);

	shared_ptr<Menu> mainMenu = make_shared<Menu>("Main Menu");
	mainMenu->setShouldReparent(false);
	welcomePrompt->setDestination(static_pointer_cast<MenuTreeObject>(mainMenu));

	shared_ptr<MenuOption> generateSeedOption = make_shared<MenuOption>(mainMenu, "Generate Seed", "Generate a random seed.");
	shared_ptr<MenuOption> generateAddressOption = make_shared<MenuOption>(mainMenu, "Generate Vanity Address", "Generate an address.");
	shared_ptr<MenuOption> seedRestoreOption = make_shared<MenuOption>(mainMenu, "Restore Seed", "Restore a seed from a mnemonic.");
	shared_ptr<MenuOption> encryptionOption = make_shared<MenuOption>(mainMenu, "Encryption", "(En/De)crypt data.");
	shared_ptr<MenuOption> exitOption = make_shared<MenuOption>(mainMenu, "Exit", "Exit this program.");
	exitOption->setDestination(static_pointer_cast<MenuTreeObject>(mainMenu));
	exitOption->setOnSelectedFunction(&requestExit);
	mainMenu->addOption(generateSeedOption);
	mainMenu->addOption(generateAddressOption);
	mainMenu->addOption(seedRestoreOption);
	mainMenu->addOption(encryptionOption);
	mainMenu->addOption(exitOption);

		shared_ptr<Prompt> seedPrompt = make_shared<Prompt>("Seed Information", mainMenu);
		seedPrompt->setDescription("This seed will be used through secure key derivative functions to create of sets of keys. The randomness of the seed is of importance as it is directly correlated to the randomness of the keys. Using dice is advised as they can provide the most security given 30+ rolls. User inputs are less secure as the user may not randomly pick the inputs. The RNG option is likely the least secure as it needs to be seeded itself and the inner workings are heavily system dependent");
		generateSeedOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPrompt));
		shared_ptr<Menu> generateSeedMenu = make_shared<Menu>("Generate Seed", seedPrompt);
		seedPrompt->setDestination(static_pointer_cast<MenuTreeObject>(generateSeedMenu));
		shared_ptr<MenuOption> generateSeedFromRngOption = make_shared<MenuOption>(generateSeedMenu, "From RNG", "Use random number generation.");
		shared_ptr<MenuOption> generateSeedFromDiceOption = make_shared<MenuOption>(generateSeedMenu, "From Dice", "Use physical dice to generate a seed.");
		shared_ptr<MenuOption> generateSeedFromInputOption = make_shared<MenuOption>(generateSeedMenu, "From Input", "Use user input.");
		generateSeedMenu->addOption(generateSeedFromRngOption);
		generateSeedMenu->addOption(generateSeedFromDiceOption);
		generateSeedMenu->addOption(generateSeedFromInputOption);

			shared_ptr<Prompt> generateSeedFromRNGPrompt = make_shared<Prompt>("Generate Seed From RNG", generateSeedMenu);
			generateSeedFromRngOption->setDestination(static_pointer_cast<MenuTreeObject>(generateSeedFromRNGPrompt));
			generateSeedFromRNGPrompt->setDescription("You are about to generate a seed using random number generation(RNG).");
			generateSeedFromRNGPrompt->setForwardAction(&setRandom256BitSeed);

				shared_ptr<Menu> showSeedOptionsMenu = make_shared<Menu>("Seed Options", mainMenu);
				showSeedOptionsMenu->setShouldReparent(false);
				showSeedOptionsMenu->setBackwardAction(&clearSeed);
				generateSeedFromRNGPrompt->setDestination(static_pointer_cast<MenuTreeObject>(showSeedOptionsMenu));
				shared_ptr<MenuOption> showAddressesOption = make_shared<MenuOption>(showSeedOptionsMenu, "Show Addresses", "Show addresses from this seed.");
				shared_ptr<MenuOption> showSeedPhrasesOption = make_shared<MenuOption>(showSeedOptionsMenu, "Show Seed Phrases", "Show seed phrases for this seed.");
				shared_ptr<MenuOption> showSeedQROption = make_shared<MenuOption>(showSeedOptionsMenu, "Show QR code", "Show QR code of this seed.");
				shared_ptr<MenuOption> mainMenuReturnOption = make_shared<MenuOption>(showSeedOptionsMenu, "Main Menu", "Return to main menu.");
				mainMenuReturnOption->setDestination(static_pointer_cast<MenuTreeObject>(mainMenu));
				mainMenuReturnOption->setOnSelectedFunction(&clearSeed);
				showSeedOptionsMenu->addOption(showAddressesOption);
				showSeedOptionsMenu->addOption(showSeedPhrasesOption);
				showSeedOptionsMenu->addOption(showSeedQROption);
				showSeedOptionsMenu->addOption(mainMenuReturnOption);

					shared_ptr<Prompt> addressPrompt = make_shared<Prompt>("About Addresses", showSeedOptionsMenu);
					addressPrompt->setDescription("This app does not yet have wallet functionality and cannot connect to the blockchain to verify a test transaction. If the user is planning on using these addresses, they should first import the key into that coins official wallet and verify the addresses before sending any funds to it.");
					showAddressesOption->setDestination(static_pointer_cast<MenuTreeObject>(addressPrompt));
					shared_ptr<Menu> showAddressMenu = make_shared<Menu>("Show An Address", addressPrompt);
					addressPrompt->setDestination(static_pointer_cast<MenuTreeObject>(showAddressMenu));
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
					showAddressMenu->addOption(showBTCAddressOption);
					showAddressMenu->addOption(showDOGEAddressOption);
					showAddressMenu->addOption(showETCAddressOption);
					showAddressMenu->addOption(showETHAddressOption);
					showAddressMenu->addOption(showNOSTRAddressOption);
					showAddressMenu->addOption(showXMRAddressOption);

						shared_ptr<AddressPage> addressPage = make_shared<AddressPage>("Seed Address", showAddressMenu);
						addressPage->setBackwardAction(&clearCryptoContext);
						showBTCAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(addressPage));
						showDOGEAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(addressPage));
						showETCAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(addressPage));
						showETHAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(addressPage));
						showNOSTRAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(addressPage));
						showXMRAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(addressPage));

					shared_ptr<Prompt> phrasePrompt = make_shared<Prompt>("Phrase Information", showSeedOptionsMenu);
					phrasePrompt->setDescription("A seed phrase is a word-based representation of your seed. Save it non-electronically. Make backups if it is essential to avoid losing. Backing it up might be risky for those under high surveillance or with high theft risk. Memorizing it is possible, as a brain wallet, but comes with a high rate of failure. It's probably not a good idea to split up a phrase into multiple segments for security. User's should look for more information beyond what any app explains, and take time to think how they personally want to secure any important seed.");
					showSeedPhrasesOption->setDestination(static_pointer_cast<MenuTreeObject>(phrasePrompt));
					shared_ptr<Menu> showPhrasesMenu = make_shared<Menu>("Show A Phrase", phrasePrompt);
					phrasePrompt->setDestination(static_pointer_cast<MenuTreeObject>(showPhrasesMenu));
					shared_ptr<MenuOption> showBIP39Option = make_shared<MenuOption>(showPhrasesMenu, "BIP-39", "Show Bitcoin Improvement Proposal 39 seed phrase for this seed(bits).");
					showBIP39Option->setOnSelectedFunction(&setBIP39MnemonicContext);
					shared_ptr<MenuOption> showMoneroEnglishPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero English", "Show legacy monero english seed phrase for this seed.");
					showMoneroEnglishPhraseOption->setOnSelectedFunction(&setLegacyMoneroEnglishMnemonicContext);
					shared_ptr<MenuOption> showMoneroFrenchPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero French", "Show legacy monero french seed phrase for this seed.");
					showMoneroFrenchPhraseOption->setOnSelectedFunction(&setLegacyMoneroFrenchMnemonicContext);
					shared_ptr<MenuOption> showMoneroItalianPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Italian", "Show legacy monero italian seed phrase for this seed.");
					showMoneroItalianPhraseOption->setOnSelectedFunction(&setLegacyMoneroItalianMnemonicContext);
					shared_ptr<MenuOption> showMoneroDutchPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Dutch", "Show legacy monero dutch seed phrase for this seed.");
					showMoneroDutchPhraseOption->setOnSelectedFunction(&setLegacyMoneroDutchMnemonicContext);
					shared_ptr<MenuOption> showMoneroPortuguesePhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Portuguese", "Show legacy monero portuguese seed phrase for this seed.");
					showMoneroPortuguesePhraseOption->setOnSelectedFunction(&setLegacyMoneroPortugueseMnemonicContext);
#if UTF8_COMPAT_BUILD
					shared_ptr<MenuOption> showMoneroGermanPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero German", "Show legacy monero german seed phrase for this seed.");
					showMoneroGermanPhraseOption->setOnSelectedFunction(&setLegacyMoneroGermanMnemonicContext);
					shared_ptr<MenuOption> showMoneroSpanishPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Spanish", "Show legacy monero spanish seed phrase for this seed.");
					showMoneroSpanishPhraseOption->setOnSelectedFunction(&setLegacyMoneroSpanishMnemonicContext);
					shared_ptr<MenuOption> showMoneroRussianPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Russian", "Show legacy monero russian seed phrase for this seed.");
					showMoneroRussianPhraseOption->setOnSelectedFunction(&setLegacyMoneroRussianMnemonicContext);
					shared_ptr<MenuOption> showMoneroJapanesePhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Japanese", "Show legacy monero japanese seed phrase for this seed.");
					showMoneroJapanesePhraseOption->setOnSelectedFunction(&setLegacyMoneroJapaneseMnemonicContext);
					shared_ptr<MenuOption> showMoneroChinesePhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Chinese", "Show legacy monero chinese seed phrase for this seed.");
					showMoneroChinesePhraseOption->setOnSelectedFunction(&setLegacyMoneroChineseMnemonicContext);
#endif
					shared_ptr<MenuOption> showMoneroEsperantoPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Esperanto", "Show legacy monero esperanto seed phrase for this seed.");
					showMoneroEsperantoPhraseOption->setOnSelectedFunction(&setLegacyMoneroEsperantoMnemonicContext);
					shared_ptr<MenuOption> showMoneroLojbanPhraseOption = make_shared<MenuOption>(showPhrasesMenu, "Legacy Monero Lojban", "Show legacy monero lojban seed phrase for this seed.");
					showMoneroLojbanPhraseOption->setOnSelectedFunction(&setLegacyMoneroLojbanMnemonicContext);
					showPhrasesMenu->addOption(showBIP39Option);
					showPhrasesMenu->addOption(showMoneroEnglishPhraseOption);
#if UTF8_COMPAT_BUILD
					showPhrasesMenu->addOption(showMoneroGermanPhraseOption);
					showPhrasesMenu->addOption(showMoneroSpanishPhraseOption);
#endif
					showPhrasesMenu->addOption(showMoneroFrenchPhraseOption);
					showPhrasesMenu->addOption(showMoneroItalianPhraseOption);
					showPhrasesMenu->addOption(showMoneroDutchPhraseOption);
					showPhrasesMenu->addOption(showMoneroPortuguesePhraseOption);
#if UTF8_COMPAT_BUILD
					showPhrasesMenu->addOption(showMoneroRussianPhraseOption);
					showPhrasesMenu->addOption(showMoneroJapanesePhraseOption);
					showPhrasesMenu->addOption(showMoneroChinesePhraseOption);
#endif
					showPhrasesMenu->addOption(showMoneroEsperantoPhraseOption);
					showPhrasesMenu->addOption(showMoneroLojbanPhraseOption);

						shared_ptr<MnemonicPage> mnemonicPage = make_shared<MnemonicPage>("Seed Phrase", showPhrasesMenu);
						addressPage->setBackwardAction(&clearMnemonicTypeContext);
						showBIP39Option->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroEnglishPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroFrenchPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroItalianPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroDutchPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroPortuguesePhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
#if UTF8_COMPAT_BUILD
						showMoneroGermanPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroSpanishPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroRussianPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroJapanesePhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroChinesePhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
#endif
						showMoneroEsperantoPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));
						showMoneroLojbanPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(mnemonicPage));

					shared_ptr<Prompt> qrPrompt = make_shared<Prompt>("QR", showSeedOptionsMenu);
					qrPrompt->setDescription("Pointing one device's camera at another device's screen, or even the light cast by it, could allow hidden backdoors to transfer data beyond what the user intends to transfer.");
					showSeedQROption->setDestination(static_pointer_cast<MenuTreeObject>(qrPrompt));
					shared_ptr<SeedQRPage> seedQrPage = make_shared<SeedQRPage>("QR Code", qrPrompt);
					qrPrompt->setDestination(static_pointer_cast<MenuTreeObject>(seedQrPage));

			shared_ptr<SeedGenInputPage> diceRollsInputPage = make_shared<SeedGenInputPage>("Enter Dice Rolls", generateSeedMenu);
			diceRollsInputPage->setDestination(static_pointer_cast<MenuTreeObject>(showSeedOptionsMenu));
			generateSeedFromDiceOption->setDestination(static_pointer_cast<MenuTreeObject>(diceRollsInputPage));
			diceRollsInputPage->setDescription("Enter a series of randomly rolled dice values to generate a seed with.");

			shared_ptr<SeedGenInputPage> directionalInputsPage = make_shared<SeedGenInputPage>("Enter Directional Inputs", generateSeedMenu);
			directionalInputsPage->setSeedGenInputType(DIRECTIONAL);
			directionalInputsPage->setDestination(static_pointer_cast<MenuTreeObject>(showSeedOptionsMenu));
			generateSeedFromInputOption->setDestination(static_pointer_cast<MenuTreeObject>(directionalInputsPage));
			directionalInputsPage->setDescription("Enter a series of random directions to generate a seed with.");

		shared_ptr<Menu> generateAddressMenu = make_shared<Menu>("Generate Vanity Address", mainMenu);
		generateAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(generateAddressMenu));
		shared_ptr<MenuOption> generateXMRAddressOption = make_shared<MenuOption>(generateAddressMenu, "XMR", "Generate a monero public address.");
		generateXMRAddressOption->setOnSelectedFunction(&setMoneroContext);
		shared_ptr<MenuOption> generateBTCAddressOption = make_shared<MenuOption>(generateAddressMenu, "BTC", "Generate a bitcoin public address.");
		generateBTCAddressOption->setOnSelectedFunction(&setBitcoinContext);
		shared_ptr<MenuOption> generateETHAddressOption = make_shared<MenuOption>(generateAddressMenu, "ETH", "Generate a ethereum public address.");
		generateETHAddressOption->setOnSelectedFunction(&setEthereumContext);
		shared_ptr<MenuOption> generateETCAddressOption = make_shared<MenuOption>(generateAddressMenu, "ETC", "Generate a ethereum classic public address.");
		generateETCAddressOption->setOnSelectedFunction(&setEthereumClassicContext);
		shared_ptr<MenuOption> generateDOGEAddressOption = make_shared<MenuOption>(generateAddressMenu, "DOGE", "Generate a DOGE public address.");
		generateDOGEAddressOption->setOnSelectedFunction(&setDogeContext);
		shared_ptr<MenuOption> generateNostrAddressOption = make_shared<MenuOption>(generateAddressMenu, "Nostr", "Generate a Nostr public key.");
		generateNostrAddressOption->setOnSelectedFunction(&setNostrContext);
		generateAddressMenu->addOption(generateXMRAddressOption);
		generateAddressMenu->addOption(generateBTCAddressOption);
		generateAddressMenu->addOption(generateETHAddressOption);
		generateAddressMenu->addOption(generateETCAddressOption);
		generateAddressMenu->addOption(generateDOGEAddressOption);
		generateAddressMenu->addOption(generateNostrAddressOption);

			shared_ptr<VanityInputPage> vanityAddressInput = make_shared<VanityInputPage>("Input Vanity Address", generateAddressMenu);
			vanityAddressInput->setDestination(static_pointer_cast<MenuTreeObject>(showSeedOptionsMenu));
			generateXMRAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(vanityAddressInput));
			generateBTCAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(vanityAddressInput));
			generateETHAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(vanityAddressInput));
			generateETCAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(vanityAddressInput));
			generateDOGEAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(vanityAddressInput));
			generateNostrAddressOption->setDestination(static_pointer_cast<MenuTreeObject>(vanityAddressInput));

		shared_ptr<Menu> pickPhraseTypeMenu = make_shared<Menu>("Pick Seed Phrase Type", mainMenu);
		seedRestoreOption->setDestination(static_pointer_cast<MenuTreeObject>(pickPhraseTypeMenu));
		shared_ptr<MenuOption> restoreFromBIP39Option = make_shared<MenuOption>(pickPhraseTypeMenu, "BIP-39", "Restore seed bits from Bitcoin Improvement Proposal 39 seed phrase.");
		restoreFromBIP39Option->setOnSelectedFunction(&setBIP39MnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroEnglishPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero English", "Restore seed bit from legacy monero english seed phrase.");
		restoreFromLegacyMoneroEnglishPhraseOption->setOnSelectedFunction(&setLegacyMoneroEnglishMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroFrenchPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero French", "Restore seed bit from legacy monero french seed phrase.");
		restoreFromLegacyMoneroFrenchPhraseOption->setOnSelectedFunction(&setLegacyMoneroFrenchMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroItalianPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Italian", "Restore seed bit from legacy monero italian seed phrase.");
		restoreFromLegacyMoneroItalianPhraseOption->setOnSelectedFunction(&setLegacyMoneroItalianMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroDutchPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Dutch", "Restore seed bit from legacy monero dutch seed phrase.");
		restoreFromLegacyMoneroDutchPhraseOption->setOnSelectedFunction(&setLegacyMoneroDutchMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroPortuguesePhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Portuguese", "Restore seed bit from legacy monero portuguese seed phrase.");
		restoreFromLegacyMoneroPortuguesePhraseOption->setOnSelectedFunction(&setLegacyMoneroPortugueseMnemonicContext);
#if UTF8_COMPAT_BUILD
		shared_ptr<MenuOption> restoreFromLegacyMoneroGermanPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero German", "Restore seed bit from legacy monero german seed phrase.");
		restoreFromLegacyMoneroGermanPhraseOption->setOnSelectedFunction(&setLegacyMoneroGermanMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroSpanishPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Spanish", "Restore seed bit from legacy monero spanish seed phrase.");
		restoreFromLegacyMoneroSpanishPhraseOption->setOnSelectedFunction(&setLegacyMoneroSpanishMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroRussianPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Russian", "Restore seed bit from legacy monero russian seed phrase.");
		restoreFromLegacyMoneroRussianPhraseOption->setOnSelectedFunction(&setLegacyMoneroRussianMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroJapanesePhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Japanese", "Restore seed bit from legacy monero japanese seed phrase.");
		restoreFromLegacyMoneroJapanesePhraseOption->setOnSelectedFunction(&setLegacyMoneroJapaneseMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroChinesePhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Chinese", "Restore seed bit from legacy monero chinese seed phrase.");
		restoreFromLegacyMoneroChinesePhraseOption->setOnSelectedFunction(&setLegacyMoneroChineseMnemonicContext);
#endif
		shared_ptr<MenuOption> restoreFromLegacyMoneroEsperantoPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Esperanto", "Restore seed bit from legacy monero esperanto seed phrase.");
		restoreFromLegacyMoneroEsperantoPhraseOption->setOnSelectedFunction(&setLegacyMoneroEsperantoMnemonicContext);
		shared_ptr<MenuOption> restoreFromLegacyMoneroLojbanPhraseOption = make_shared<MenuOption>(pickPhraseTypeMenu, "Legacy Monero Lojban", "Restore seed bit from legacy monero lojban seed phrase.");
		restoreFromLegacyMoneroLojbanPhraseOption->setOnSelectedFunction(&setLegacyMoneroLojbanMnemonicContext);
		pickPhraseTypeMenu->addOption(restoreFromBIP39Option);
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroEnglishPhraseOption);
#if UTF8_COMPAT_BUILD
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroGermanPhraseOption);
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroSpanishPhraseOption);
#endif
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroFrenchPhraseOption);
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroItalianPhraseOption);
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroDutchPhraseOption);
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroPortuguesePhraseOption);
#if UTF8_COMPAT_BUILD
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroRussianPhraseOption);
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroJapanesePhraseOption);
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroChinesePhraseOption);
#endif
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroEsperantoPhraseOption);
		pickPhraseTypeMenu->addOption(restoreFromLegacyMoneroLojbanPhraseOption);

			shared_ptr<SeedPhraseInputPage> seedPhraseInputPage = make_shared<SeedPhraseInputPage>("Input Seed Phrase", pickPhraseTypeMenu);
			seedPhraseInputPage->setDestination(static_pointer_cast<MenuTreeObject>(showSeedOptionsMenu));
			seedPhraseInputPage->setBackwardAction(&clearMnemonicTypeContext);
			restoreFromBIP39Option->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroEnglishPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroFrenchPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroItalianPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroDutchPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroPortuguesePhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
#if UTF8_COMPAT_BUILD
			restoreFromLegacyMoneroGermanPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroSpanishPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroRussianPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroJapanesePhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroChinesePhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
#endif
			restoreFromLegacyMoneroEsperantoPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));
			restoreFromLegacyMoneroLojbanPhraseOption->setDestination(static_pointer_cast<MenuTreeObject>(seedPhraseInputPage));

		shared_ptr<Prompt> encryptionPrompt = make_shared<Prompt>("End-to-End Encryption", mainMenu);
		encryptionPrompt->setDescription("E2E encryption has become a marketing term widely used to gaslight the public in the age of security theatre. True E2E encryption requires both the sender's and receiver's devices to have no leakpoints and/or backdoors.");
		encryptionOption->setDestination(static_pointer_cast<MenuTreeObject>(encryptionPrompt));
		shared_ptr<Menu> encryptionMenu = make_shared<Menu>("Select Option", mainMenu);
		encryptionPrompt->setDestination(static_pointer_cast<MenuTreeObject>(encryptionMenu));
		shared_ptr<MenuOption> encryptOption = make_shared<MenuOption>(encryptionMenu, "Encrypt", "Encrypt data using AES-256.");
		shared_ptr<MenuOption> decryptOption = make_shared<MenuOption>(encryptionMenu, "Decrypt", "Decrypt data using AES-256.");
		shared_ptr<MenuOption> encryptMainMenuReturnOption = make_shared<MenuOption>(encryptionMenu, "Main Menu", "Return to Main Menu.");
		encryptMainMenuReturnOption->setDestination(static_pointer_cast<MenuTreeObject>(mainMenu));
		encryptionMenu->addOption(encryptOption);
		encryptionMenu->addOption(decryptOption);
		encryptionMenu->addOption(encryptMainMenuReturnOption);

			shared_ptr<EncryptionPage> encryptionPage = make_shared<EncryptionPage>("Encryption Page", encryptionMenu);
			encryptOption->setDestination(static_pointer_cast<MenuTreeObject>(encryptionPage));
			shared_ptr<EncryptionPage> decryptionPage = make_shared<EncryptionPage>("Decryption Page", encryptionMenu);
			decryptOption->setDestination(static_pointer_cast<MenuTreeObject>(decryptionPage));
			decryptionPage->setWillDecrypt();
}

shared_ptr<MenuTreeObject> MenuTree::getRoot()
{
	return root;
}
