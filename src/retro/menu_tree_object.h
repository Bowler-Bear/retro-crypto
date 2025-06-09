#ifndef MENU_TREE_OBJECT_H
#define MENU_TREE_OBJECT_H

#include <memory>
#include <vector>

#include "display.h"

namespace RetroCrypto
{
	class MenuOption;
	class MenuTreeObject
	{
	protected:
		std::shared_ptr<MenuTreeObject> parent;
		std::vector<std::shared_ptr<MenuOption>> options;
	public:
		virtual void add(std::shared_ptr<MenuTreeObject> child) = 0;
		virtual void draw(IDisplay* display, int optionIndex) = 0;
		virtual std::vector<std::shared_ptr<MenuOption>>& getOptions()
		{
			return options;
		}
	};
}
#endif
