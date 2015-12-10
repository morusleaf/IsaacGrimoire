#pragma once

#include <FL/Fl_Group.H>
#include "UIFltk/HUDPane.h"
#include "Inquirer.h"

class StatTab : public Fl_Group {
private:
	Inquirer *inquirer;
public:
	HUDPane *hudpane;
	StatTab(int x, int y, int w, int h, const char *title, Inquirer *inquirer);
	~StatTab();
	void refresh();
};
