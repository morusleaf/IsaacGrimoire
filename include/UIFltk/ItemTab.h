#pragma once

#include <FL/Fl_Group.H>
#include "UIFltk/ItemPane.h"
#include "UIFltk/InfoPane.h"
#include "Item.h"
#include "Inquirer.h"

class ItemTab : public Fl_Group {
private:
	ItemList *lastRoomItems;
	ItemList *lastPlayerItems;
	Inquirer *inquirer;
public:
	ItemPane *itempane;
	InfoPane *infopane;
	ItemTab(int x, int y, int w, int h, const char *title, Inquirer *inquirer);
	~ItemTab();
	void refresh();
};
