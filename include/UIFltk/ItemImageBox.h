#pragma once

#include "UIFltk/InfoPane.h"
#include <FL/Fl_Box.H>
#include "Item.h"

class ItemImageBox : public Fl_Box {
private:
	InfoPane *infopane;
	Item *item;
public:
	void setChosen();
	ItemImageBox(int x, int y, int w, int h, InfoPane *infopane, Item *item);
	~ItemImageBox();
	int handle(int evt);
};
