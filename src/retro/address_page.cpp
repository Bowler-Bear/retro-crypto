#include "address_page.h"
#include "core_system.h"
#include "crypto_functions.h"

using namespace RetroCrypto;

AddressPage::AddressPage()
{
	title = "";
	address = "";
	addressType = "";
	seed = "";
}

AddressPage::AddressPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: AddressPage()
{
	title = inTitle;
	parent = inParent;
}

void AddressPage::add(std::shared_ptr<MenuTreeObject> child)
{
}

int AddressPage::getNewSelectedOption(int selectedOptionIndex, InputType input)
{
	return 0;
}

void AddressPage::draw(shared_ptr<IDisplay> display, int selectedOptionIndex)
{
	if (address == "")
		address = cryptoAddressFromGlobalContext();
	if (seed == "")
	{
		ContextData contextData = CoreSystem::getCoreSystem().getContextData();
		seed = contextData.getSeedAsHexString();
		switch(contextData.crypto)
		{
		case RetroCrypto::CryptoType::BTC:
			addressType = "Bitcoin ";
			break;
		case RetroCrypto::CryptoType::DOGE:
			addressType = "Doge ";
			break;
		case RetroCrypto::CryptoType::ETC:
			addressType = "Ethereum Classic ";
			break;
		case RetroCrypto::CryptoType::ETH:
			addressType = "Ethereum ";
			break;
		case RetroCrypto::CryptoType::NOSTR:
			addressType = " Nostr ";
			break;
		case RetroCrypto::CryptoType::XMR:
			addressType = "Monero ";
			break;
		default:
			break;
		}
	}
	drawBorder(display, selectedOptionIndex);
	drawTitle(display);
	drawSeed(display);
	drawAddress(display);
}

shared_ptr<MenuTreeObject> AddressPage::getDestination(int selectedOptionIndex)
{
	return nullptr;
}

void AddressPage::onBackward(int selectedOptionIndex)
{
	MenuTreeObject::onBackward(selectedOptionIndex);
	address = "";
	addressType = "";
	seed = "";
}

void AddressPage::drawTitle(shared_ptr<IDisplay> display)
{
	TextBox titleBox(title);
	titleBox.yPosition = PAGE_TITLE_BOX_Y_POSITION;
	titleBox.xPosition = (BASE_BORDER_BOX_WIDTH-titleBox.text.size())/2;
	titleBox.width = titleBox.text.size()+1;
	titleBox.height = PAGE_TITLE_BOX_HEIGHT;
	titleBox.setUnderlined();
	titleBox.setBold();
	titleBox.setBordered();
	display->drawTextBox(titleBox);
}

void AddressPage::drawSeed(shared_ptr<IDisplay> display)
{
	TextBox seedTitleBox(string("Seed"));
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
	TextBox addressTitleBox(addressType+string("Address"));
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
