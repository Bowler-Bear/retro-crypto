#ifndef VANITY_INPUT_PAGE_H
#define VANITY_INPUT_PAGE_H

#include "input_page.h"
#include "core_context.h"

using namespace std;

namespace RetroCrypto
{
	enum GenerationState
	{
		INPUT,
		GENERATING,
		FOUND
	};

	struct GenerationData
	{
		uint32_t currentAttempt;
		ContextData seedData;
		string matchString;
		string address;
	};

	class VanityInputPage : public InputPage
	{
	protected:
		string prefix;
		GenerationState currentState;
		GenerationData generationData;
	public:
		VanityInputPage();
		VanityInputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual bool canConsumeAllInputs() override;
		virtual bool consumeInput(InputType input) override;
		virtual shared_ptr<MenuTreeObject> getDestination() override;
		virtual void reset() override;
		virtual void onForward() override;
		virtual void onEnter() override;
		virtual void onExit() override;
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual void drawInput(shared_ptr<IDisplay> display) override;
		virtual void tick() override;
		void drawGenerationPage(shared_ptr<IDisplay> display);
	};
}
#endif
