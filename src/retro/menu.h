#ifndef MENU_H
#define MENU_H

#include <memory>
#include <vector>

#include "menu_tree_object.h"
#include "menu_option.h"

using namespace std;

namespace RetroCrypto
{
	class Menu : public MenuTreeObject
	{
	private:
		string title;

		vector<shared_ptr<MenuOption>> options;
	public:
		void setTitle(string title);

		string getTitle();

		virtual void add(shared_ptr<MenuTreeObject> option) override;

		void add(shared_ptr<MenuOption> option);
	};
}
#endif
