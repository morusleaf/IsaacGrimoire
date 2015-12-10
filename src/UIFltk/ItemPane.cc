#include "UIFltk/InfoPane.H"
#include "UIFltk/ItemPane.H"
#include "UIFltk/ItemImageBox.H"
#include "util.h"
#include <FL/Fl_PNG_Image.H>
#include "UIFltk/Layout.H"
#include <assert.h>

ItemPane::ItemPane(int x, int y, int w, int h, InfoPane *_infopane) :
	Fl_Scroll(x, y, w, h) {
	infopane = _infopane;
	box(FL_DOWN_BOX);
	type(Fl_Scroll::VERTICAL_ALWAYS);
	begin();
	end();
}

ItemPane::~ItemPane() {
	UILog("destruct ItemPane");
}

void ItemPane::resetBoxesType() {
	int n = children();
	for (int i = 0; i<n; i++) {
		ItemImageBox *itembox = (ItemImageBox*)child(i);
		if (itembox->box() == FL_DOWN_BOX) {
			itembox->box(FL_FLAT_BOX);
			itembox->redraw();
		}
	}
}

void ItemPane::refresh(ItemList *roomItems, ItemList *playerItems, ImageList *roomImgs, ImageList *playerImgs, bool roomListChanged) {
	UILog("refresh ItemPane");
	clear();
	assert(roomItems->size() == roomImgs->size());
	assert(playerItems->size() == playerImgs->size());
	int roomListSize = roomItems->size();
	int index = 0;
	for (int i = 0; i < roomListSize; i++) {
		ItemImageBox *itembox = new ItemImageBox(ITEMBOX_X(index), ITEMBOX_Y(index), ITEM_W, ITEM_H, infopane, roomItems->at(i));
		itembox->image(roomImgs->at(i));
		add(itembox);
		if (i == 0 && roomListChanged) itembox->setChosen();
		index++;
	}
	int playerListSize = playerItems->size();
	for (int i = 0; i < playerListSize; i++) {
		ItemImageBox *itembox = new ItemImageBox(ITEMBOX_X(index), ITEMBOX_Y(index), ITEM_W, ITEM_H, infopane, playerItems->at(i));
		itembox->image(playerImgs->at(i));
		itembox->color(FL_DARK1);
		add(itembox);
		index++;
	}
	redraw();
}
