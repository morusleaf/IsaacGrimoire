#pragma once

#include <FL/Fl_Scroll.h>
#include <FL/Fl_Output.h> 

class HUDPane : public Fl_Group {
private:
	Fl_Output *attackDamageOut;
	Fl_Output *attackDelayOut;
	Fl_Output *attackRangeOut;
	Fl_Output *shotSpeedOut;
	Fl_Output *shotHeightOut;
	Fl_Output *moveSpeedOut;
	Fl_Output *luckOut;
public:
	HUDPane(int x, int y, int w, int h);
	~HUDPane();
	void refresh(int attackStyle, float attackDamage, int attackDelay, float attackRange,
		float shotSpeed, float shotHeight, float moveSpeed, float luck, float playerX, float playerY);
};
