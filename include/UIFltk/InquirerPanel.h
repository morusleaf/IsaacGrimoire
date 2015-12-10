#pragma once

#include <FL/Fl_Tabs.H>
#include "UIFltk/ItemTab.h"
#include "UIFltk/StatTab.h"
#include "Inquirer.h"

class InquirerPanel : public Fl_Tabs {
private:
	Inquirer *inquirer;
public:
	ItemTab *itemtab;
	StatTab *stattab;
	InquirerPanel(int x, int y, int w, int h);
	~InquirerPanel();
};
