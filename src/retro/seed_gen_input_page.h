#ifndef SEED_GEN_INPUT_PAGE_H
#define SEED_GEN_INPUT_PAGE_H

#include "input_page.h"

using namespace std;

namespace RetroCrypto
{

	enum SeedGenPageType
	{
		DICE,
		DIRECTIONAL
	};

	class SeedGenInputPage : public InputPage
	{
	protected:
		SeedGenPageType pageType;
	public:
		SeedGenInputPage();
		SeedGenInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void updateSelectedOption(InputType input) override;
		virtual shared_ptr<MenuTreeObject> getDestination() override;
		virtual void onForward() override;
		void setSeedGenInputType(SeedGenPageType newInput);
	};
}
#endif
