#ifndef SEED_QR_PAGE_H
#define SEED_QR_PAGE_H

#include "page.h"
#include "qrcodegen.hpp"

using namespace std;

namespace RetroCrypto
{
	class SeedQRPage : public Page
	{
	protected:
		qrcodegen::QrCode qr;

	public:
		SeedQRPage();
		SeedQRPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual void onEnter() override;
		void drawQR(shared_ptr<IDisplay> display);
	};
}
#endif
