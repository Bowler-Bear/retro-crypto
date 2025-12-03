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
	QrBox qrBox(qr);
	qrBox.yPosition = BASE_BORDER_BOX_Y_POSITION+1;
	qrBox.xPosition = (BASE_BORDER_BOX_WIDTH-qr.getSize())/2;
	qrBox.width = qr.getSize();
	qrBox.height = qr.getSize();
	display->drawQrBox(qrBox);
}
