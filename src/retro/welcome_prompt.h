#ifndef WELCOME_PROMPT_H
#define WELCOME_PROMPT_H

#include "prompt.h"

using namespace std;

namespace RetroCrypto
{
	class WelcomePrompt : public Prompt
	{
	private:
		uint8_t getRandomTipIndex();

	public:
		static const char* tips[];

		WelcomePrompt();
		WelcomePrompt(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void onEnter() override;
	};
}
#endif
