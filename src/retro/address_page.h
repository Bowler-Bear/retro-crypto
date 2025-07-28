#ifndef ADDRESS_PAGE_H
#define ADDRESS_PAGE_H

#include "page.h"

using namespace std;

namespace RetroCrypto
{
	class AddressPage : public Page
	{
	protected:
		string address;
		string addressType;
		string seed;
		string seedTitle;

	public:
		AddressPage();
		AddressPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual void onEnter() override;
		virtual void onBackward() override;
		void drawSeed(shared_ptr<IDisplay> display);
		void drawAddress(shared_ptr<IDisplay> display);
	};
}
#endif
