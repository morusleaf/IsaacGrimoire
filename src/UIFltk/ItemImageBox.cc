#include "UIFltk/ItemImageBox.H"
#include "UIFltk/ItemPane.H"
#include "util.h"
#include "Item.h"

ItemImageBox::ItemImageBox(int x, int y, int w, int h, InfoPane *_infopane, Item *_item) :
	Fl_Box(x, y, w, h) {
	infopane = _infopane;
	item = _item;
	box(FL_FLAT_BOX);
}

ItemImageBox::~ItemImageBox() {
	UILog("destruct ItemImageBox");
}

void ItemImageBox::setChosen() {
	((ItemPane*)parent())->resetBoxesType();
	box(FL_DOWN_BOX);
	redraw();
	infopane->refresh(item);
}

int ItemImageBox::handle(int evt) {
	if (evt == FL_PUSH) {
		UILog("handle ItemImageBox");
		setChosen();
		return 1;
	}
	else
		return 0;
}
