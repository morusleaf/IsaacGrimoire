#pragma once

#include <FL/Fl_Output.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include "Item.h"

class InfoPane : public Fl_Group {
private:
	Fl_Output *title;
	Fl_Text_Display *description;
	Fl_Text_Buffer *buffer;
public:
	InfoPane(int x, int y, int w, int h);
	~InfoPane();
	void refresh(Item *item);
};
