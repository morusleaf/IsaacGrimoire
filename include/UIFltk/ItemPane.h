#pragma once

#include <FL/Fl_Scroll.H>
#include "Item.h"
#include "UIFltk/InfoPane.h"

class ItemPane : public Fl_Scroll {
private:
	InfoPane *infopane;
public:
	ItemPane(int x, int y, int w, int h, InfoPane *infopane);
	~ItemPane();
	void refresh(ItemList *roomItems, ItemList *playerItems, ImageList *roomImgs, ImageList *playerImgs, bool roomListChanged);
	void resetBoxesType();
};
