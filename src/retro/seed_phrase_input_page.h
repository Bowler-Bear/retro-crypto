#ifndef SEED_PHRASE_INPUT_PAGE_H
#define SEED_PHRASE_INPUT_PAGE_H

#include "input_page.h"

using namespace std;

namespace RetroCrypto
{
	class SeedPhraseInputPage : public InputPage
	{
	protected:
		string previousWord;
		string predictedWord;
	public:
		SeedPhraseInputPage();
		SeedPhraseInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void updateSelectedOption(InputType input) override;
		virtual bool canConsumeAllInputs() override;
		virtual bool consumeInput(InputType input) override;
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual shared_ptr<MenuTreeObject> getDestination() override;
		virtual void reset() override;
		virtual void onEnter() override;
		void drawPredictedWord(shared_ptr<IDisplay> display);
		void loadCurrentInputWord(char* currentWord, bool onlyUpToCursor = true);
	};
}
#endif
