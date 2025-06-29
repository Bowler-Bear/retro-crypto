#include "context_update_functions.h"
#include "core_system.h"

namespace RetroCrypto
{
	void setRandom256BitSeed()
	{
		setRandomSeed(256/8);
	}

	void setRandomSeed(uint8_t seedSize = MAXIMUM_SEED_SIZE)
	{
		if (seedSize == 0 || seedSize > MAXIMUM_SEED_SIZE)
			seedSize = MAXIMUM_SEED_SIZE;
		ContextData newData;
		newData.seedSize = seedSize;
		for(int i = 0; i < newData.seedSize; i++)
			newData.seed[i] = std::rand() & 0xFF;
		CoreSystem::getCoreSystem().updateContextData(ContextUpdate::SEED | ContextUpdate::SEED_SIZE, newData);
	}

	void setMnemonicTypeContext(MnemonicType newMnemonicTypeContext)
	{
		CoreSystem::getCoreSystem().updateContextData(ContextUpdate::MNEMONIC_TYPE, ContextData(newMnemonicTypeContext));
	}

	void clearMnemonicTypeContext()
	{
		setMnemonicTypeContext(MnemonicType::NONE);
	}

	void setBIP39MnemonicContext()
	{
		setMnemonicTypeContext(MnemonicType::BIP39);
	}

	void setLegacyMoneroMnemonicContext()
	{
		setMnemonicTypeContext(MnemonicType::LEGACY_MONERO);
	}

	void clearMnemonicContext()
	{
		CoreSystem::getCoreSystem().updateContextData(ContextUpdate::MNEMONIC, ContextData());
	}

	void setMnemonicContext(std::string inMnemonic)
	{
		CoreSystem::getCoreSystem().updateContextData(ContextUpdate::MNEMONIC, ContextData(inMnemonic));
	}

	void setCryptoContext(CryptoType newCryptoContext)
	{
		CoreSystem::getCoreSystem().updateContextData(ContextUpdate::CRYPTO, ContextData(newCryptoContext));
	}

	void clearCryptoContext()
	{
		setCryptoContext(CryptoType::NONE);
	}

	void setBitcoinContext()
	{
		setCryptoContext(CryptoType::BTC);
	}

	void setDogeContext()
	{
		setCryptoContext(CryptoType::DOGE);
	}

	void setEthereumClassicContext()
	{
		setCryptoContext(CryptoType::ETC);
	}

	void setEthereumContext()
	{
		setCryptoContext(CryptoType::ETH);
	}

	void setNostrContext()
	{
		setCryptoContext(CryptoType::NOSTR);
	}

	void setMoneroContext()
	{
		setCryptoContext(CryptoType::XMR);
	}
}
