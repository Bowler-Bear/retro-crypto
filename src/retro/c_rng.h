#ifndef C_RNG_H
#define C_RNG_H

#include "random_number_generator.h"

namespace RetroCrypto
{
	class CRNG : public IRandomNumberGenerator
	{
	public:
		CRNG();

		virtual void seedGenerator(unsigned seed);

		virtual uint8_t getRandom8();

		virtual uint16_t getRandom16();

		virtual uint32_t getRandom32();
	};
}
#endif
