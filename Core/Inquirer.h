#pragma once

#include "Core/CheatSheet.h"
#include "Core/Memory.h"

class Inquirer {
private:
	CheatSheet* cheatsheet;
	Memory* memory;
	mem_address_t base;
	bool beginInquire();
	bool endInquire();
public:
	bool getItemsFromRoom(ItemList* list, ImageList* imglist, bool *pillsDiscovered, int *pillsEffect);
	bool getItemsFromPlayer(ItemList* list, ImageList* imglist, bool *pillsDiscovered, int *pillsEffect);
	bool getPillsTable(bool *discovered, int *effect);
	bool getStat(int &attackStyle, float &attackDamage, int &attackDelay, float &attackRange,
        float &shotSpeed, float &shotHeight, float &moveSpeed, float &luck);
	Inquirer();
	~Inquirer();
};
