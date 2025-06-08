#ifndef DISPLAY_FACTORY_H
#define DISPLAY_FACTORY_H

#include "display.h"

namespace RetroCrypto
{
	class IDisplayFactory
	{
	public:
		virtual IDisplay* constructDisplay() = 0;
	};
}
#endif
