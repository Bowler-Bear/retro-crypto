#ifndef MENU_TREE_OBJECT_H
#define MENU_TREE_OBJECT_H

#include "display.h"

namespace RetroCrypto
{
	class MenuTreeObject
	{
	protected:
		std::shared_ptr<MenuTreeObject> parent;
	public:
		virtual void add(std::shared_ptr<MenuTreeObject> child) = 0;
		virtual void draw(IDisplay* display, int optionIndex) = 0;
	};
}
#endif
