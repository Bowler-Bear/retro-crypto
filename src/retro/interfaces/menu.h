#ifndef MENU_H
#define MENU_H

#include "drawable.h"

namespace RetroCrypto
{

	class IMenu: public IDrawable
	{
	public:
		IMenu()
		{
		}
		virtual void redraw() override = 0;
		virtual void clear() override = 0;
	};
}
#endif
