#ifndef DEFAULT_RNG_H
#define DEFAULT_RNG_H

#include "random_number_generator.h"

extern "C"
{
#include "chacha_drbg.h"
}

namespace RetroCrypto
{
	class DefaultRNG : public IRandomNumberGenerator
	{
		CHACHA_DRBG_CTX chachaDrbgContext;
		bool seeded;
	public:
		DefaultRNG();

		virtual void seedGenerator(const uint8_t* seed, uint8_t seedSize) override;

		virtual void generateBytes(uint8_t *output, size_t outputSize) override;

		virtual uint8_t getRandom8() override;

		virtual uint16_t getRandom16() override;

		virtual uint32_t getRandom32() override;
	};
}
#endif
