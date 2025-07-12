#ifndef RETRO_RNG_H
#define RETRO_RNG_H

#include <stdint.h>

namespace RetroCrypto
{
	class IRandomNumberGenerator
	{
	public:
		virtual void seedGenerator(unsigned seed) = 0;

		virtual uint8_t getRandom8() = 0;

		virtual uint16_t getRandom16() = 0;

		virtual uint32_t getRandom32() = 0;
	};
}
#endif
