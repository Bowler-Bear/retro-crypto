#ifndef MENU_TREE_SYSTEM_H
#define MENU_TREE_SYSTEM_H

#include "menu_tree_object.h"

using namespace std;

namespace RetroCrypto
{
	class MenuTree
	{
	private:
		shared_ptr<MenuTreeObject> root;
	public:
		MenuTree();

		shared_ptr<MenuTreeObject> getRoot();
	};
}
#endif
