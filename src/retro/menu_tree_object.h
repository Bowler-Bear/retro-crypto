#ifndef MENU_TREE_OBJECT_H
#define MENU_TREE_OBJECT_H

#include <cstdint>
#include <memory>
#include <vector>

#include "retro_display.h"
#include "input_capturer.h"

#define BASE_BORDER_BOX_X_POSITION 0
#define BASE_BORDER_BOX_Y_POSITION 0
#define BASE_BORDER_BOX_WIDTH 100
#define BASE_BORDER_BOX_HEIGHT 25

#define MENU_TREE_OBJECT_REPARENT_DEFAULT true

typedef void (*BlockingAction)(void);

namespace RetroCrypto
{
	class MenuOption;
	class MenuTreeObject
	{
	protected:
		std::shared_ptr<MenuTreeObject> parent;
		bool shouldReparent;
		std::vector<std::shared_ptr<MenuOption>> options;
		uint8_t selectedOptionIndex;
		BlockingAction forwardAction;
		BlockingAction backwardAction;
	public:
		virtual void addOption(std::shared_ptr<MenuOption> option)
		{
			if (options.size() < 0xff)
				options.push_back(option);
		}
		virtual uint8_t getOptionCount()
		{
			return options.size() > 0xff ? 0xff : options.size();
		}
		virtual void draw(std::shared_ptr<IDisplay> display) = 0;
		virtual void drawBorder(std::shared_ptr<IDisplay> display)
		{
			Box border;
			border.xPosition = BASE_BORDER_BOX_X_POSITION;
			border.yPosition = BASE_BORDER_BOX_Y_POSITION;
			border.width = BASE_BORDER_BOX_WIDTH;
			border.height = BASE_BORDER_BOX_HEIGHT;
			display->drawBox(border);
		}
		virtual uint8_t getSelectedOption()
		{
			return selectedOptionIndex;
		}
		virtual void updateSelectedOption(InputType input) = 0;
		virtual bool canConsumeAllInputs()
		{
			return false;
		}
		virtual bool consumeInput(InputType input)
		{
			return false;
		}
		virtual std::shared_ptr<MenuTreeObject> getDestination() = 0;
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
		virtual void setShouldReparent(bool newShouldReparent)
		{
			shouldReparent = newShouldReparent;
		}
		virtual bool getShouldReparent()
		{
			return shouldReparent;
		}
		virtual void setForwardAction(BlockingAction action)
		{
			forwardAction = action;
		}
		virtual void setBackwardAction(BlockingAction action)
		{
			backwardAction = action;
		}
		virtual void onForward()
		{
			onExit();
			if (forwardAction != nullptr)
				(*forwardAction)();
		}
		virtual void onBackward()
		{
			onExit();
			if (backwardAction != nullptr)
				(*backwardAction)();
		}
		virtual void onEnter()
		{
		}
		virtual void onExit()
		{
		}
	};
}
#endif
