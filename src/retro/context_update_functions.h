#include <stdint.h>

#include "core_context.h"

namespace RetroCrypto
{
	void setRandom256BitSeed();

	void setRandomSeed(uint8_t seedSize);

	void setSeedFromHashedData(const uint8_t* data, uint8_t length);

	void setMnemonicTypeContext(MnemonicType newMnemonicTypeContext);

	void clearMnemonicTypeContext();

	void setBIP39MnemonicContext();

	void setLegacyMoneroMnemonicContext();

	void clearMnemonicContext();

	void setMnemonicContext();

	void setCryptoContext(CryptoType newCryptoContext);

	void clearCryptoContext();

	void setBitcoinContext();

	void setDogeContext();

	void setEthereumClassicContext();

	void setEthereumContext();

	void setNostrContext();

	void setMoneroContext();
}
