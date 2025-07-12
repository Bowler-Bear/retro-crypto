#include <cstdlib>
#include <ctime>

#include "c_rng.h"

using namespace RetroCrypto;

CRNG::CRNG()
{
	seedGenerator(std::time(0));
}

void CRNG::seedGenerator(unsigned int seed)
{
	std::srand(seed);
}

uint8_t CRNG::getRandom8()
{
	return getRandom32() & 0xFF;
}

uint16_t CRNG::getRandom16()
{
	return getRandom32() & 0xFFFF;
}

uint32_t CRNG::getRandom32()
{
	return std::rand();
}
