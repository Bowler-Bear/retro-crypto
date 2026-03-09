#include "prompt.h"

using namespace RetroCrypto;

Prompt::Prompt()
: Menu()
{
}

Prompt::Prompt(std::string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: Menu(inTitle, inParent)
{
}


void Prompt::drawOptions(shared_ptr<IDisplay> display)
{
	TextBox optionBox("Continue");
	optionBox.yPosition = MENU_BOX_Y_POSITION + MENU_TITLE_BOX_HEIGHT + PROMPT_DESCRIPTION_HEIGHT + 5;
	optionBox.xPosition = (MENU_BOX_WIDTH-optionBox.text.size())/2;
	optionBox.width = optionBox.text.size()+3;
	optionBox.height = 3;
	optionBox.setBordered();
	optionBox.setBold();
	optionBox.setBlinking();
	display->drawTextBox(optionBox);
}

void Prompt::drawDescription(shared_ptr<IDisplay> display)
{
	TextBox descriptionBox;
	descriptionBox.yPosition = MENU_BOX_Y_POSITION + MENU_TITLE_BOX_HEIGHT + 4;
	descriptionBox.xPosition = MENU_BOX_X_POSITION + 1;
	descriptionBox.width = MENU_BOX_WIDTH-2;
	descriptionBox.height = PROMPT_DESCRIPTION_HEIGHT;
	descriptionBox.setBordered();
	if (getDescription().length() <= descriptionBox.width)
		descriptionBox.text = getDescription();
	display->drawTextBox(descriptionBox);
	if (getDescription().length() > descriptionBox.width)
	{
		TextBox descriptionLine;
		descriptionLine.xPosition = descriptionBox.xPosition;
		descriptionLine.width = descriptionBox.width;
		descriptionLine.height = 1;
		const uint8_t reductionOffset = 4;
		const uint8_t lineWidth = (descriptionBox.width-reductionOffset);
		const uint8_t descriptionLines = (getDescription().length()/lineWidth)+1;
		descriptionLine.yPosition = MENU_BOX_Y_POSITION + MENU_TITLE_BOX_HEIGHT + 4 + ((PROMPT_DESCRIPTION_HEIGHT-2)/2+1) - (descriptionLines/2+1);
		for(int i = 0; i < descriptionLines; i++)
		{
			descriptionLine.text = getDescription().substr(i*lineWidth, lineWidth);
			descriptionLine.yPosition += 1;
			display->drawTextBox(descriptionLine);
		}
	}
}

string Prompt::getDescription()
{
	return description;
}

void Prompt::setDescription(string newDescription)
{
	description = newDescription;
}

shared_ptr<MenuTreeObject> Prompt::getDestination()
{
	return destination;
}

void Prompt::setDestination(shared_ptr<MenuTreeObject> newDestination)
{
	destination = newDestination;
}
