#include "UIFltk/Layout.H"
#include "UIFltk/SheetWindow.H"
#include "util.h"

SheetWindow::SheetWindow(int w, int h, const char *title) :
	Fl_Window(w, h, title) {
	begin();
	inquirerpanel = new InquirerPanel(PANEL_X, PANEL_Y, PANEL_W, PANEL_H);
	gamebutton = new GameButton(GAMEBUTTON_X, GAMEBUTTON_Y, GAMEBUTTON_W, GAMEBUTTON_H, GAMEBUTTON_MENU);
	end();
	resizable(inquirerpanel);
}

SheetWindow::~SheetWindow() {
	UILog("destruct SheetWindow");
}
