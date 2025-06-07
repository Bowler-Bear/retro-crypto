#ifndef MENU_FACTORY_H
#define MENU_FACTORY_H

#include "menu.h"

namespace RetroCrypto
{
	class IMenuFactory
	{
	public:
		virtual IMenu* constructMenu() = 0;
	};
}
#endif
