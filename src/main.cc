#include "util.h"
#include "UIFltk/Layout.h"
#include "UIFltk/SheetWindow.h"
#include <FL/Fl.H>

void timeup(void *p) {
	InquirerPanel *panel = (InquirerPanel*)p;
	try {
		if (panel->value() == panel->itemtab)
			panel->itemtab->refresh();
		else if (panel->value() == panel->stattab)
			panel->stattab->refresh();
	}
    catch (std::exception &e) {
        ErrorLog("uncatched exception in timeup: %s", e.what());
        ALERT(e.what());
    }
	catch (...) {
		ErrorLog("Error Occurerd in timeup");
        ALERT("Unknown exception in timeup");
	}
	Fl::repeat_timeout(0.5, timeup, p);
}

static int gate_handler(int event) {
	if (event == FL_SHORTCUT) return 1;
	return 0;
}

int main(int argc, char* argv[]) {
	SheetWindow sheetwindow(WINDOW_W, WINDOW_H, WINDOW_MENU);
	sheetwindow.show();
	Fl::add_handler(gate_handler);
	Fl::add_timeout(0.5, timeup, sheetwindow.inquirerpanel);
	try {
		return Fl::run();
	}
    catch (std::exception &e) {
        ErrorLog("uncatched exception in main: %s", e.what());
        ALERT(e.what());
    }
	catch (...) {
		ErrorLog("Unknown exception");
        ALERT("Unknown exception in main");
	}
}
