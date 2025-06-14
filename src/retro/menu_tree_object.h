#ifndef MENU_TREE_OBJECT_H
#define MENU_TREE_OBJECT_H

#include <memory>
#include <vector>

#include "display.h"
#include "input_capturer.h"

typedef void (*BlockingAction)(void);

namespace RetroCrypto
{
	class MenuOption;
	class MenuTreeObject
	{
	protected:
		std::shared_ptr<MenuTreeObject> parent;
		std::vector<std::shared_ptr<MenuOption>> options;
		BlockingAction forwardAction;
		BlockingAction backwardAction;
	public:
		virtual void add(std::shared_ptr<MenuTreeObject> child) = 0;
		virtual void draw(std::shared_ptr<IDisplay> display, int selectedOptionIndex) = 0;
		virtual int getNewSelectedOption(int selectedOptionIndex, InputType input) = 0;
		virtual std::shared_ptr<MenuTreeObject> getDestination(int selectedOptionIndex) = 0;
		virtual std::vector<std::shared_ptr<MenuOption>>& getOptions()
		{
			return options;
		}
		virtual std::shared_ptr<MenuTreeObject> getParent()
		{
			return parent;
		}
		virtual void setParent(std::shared_ptr<MenuTreeObject> newParent)
		{
			parent = newParent;
		}
		virtual void setForwardAction(BlockingAction action)
		{
			forwardAction = action;
		}
		virtual void setBackwardAction(BlockingAction action)
		{
			backwardAction = action;
		}
		virtual void onForward(int selectedOptionIndex)
		{
			if (forwardAction != nullptr)
				(*forwardAction)();
		}
		virtual void onBackward(int selectedOptionIndex)
		{
			if (backwardAction != nullptr)
				(*backwardAction)();
		}
	};
}
#endif
