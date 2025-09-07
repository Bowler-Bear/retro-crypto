#ifndef ADDRESS_PAGE_H
#define ADDRESS_PAGE_H

#include "page.h"
#include "crypto_functions.h"

using namespace std;

namespace RetroCrypto
{
	class AddressPage : public Page
	{
	protected:
		AddressInformation addressInformation;
		string addressTitle;
		string seed;
		string seedTitle;
		string xpriv;
		string xpub;

	public:
		AddressPage();
		AddressPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual void onEnter() override;
		virtual void onBackward() override;
		void drawSeed(shared_ptr<IDisplay> display);
		void drawAddress(shared_ptr<IDisplay> display);
		void drawXpriv(shared_ptr<IDisplay> display);
		void drawXpub(shared_ptr<IDisplay> display);
	};
}
#endif
