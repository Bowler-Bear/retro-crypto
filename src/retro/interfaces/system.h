#ifndef SYSTEM_H
#define SYSTEM_H

namespace RetroCrypto
{
	class ISystem
	{
	protected:
		bool initialized;

		bool quitRequested;

	public:
		virtual bool init() = 0;

		virtual bool getInitialized()
		{
			return initialized;
		}

		virtual void setInitialized(bool newValue)
		{
			initialized = newValue;
		}

		virtual bool getQuitRequested()
		{
			return quitRequested;
		}

		void setQuitRequested(bool newValue)
		{
			quitRequested = newValue;
		}

		virtual void tick() = 0;
	};
}
#endif
