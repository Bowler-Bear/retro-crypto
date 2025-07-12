#include "page.h"

using namespace RetroCrypto;

Page::Page()
{
	selectedOptionIndex = 0;
	title = "";
	parent = nullptr;
}

Page::Page(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: Page()
{
	title = inTitle;
	parent = inParent;
}

void Page::addOption(std::shared_ptr<MenuOption> option)
{
}

void Page::updateSelectedOption(InputType input)
{
}

void Page::draw(shared_ptr<IDisplay> display)
{
	drawBorder(display);
	drawTitle(display);
}

shared_ptr<MenuTreeObject> Page::getDestination()
{
	return nullptr;
}

void Page::onBackward()
{
	MenuTreeObject::onBackward();
}

void Page::drawTitle(shared_ptr<IDisplay> display)
{
	TextBox titleBox(title);
	titleBox.yPosition = PAGE_TITLE_BOX_Y_POSITION;
	titleBox.xPosition = (BASE_BORDER_BOX_WIDTH-titleBox.text.size())/2;
	titleBox.width = titleBox.text.size()+1;
	titleBox.height = PAGE_TITLE_BOX_HEIGHT;
	titleBox.setUnderlined();
	titleBox.setBold();
	titleBox.setBordered();
	display->drawTextBox(titleBox);
}
