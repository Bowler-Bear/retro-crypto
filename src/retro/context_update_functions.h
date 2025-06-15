#include <stdint.h>

#include "core_context.h"

namespace RetroCrypto
{
	void setRandom256BitSeed();

	void setRandomSeed(uint8_t seedSize);

	void setCryptoContext(CryptoType newCryptoContext);

	void clearCryptoContext();

	void setBitcoinContext();

	void setDogeContext();

	void setEthereumClassicContext();

	void setEthereumContext();

	void setNostrContext();

	void setMoneroContext();
}
