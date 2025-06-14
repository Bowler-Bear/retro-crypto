#ifndef MENU_OPTION_H
#define MENU_OPTION_H

#include <memory>
#include <string>

typedef void (*BlockingFunction)(void);

using namespace std;

namespace RetroCrypto
{
	class Menu;
	class MenuTreeObject;
	class MenuOption
	{
	private:
		string label;

		string description;

		bool disabled;

		shared_ptr<MenuTreeObject> parent;

		shared_ptr<MenuTreeObject> destination;

		BlockingFunction onSelectedFunction;
	public:
		MenuOption(const shared_ptr<Menu> menu, const string& label, const string& description);

		void add(shared_ptr<MenuTreeObject> destination);

		shared_ptr<MenuTreeObject> getDestination();

		string getLabel();

		string getDescription();

		bool getDisabled();

		void setDisabled(bool newValue);

		void setOnSelectedFunction(BlockingFunction function);

		void onSelected();
	};
}
#endif
