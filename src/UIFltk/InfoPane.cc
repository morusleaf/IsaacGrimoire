#include "UIFltk/InfoPane.H"
#include "UIFltk/Layout.H"
#include "util.h"

InfoPane::InfoPane(int x, int y, int w, int h) :
	Fl_Group(x, y, w, h) {
	begin();
	title = new Fl_Output(TITLE_X, TITLE_Y, TITLE_W, TITLE_H);
	description = new Fl_Text_Display(DESCRIPTION_X, DESCRIPTION_Y,
		DESCRIPTION_W, DESCRIPTION_H);
	buffer = new Fl_Text_Buffer();
	title->cursor_color(FL_WHITE);
	description->buffer(buffer);
	description->wrap_mode(Fl_Text_Display::WRAP_AT_COLUMN, 0);
	end();
}

InfoPane::~InfoPane() {
	UILog("destruct InfoPane");
}

void InfoPane::refresh(Item *item) {
	title->value(item->getTitle().c_str());
	buffer->text(item->getDescription().c_str());
}
