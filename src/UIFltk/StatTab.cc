#include "UIFltk/StatTab.H"
#include "UIFltk/Layout.H"
#include "exception/get_task_error.h"
#include "exception/get_process_error.h"
#include "exception/read_memory_error.h"
#include "util.h"

StatTab::StatTab(int x, int y, int w, int h, const char *title, Inquirer *_inquirer) :
	Fl_Group(x, y, w, h, title) {
	inquirer = _inquirer;
	begin();
	hudpane = new HUDPane(HUDPANE_X, HUDPANE_Y, HUDPANE_W, HUDPANE_H);
	end();
}

StatTab::~StatTab() {
	UILog("destruct StatTab");
}

void StatTab::refresh() {
	int attackStyle, attackDelay;
	float attackDamage, attackRange, shotSpeed, shotHeight, moveSpeed, luck, playerX, playerY;
	bool ret = false;
	UILog("refresh StatTab");
	try {
		ret = inquirer->getStat(attackStyle, attackDamage, attackDelay, attackRange, shotSpeed,
			shotHeight, moveSpeed, luck, playerX, playerY);
	}
	catch (get_task_error) {
		ErrorLog("can not use task_for_pid(), try sudo");
        ALERT("can not use task_for_pid()");
		exit(1);
	}
	catch (get_process_error& e) {
		WarningLog("%s", e.what());
		return;
	}
	catch (read_memory_error) {
		WarningLog("error occured during reading memory");
		return;
	}
	if (ret)
		hudpane->refresh(attackStyle, attackDamage, attackDelay, attackRange, shotSpeed,
		shotHeight, moveSpeed, luck, playerX, playerY);
}
