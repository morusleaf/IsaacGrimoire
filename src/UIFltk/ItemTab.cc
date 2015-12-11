#include "UIFltk/Layout.H"
#include "UIFltk/ItemTab.H"
#include "exception/get_task_error.h"
#include "exception/get_process_error.h"
#include "exception/read_memory_error.h"
#include "util.h"

ItemTab::ItemTab(int x, int y, int w, int h, const char *title, Inquirer *_inquirer) :
	Fl_Group(x, y, w, h, title) {
	inquirer = _inquirer;
	lastRoomItems = new ItemList();
	lastPlayerItems = new ItemList();
	begin();
	infopane = new InfoPane(INFOPANE_X, INFOPANE_Y, INFOPANE_W, INFOPANE_H);
	itempane = new ItemPane(ITEMPANE_X, ITEMPANE_Y, ITEMPANE_W, ITEMPANE_H, infopane);
	end();
	resizable(itempane);
}

ItemTab::~ItemTab() {
	delete lastRoomItems;
	delete lastPlayerItems;
}

// assert v1 is the new list, v2 is the last list
// if v1 == v2 return -1
// else return the index of v1's different item (collectible has priority)
//static int listDiff(ItemList *v1, ItemList *v2) {
//	if (v1 == NULL) return -1;
//	if (v2 == NULL) {
//		int ret = 0;
//		// TODO: use iterator
//		for (unsigned i = 0; i < v1->size(); i++)
//			if (v1->at(i)->getKind() == Collectible) return i;
//		return ret;
//	}
//	int ret = 0;
//	int i = 0, j = 0;
//	while (i < v1->size() && j < v2->size()) {
//		if (v1 == v2) continue;
//	}
//}

static bool listEqual(ItemList* v1, ItemList* v2) {
	if (v1 == NULL || v2 == NULL) return false;
	if (v1->size() != v2->size()) return false;
	unsigned int n = v1->size();
	return std::equal(v1->begin(), v1->begin() + n, v2->begin());
}

void ItemTab::refresh() {
	UILog("refresh ItemTab");
	ItemList *newRoomItems = new ItemList();
	ItemList *newPlayerItems = new ItemList();
	ImageList roomImgs = ImageList();
	ImageList playerImgs = ImageList();
	bool pillsDiscovered[13];
	int pillsEffect[13];
	bool ret = false;
	try {
		ret = inquirer->getPillsTable(pillsDiscovered, pillsEffect);
		if (ret)
			ret = inquirer->getItemsFromRoom(newRoomItems, &roomImgs, pillsDiscovered, pillsEffect);
		if (ret)
			ret = inquirer->getItemsFromPlayer(newPlayerItems, &playerImgs, pillsDiscovered, pillsEffect);
	}
	catch (get_task_error) {
		ErrorLog("can not use task_for_pid(), try sudo");
        ALERT("can not use task_for_pid()");
		delete newRoomItems;
		delete newPlayerItems;
		exit(1);
	}
	catch (get_process_error& e) {
		WarningLog("%s", e.what());
		delete newRoomItems;
		delete newPlayerItems;
		return;
	}
	catch (read_memory_error) {
		WarningLog("error occured during reading memory");
		delete newRoomItems;
		delete newPlayerItems;
		return;
	}
	if (!ret) return;

	// If items is not changed, no need to refresh
	bool roomListNotChanged = listEqual(newRoomItems, lastRoomItems);
	if (roomListNotChanged && listEqual(newPlayerItems, lastPlayerItems)) {
		delete newRoomItems;
		delete newPlayerItems;
		return;
	}
	delete lastRoomItems;
	delete lastPlayerItems;
	lastRoomItems = newRoomItems;
	lastPlayerItems = newPlayerItems;
    // roomListNotChanged == true: show 1st item in roomList
    // roomListNotChanged == false:show 1st item in playerList
	itempane->refresh(lastRoomItems, lastPlayerItems, &roomImgs, &playerImgs, !roomListNotChanged);
}
