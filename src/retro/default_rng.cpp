#include <ctime>
#include <stdexcept>
#include <string>

#include "default_rng.h"

#define BYTES_PER_32_BIT_INT 4

using namespace RetroCrypto;

DefaultRNG::DefaultRNG()
{
	seeded = false;
}

void DefaultRNG::seedGenerator(const uint8_t* seed, uint8_t seedSize)
{
	if (!seeded)
	{
		chacha_drbg_reseed(&chachaDrbgContext, seed, seedSize, nullptr, 0);
		seeded = true;
	}
	else
	{
		chacha_drbg_reseed(&chachaDrbgContext, nullptr, 0, seed, seedSize);
	}
}

void DefaultRNG::generateBytes(uint8_t *output, size_t outputSize)
{
	if (!seeded)
		throw std::runtime_error(std::string(__func__)+": RNG was not seeded before request.");
	chacha_drbg_generate(&chachaDrbgContext, output, outputSize);
}

uint8_t DefaultRNG::getRandom8()
{
	return getRandom32() & 0xFF;
}

uint16_t DefaultRNG::getRandom16()
{
	return getRandom32() & 0xFFFF;
}

uint32_t DefaultRNG::getRandom32()
{
	uint8_t buffer[BYTES_PER_32_BIT_INT] = { 0 };
	generateBytes(buffer, BYTES_PER_32_BIT_INT);
	uint32_t randomInt = 0;
	for(uint8_t i = 0; i < BYTES_PER_32_BIT_INT; i++)
		randomInt += (buffer[i] << (24-8*i));
	return randomInt;
}
