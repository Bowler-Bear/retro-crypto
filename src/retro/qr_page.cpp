#include "qr_page.h"
#include "core_system.h"
#include "crypto_functions.h"

using namespace RetroCrypto;

QRPage::QRPage()
: Page(), qr(qrcodegen::QrCode::encodeText("none", qrcodegen::QrCode::Ecc::LOW))
{
}

QRPage::QRPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: Page(inTitle, inParent), qr(qrcodegen::QrCode::encodeText("none", qrcodegen::QrCode::Ecc::LOW))
{
}

void QRPage::setEncodedData(QREncodedData newEncodedData)
{
	encodedData = newEncodedData;
}

void QRPage::draw(shared_ptr<IDisplay> display)
{
	drawBorder(display);
	drawQR(display);
}

void QRPage::onEnter()
{
	switch (encodedData)
	{
	case SEED:
		qr = qrcodegen::QrCode::encodeText(CoreSystem::getCoreSystem().getContextData().getSeedAsHexString().c_str(), qrcodegen::QrCode::Ecc::LOW);
		break;
	case ADDRESS:
		qr = qrcodegen::QrCode::encodeText(cryptoAddressFromGlobalContext().address.c_str(), qrcodegen::QrCode::Ecc::LOW);
		break;
	case MNEMONIC:
		qr = qrcodegen::QrCode::encodeText(mnemonicFromGlobalContext().c_str(), qrcodegen::QrCode::Ecc::LOW);
		break;
	case NONE:
	default:
		qr = qrcodegen::QrCode::encodeText("none", qrcodegen::QrCode::Ecc::LOW);
		break;
	}
}

void QRPage::drawQR(shared_ptr<IDisplay> display)
{
	QrBox qrBox(qr);
	qrBox.yPosition = BASE_BORDER_BOX_Y_POSITION+1;
	qrBox.xPosition = (BASE_BORDER_BOX_WIDTH-qr.getSize())/2;
	qrBox.width = qr.getSize();
	qrBox.height = qr.getSize();
	display->drawQrBox(qrBox);
}
