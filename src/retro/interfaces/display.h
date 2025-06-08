#ifndef DISPLAY_H
#define DISPLAY_H

#include "drawable.h"

namespace RetroCrypto
{

	class IDisplay: public IDrawable
	{
	public:
		IDisplay()
		{
		}
		virtual void redraw() override = 0;
		virtual void clear() override = 0;
	};
}
#endif
