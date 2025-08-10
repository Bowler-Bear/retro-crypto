#include "seed_qr_page.h"
#include "core_system.h"
using namespace RetroCrypto;

SeedQRPage::SeedQRPage()
: Page(), qr(qrcodegen::QrCode::encodeText("none", qrcodegen::QrCode::Ecc::LOW))
{
}

SeedQRPage::SeedQRPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: Page(inTitle, inParent), qr(qrcodegen::QrCode::encodeText("none", qrcodegen::QrCode::Ecc::LOW))
{
}

void SeedQRPage::draw(shared_ptr<IDisplay> display)
{
	drawBorder(display);
	drawQR(display);
}

void SeedQRPage::onEnter()
{
	ContextData contextData = CoreSystem::getCoreSystem().getContextData();
	qr = qrcodegen::QrCode::encodeText(contextData.getSeedAsHexString().c_str(), qrcodegen::QrCode::Ecc::LOW);
}

void SeedQRPage::drawQR(shared_ptr<IDisplay> display)
{
	for (int y = 0; y < qr.getSize(); y++)
	{
		for (int x = 0; x < qr.getSize(); x++)
		{
			TextBox blockBox(qr.getModule(x, y) ? "##" : "  ");
			blockBox.yPosition = PAGE_TITLE_BOX_Y_POSITION-1+y;
			blockBox.xPosition = (BASE_BORDER_BOX_WIDTH-2*qr.getSize())/2+2*x;
			blockBox.width = 2;
			blockBox.height = 1;
			display->drawTextBox(blockBox);
		}
	}
}
