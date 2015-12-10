#include "UIFltk/InquirerPanel.H"
#include "UIFltk/Layout.H"
#include <FL/Fl.H>
#include "util.h"
#include "Inquirer.h"
#include <fstream>

InquirerPanel::InquirerPanel(int x, int y, int w, int h) :
	Fl_Tabs(x, y, w, h) {
	begin();
	try {
		inquirer = new Inquirer();
		itemtab = new ItemTab(TAB_X, TAB_Y, TAB_W, TAB_H, ITEMS_MENU, inquirer);
		stattab = new StatTab(TAB_X, TAB_Y, TAB_W, TAB_H, STATS_MENU, inquirer);
	}
	catch (std::ifstream::failure& e) {
		ErrorLog("Can not open/read/close JSON/JS file");
		exit(1);
	}
	catch (Json::RuntimeError& e) {
		ErrorLog("Read JSON/JS file error");
		exit(1);
	}
	end();
	resizable(itemtab);
}

InquirerPanel::~InquirerPanel() {
	delete inquirer;
	UILog("destruct InquirerPanel");
}
