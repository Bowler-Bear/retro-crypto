#include "core_system.h"
#include "welcome_prompt.h"

using namespace RetroCrypto;

const char* WelcomePrompt::tips[] = {
	"Did you compile this from the source yourself?",
	"Auditing the source code and compiling it are the most secure way to use this program.",
	"Have you considered using an off-grid power source to disconnect from grid based backdoors?",
	"This program's RNG is only pseudo random. Using dice and coins are more reliable and secure."
};

WelcomePrompt::WelcomePrompt()
: Prompt()
{
}

WelcomePrompt::WelcomePrompt(std::string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: Prompt(inTitle, inParent)
{
}

uint8_t WelcomePrompt::getRandomTipIndex()
{
	std::shared_ptr<IRandomNumberGenerator> rng = CoreSystem::getCoreSystem().getRandomNumberGenerator();
	if (!rng)
		return 0;
	return rng->getRandom8() % (sizeof(tips)/sizeof(char*));
}

void WelcomePrompt::onEnter()
{
	if (tips != nullptr)
		setDescription(tips[getRandomTipIndex()]);
}
