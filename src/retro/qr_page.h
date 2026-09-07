#ifndef QR_PAGE_H
#define QR_PAGE_H

#include "page.h"
#include "qrcodegen.hpp"

using namespace std;

namespace RetroCrypto
{

	class QRPage : public Page
	{
	public:
		enum QREncodedData
		{
			NONE,
			SEED,
			ADDRESS,
			MNEMONIC
		};

	protected:
		qrcodegen::QrCode qr;
		QREncodedData encodedData;

	public:

		QRPage();
		QRPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		void setEncodedData(QREncodedData newEncodedData);
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual void onEnter() override;
		void drawQR(shared_ptr<IDisplay> display);
	};
}
#endif
