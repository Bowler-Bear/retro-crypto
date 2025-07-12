#ifndef PAGE_H
#define PAGE_H

#include "menu_tree_object.h"

#define PAGE_TITLE_BOX_Y_POSITION 2
#define PAGE_TITLE_BOX_HEIGHT 5

using namespace std;

namespace RetroCrypto
{
	class Page : public MenuTreeObject
	{
	protected:
		string title;

	public:
		Page();
		Page(string inTitle, std::shared_ptr<MenuTreeObject> inParent = nullptr);
		virtual void addOption(std::shared_ptr<MenuOption> option) override;
		virtual void updateSelectedOption(InputType input) override;
		virtual void draw(shared_ptr<IDisplay> display) override;
		virtual shared_ptr<MenuTreeObject> getDestination() override;
		virtual void onBackward() override;
		virtual void drawTitle(shared_ptr<IDisplay> display);
	};
}
#endif
