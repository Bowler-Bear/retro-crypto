#include "core_context.h"

using namespace RetroCrypto;

void Context::updateData(ContextUpdate update, ContextData dataContainer)
{
	if (update & ContextUpdate::SEED_SIZE)
	{
		data.seedSize = dataContainer.seedSize;
	}
	if (update & ContextUpdate::SEED)
	{
		data.setSeed(dataContainer.seed, dataContainer.seedSize);
	}
	if (update & ContextUpdate::MNEMONIC_TYPE)
	{
		data.mnemonicType = dataContainer.mnemonicType;
	}
	if (update & ContextUpdate::MNEMONIC)
	{
		data.mnemonic = dataContainer.mnemonic;
	}
	if (update & ContextUpdate::CRYPTO)
	{
		data.crypto = dataContainer.crypto;
	}
}

ContextData Context::getData()
{
	return data;
}
