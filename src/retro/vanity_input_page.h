#ifndef VANITY_INPUT_PAGE_H
#define VANITY_INPUT_PAGE_H

#include "input_page.h"

using namespace std;

namespace RetroCrypto
{
	class VanityInputPage : public InputPage
	{
	protected:
		string prefix;
	public:
		VanityInputPage();
		VanityInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void onEnter() override;
		virtual void drawInput(shared_ptr<IDisplay> display);
	};
}
#endif
