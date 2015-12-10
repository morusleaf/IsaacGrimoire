#pragma once

#include <FL/Fl_Button.H>

class GameButton : public Fl_Button {
public:
	GameButton(int x, int y, int w, int h, const char *title);
	~GameButton();
};