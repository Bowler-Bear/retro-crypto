#ifndef DRAWABLE_H
#define DRAWABLE_H

namespace RetroCrypto
{
	class IDrawable
	{
	public:
		virtual ~IDrawable() {}
		virtual void redraw() = 0;
		virtual void clear() = 0;
	};
}
#endif
