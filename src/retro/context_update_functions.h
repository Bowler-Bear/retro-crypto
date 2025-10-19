#ifndef CONTEXT_UPDATE_FUNCTIONS_H
#define CONTEXT_UPDATE_FUNCTIONS_H

#include <stdint.h>

#include "core_context.h"

namespace RetroCrypto
{
	void requestExit();

	void clearSeed();

	void setRandom256BitSeed();

	void setRandomSeed(uint8_t seedSize);

	void setSeedFromHashedData(const uint8_t* data, uint8_t length);

	void setMnemonicTypeContext(MnemonicType newMnemonicTypeContext);

	void clearMnemonicTypeContext();

	void setBIP39MnemonicContext();

	void setLegacyMoneroEnglishMnemonicContext();

	void setLegacyMoneroGermanMnemonicContext();

	void setLegacyMoneroSpanishMnemonicContext();

	void setLegacyMoneroFrenchMnemonicContext();

	void setLegacyMoneroItalianMnemonicContext();

	void setLegacyMoneroDutchMnemonicContext();

	void setLegacyMoneroPortugueseMnemonicContext();

	void setLegacyMoneroRussianMnemonicContext();

	void setLegacyMoneroJapaneseMnemonicContext();

	void setLegacyMoneroChineseMnemonicContext();

	void setLegacyMoneroEsperantoMnemonicContext();

	void setLegacyMoneroLojbanMnemonicContext();

	void clearMnemonicContext();

	void setMnemonicContext(std::string inMnemonic);

	void setCryptoContext(CryptoType newCryptoContext);

	void clearCryptoContext();

	void setBitcoinContext();

	void setDogeContext();

	void setEthereumClassicContext();

	void setEthereumContext();

	void setNostrContext();

	void setMoneroContext();
}

#endif
