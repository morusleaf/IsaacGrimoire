#include "UIFltk/GameButton.h"
#include <stdlib.h>

static void gamebutton_callback(Fl_Widget *w, void*) {
#ifdef _WIN64
	system("start steam://rungameid/250900");
#else
    system("open steam://rungameid/250900");
#endif
}

GameButton::GameButton(int x, int y, int w, int h, const char *title) :
	Fl_Button(x, y, w, h, title) {
	callback(gamebutton_callback);
}

GameButton::~GameButton() {}
