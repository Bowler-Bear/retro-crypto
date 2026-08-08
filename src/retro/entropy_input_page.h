#ifndef SEED_GEN_INPUT_PAGE_H
#define SEED_GEN_INPUT_PAGE_H

#include "input_page.h"

using namespace std;

namespace RetroCrypto
{

	enum EntropyType
	{
		DICE,
		DIRECTIONAL,
		COIN
	};

	class EntropyInputPage : public InputPage
	{
	protected:
		EntropyType pageType;
	public:
		EntropyInputPage();
		EntropyInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void updateSelectedOption(InputType input) override;
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual shared_ptr<MenuTreeObject> getDestination() override;
		virtual void onForward() override;
		void setEntropyInputType(EntropyType newInput);
		void drawCollisionOdds(shared_ptr<IDisplay> display);
	};
}
#endif
