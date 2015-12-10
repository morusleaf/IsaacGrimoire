#include "UIFltk/HUDPane.H"
#include "UIFltk/Layout.H"
#include "util.h"

HUDPane::HUDPane(int x, int y, int w, int h) :
	Fl_Group(x, y, w, h) {
	begin();
	attackDamageOut = new Fl_Output(STATE_X, STATE_Y(0), STATE_W, STATE_H, ATTACKDAMAGE_MENU);
	attackDelayOut = new Fl_Output(STATE_X, STATE_Y(1), STATE_W, STATE_H, ATTACKDELAY_MENU);
	attackRangeOut = new Fl_Output(STATE_X, STATE_Y(2), STATE_W, STATE_H, ATTACKRANGE_MENU);
	shotSpeedOut = new Fl_Output(STATE_X, STATE_Y(3), STATE_W, STATE_H, SHOTSPEED_MENU);
	shotHeightOut = new Fl_Output(STATE_X, STATE_Y(4), STATE_W, STATE_H, SHOTHEIGHT_MENU);
	moveSpeedOut = new Fl_Output(STATE_X, STATE_Y(5), STATE_W, STATE_H, MOVESPEED_MENU);
	luckOut = new Fl_Output(STATE_X, STATE_Y(6), STATE_W, STATE_H, LUCK_MENU);
	end();
	attackDamageOut->cursor_color(FL_WHITE);
	attackDelayOut->cursor_color(FL_WHITE);
	attackRangeOut->cursor_color(FL_WHITE);
	shotSpeedOut->cursor_color(FL_WHITE);
	shotHeightOut->cursor_color(FL_WHITE);
	moveSpeedOut->cursor_color(FL_WHITE);
	luckOut->cursor_color(FL_WHITE);
}

HUDPane::~HUDPane() {
	UILog("destruct HUDPane");
}

void HUDPane::refresh(int attackStyle, float attackDamage, int attackDelay, float attackRange,
	float shotSpeed, float shotHeight, float moveSpeed, float luck, float playerX, float playerY) {
	char buffer[32];
	// Fl_Output.value(arg) will copy arg to internal buffer,
	// so we can alloc buffer on stack
	snprintf(buffer, 32, "%f", attackDamage); attackDamageOut->value(buffer);
	snprintf(buffer, 32, "%d", attackDelay); attackDelayOut->value(buffer);
	snprintf(buffer, 32, "%f", attackRange); attackRangeOut->value(buffer);
	snprintf(buffer, 32, "%f", shotSpeed); shotSpeedOut->value(buffer);
	snprintf(buffer, 32, "%f", shotHeight); shotHeightOut->value(buffer);
	snprintf(buffer, 32, "%f", moveSpeed); moveSpeedOut->value(buffer);
	snprintf(buffer, 32, "%f", luck); luckOut->value(buffer);
}
