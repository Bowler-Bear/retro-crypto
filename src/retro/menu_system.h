#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "system.h"
#include "display_factory.h"
#include "menu_tree.h"
#include "input_capturer.h"

namespace RetroCrypto
{
	class MenuSystem : public ISystem
	{
	private:
		std::shared_ptr<MenuTree> menuTree;
		
		std::shared_ptr<MenuTreeObject> currentMenuPosition;

		int optionIndex;

		IDisplayFactory* displayFactory;

		IDisplay* display;

		MenuSystem();

		~MenuSystem();

		MenuSystem(MenuSystem const&);

		void operator=(MenuSystem const&);
	public:
		static MenuSystem* getMenuSystem();

		void setDisplayFactory(IDisplayFactory& factory);

		virtual bool init() override;

		virtual void tick() override;

		void redraw();

		void processInput(InputType input);
	};
}
#endif
