#include "address_page.h"
#include "core_system.h"
#include "crypto_functions.h"
#include "crypto_definitions.h"

#define STRING(s) #s
#define S(s) STRING(s)

using namespace RetroCrypto;

AddressPage::AddressPage()
: Page()
{
	address = "";
	addressTitle = "";
	seedTitle = "";
	seed = "";
}

AddressPage::AddressPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: AddressPage()
{
	title = inTitle;
	parent = inParent;
}

void AddressPage::draw(shared_ptr<IDisplay> display)
{
	Page::draw(display);
	drawSeed(display);
	drawAddress(display);
}

void AddressPage::onEnter()
{
	if (address == "")
	{
		address = cryptoAddressFromGlobalContext();
	}
	if (seed == "")
	{
		ContextData contextData = CoreSystem::getCoreSystem().getContextData();
		seed = contextData.getSeedAsHexString();
		switch(contextData.crypto)
		{
		case RetroCrypto::CryptoType::BTC:
			seedTitle = "Seed Bits";
			addressTitle = "Bitcoin m/" S(BITCOIN_PATH_PURPOSE) "'/" S(BITCOIN_PATH_COIN_TYPE) "'/" S(BITCOIN_PATH_ACCOUNT) "' ";
			break;
		case RetroCrypto::CryptoType::DOGE:
			seedTitle = "Seed Bits";
			addressTitle = "Doge m/" S(DOGE_PATH_PURPOSE) "'/" S(DOGE_PATH_COIN_TYPE) "'/" S(DOGE_PATH_ACCOUNT) "' ";
			break;
		case RetroCrypto::CryptoType::ETC:
			seedTitle = "Seed Bits";
			addressTitle = "Ethereum Classic m/" S(ETHEREUM_CLASSIC_PATH_PURPOSE) "'/" S(ETHEREUM_CLASSIC_PATH_COIN_TYPE) "'/" S(ETHEREUM_CLASSIC_PATH_ACCOUNT) "' ";
			break;
		case RetroCrypto::CryptoType::ETH:
			seedTitle = "Seed Bits";
			addressTitle = "Ethereum m/" S(ETHEREUM_PATH_PURPOSE) "'/" S(ETHEREUM_PATH_COIN_TYPE) "'/" S(ETHEREUM_PATH_ACCOUNT) "' ";
			break;
		case RetroCrypto::CryptoType::NOSTR:
			seedTitle = "Seed Bits";
			addressTitle = "Nostr m/" S(NOSTR_PATH_PURPOSE) "'/" S(NOSTR_PATH_COIN_TYPE) "'/" S(NOSTR_PATH_ACCOUNT) "'/" S(NOSTR_PATH_CHANGE) "/" S(NOSTR_PATH_INDEX) " ";
			break;
		case RetroCrypto::CryptoType::XMR:
			seedTitle = "Private Spend Key";
			addressTitle = "Monero Primary ";
			break;
		default:
			break;
		}
	}
}

void AddressPage::onBackward()
{
	Page::onBackward();
	address = "";
	addressTitle = "";
	seedTitle = "";
	seed = "";
}

void AddressPage::drawSeed(shared_ptr<IDisplay> display)
{
	TextBox seedTitleBox(seedTitle);
	seedTitleBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT+1;
	seedTitleBox.xPosition = (BASE_BORDER_BOX_WIDTH-seedTitleBox.text.size())/2;
	seedTitleBox.width = seedTitleBox.text.size();
	seedTitleBox.height = 3;
	seedTitleBox.setUnderlined();
	seedTitleBox.setBold();
	display->drawTextBox(seedTitleBox);

	TextBox seedBox(seed);
	seedBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT+3;
	seedBox.xPosition = 2;
	seedBox.width = BASE_BORDER_BOX_WIDTH-3;
	seedBox.height = 3;
	seedBox.setBordered();
	display->drawTextBox(seedBox);
}

void AddressPage::drawAddress(shared_ptr<IDisplay> display)
{
	TextBox addressTitleBox(addressTitle+string("Address"));
	addressTitleBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT+6;
	addressTitleBox.xPosition = (BASE_BORDER_BOX_WIDTH-addressTitleBox.text.size())/2;
	addressTitleBox.width = addressTitleBox.text.size();
	addressTitleBox.height = 3;
	addressTitleBox.setUnderlined();
	addressTitleBox.setBold();
	display->drawTextBox(addressTitleBox);

	TextBox addressBox(address);
	addressBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+PAGE_TITLE_BOX_HEIGHT+8;
	addressBox.xPosition = 2;
	addressBox.width = BASE_BORDER_BOX_WIDTH-3;
	addressBox.height = 3;
	addressBox.setBordered();
	display->drawTextBox(addressBox);
}
