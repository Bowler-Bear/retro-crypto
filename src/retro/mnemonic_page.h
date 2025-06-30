#ifndef MNEMONIC_PAGE_H
#define MNEMONIC_PAGE_H

#include "page.h"

using namespace std;

namespace RetroCrypto
{
	class MnemonicPage : public Page
	{
	protected:
		string mnemonic;
		string seed;

	public:
		MnemonicPage();
		MnemonicPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual void onEnter() override;
		virtual void onBackward() override;
		void drawSeed(shared_ptr<IDisplay> display);
		void drawMnemonic(shared_ptr<IDisplay> display);
	};
}
#endif
