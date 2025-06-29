#ifndef PROMPT_H
#define PROMPT_H

#include "menu.h"

#define PROMPT_DESCRIPTION_HEIGHT 7

using namespace std;

namespace RetroCrypto
{
	class Prompt : public Menu
	{
	private:
		string description;
		shared_ptr<MenuTreeObject> destination;
	public:
		Prompt();
		Prompt(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void drawOptions(shared_ptr<IDisplay> display) override;
		virtual void drawDescription(shared_ptr<IDisplay> display) override;
		string getDescription();
		void setDescription(string newDescription);
		virtual shared_ptr<MenuTreeObject> getDestination() override;
		void setDestination(shared_ptr<MenuTreeObject> newDestination);
	};
}
#endif
