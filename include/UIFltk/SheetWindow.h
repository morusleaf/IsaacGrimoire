#pragma once

#include "UIFltk/InquirerPanel.h"
#include "UIFltk/GAMEButton.h"
#include <FL/Fl_Window.H>

class SheetWindow : public Fl_Window {
private:
public:
	InquirerPanel *inquirerpanel;
	GameButton *gamebutton;
	SheetWindow(int w, int h, const char *title);
	~SheetWindow();
};
