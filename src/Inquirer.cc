#include "Inquirer.h"
#include <iostream>
#include "exception/read_memory_error.h"
#include "exception/not_in_game.h"
#include <fstream>
#include <json/json.h>
#include "util.h"

Inquirer::Inquirer() {
	memory = new Memory();
	std::string fileName[ItemKindNum] = {
		"collectibles.js",
		"trinkets.js",
		"cards.js",
		"pills.js"
	};
	cheatsheet = new CheatSheet(fileName, "pillspic.json");
}

Inquirer::~Inquirer() {
	delete memory;
	delete cheatsheet;
}

#ifdef _WIN64
#define PROCNAME "Binding of Isaac: Afterbirth"
#else
#define PROCNAME "The Binding of I"
#endif
bool Inquirer::beginInquire() {
	// TODO: use accurate name
	if (!memory->connect(PROCNAME)) {
		ErrorLog("fail to prepare inquire");
		return false;
	}
	try {
		base = memory->getBase();
	}
	catch (read_memory_error) {
		memory->disconnect();
		throw;
	}
	return true;
}

bool Inquirer::endInquire() {
	if (!memory->disconnect()) {
		ErrorLog("fail to end inquire");
		return false;
	}
	return true;
}

bool Inquirer::getPillsTable(bool *discovered, int *effect) {
	if (!beginInquire()) {
		ErrorLog("can not get items from room");
		return false;
	}
	uint8_t *discoverBuff = (uint8_t*)malloc(13);
	uint8_t *effectBuff = (uint8_t*)malloc(4 * 13);
	uint32_t *p32;
	bool ret = true;
	
	try
	{
#ifdef _WIN64
		mem_address_t pManager = memory->readAddr(base + 0x2E4634);
#else
        mem_address_t pManager = memory->readAddr(base + 0x30AF30);
#endif
		data_t floorNum = memory->readData(pManager);
		if (floorNum == 0) throw not_in_game();

#ifdef _WIN64
		mem_address_t discoverAddr = pManager + 0x9869;
		mem_address_t effectAddr = pManager + 0x9834;
#else
        mem_address_t discoverAddr = pManager + 0xB4F1;
        mem_address_t effectAddr = pManager + 0xB4BC;
#endif

		memory->read(discoverAddr, 13, discoverBuff, NULL);
		memory->read(effectAddr, 13 * sizeof(data_t), effectBuff, NULL);
		p32 = (data_t*)effectBuff;
		for (int i = 0; i < 13; i++) {
			discovered[i] = (bool)discoverBuff[i];
			effect[i] = p32[i];
			//Log("%d %d", discovered[i], effect[i]);
		}
	}
	catch (read_memory_error) {
		memory->disconnect();
		FREE(discoverBuff);
		FREE(effectBuff);
		throw;
	}
	catch (std::range_error& e) {
		ErrorLog("%s", e.what());
		ret = false;
	}
	catch (not_in_game) {
		WarningLog("Not in game");
		ret = false;
	}

	FREE(discoverBuff);
	FREE(effectBuff);

	if (!endInquire()) {
		ErrorLog("error occured after inquire");
		return false;
	}
	return ret;
}

bool Inquirer::getItemsFromRoom(ItemList* list, ImageList* imglist, bool *pillsDiscovered, int *pillsEffect) {
	if (!beginInquire()) {
		ErrorLog("can not get items from room");
		return false;
	}

	uint8_t *buff = (uint8_t*)malloc(12);
	uint32_t *p32;
	bool ret = true;
	try
	{
#ifdef _WIN64
		mem_address_t pManager = memory->readAddr(base + 0x2E4634);
#else
        mem_address_t pManager = memory->readAddr(base + 0x30AF30);
#endif
		data_t floorNum = memory->readData(pManager);
		if (floorNum == 0) throw not_in_game();

		uint32_t curse = memory->readu32(pManager + 0xC);
		uint32_t blindMask = 0x40;
		bool blind = (curse & blindMask) != 0;
		//Log("%X %X %d", curse, blindMask, blind);

#ifdef _WIN64
		mem_address_t pe1 = memory->readAddr(pManager + 0x7018);
		mem_address_t pe2 = pe1 + 0xE9C;
		mem_address_t pEntityList = memory->readAddr(pe2 + 0x50);
		uint32_t EntityListLength = memory->readu32(pe2 + 0x48);
#else
        mem_address_t pe1 = memory->readAddr(pManager + 0x8BF8);
        mem_address_t pe2 = pe1 + 0x15D0;
        mem_address_t pEntityList = memory->readAddr(pe2 + 0x40);
        uint32_t EntityListLength = memory->readu32(pe2 + 0x4C);
#endif

		for (unsigned index = 0; index < EntityListLength; index++) {
			mem_address_t pEntity = memory->readAddr(pEntityList + index * sizeof(mem_address_t));
#ifdef _WIN64
			memory->read(pEntity + 0x24, 12, buff, NULL);
#else
            memory->read(pEntity + 0x30, 12, buff, NULL);
#endif
			p32 = (uint32_t*)buff;
			uint32_t item_no0 = p32[0];
			uint32_t item_no1 = p32[1];
			uint32_t item_no2 = p32[2];
			//Log("%d %d %d", item_no0, item_no1, item_no2);
			if (item_no0 == 5 && item_no2 != 0 &&
				(item_no1 == 100 || item_no1 == 350 || item_no1 == 300 || item_no1 == 70)) {
				switch (item_no1) {
				case 100:
					list->push_back(cheatsheet->getItem(Collectible, item_no2, blind));
					imglist->push_back(cheatsheet->getImg(Collectible, item_no2, blind));
					break;
				case 350:
					list->push_back(cheatsheet->getItem(Trinket, item_no2));
					imglist->push_back(cheatsheet->getImg(Trinket, item_no2));
					break;
				case 300:
					list->push_back(cheatsheet->getItem(Card, item_no2, true));
					imglist->push_back(cheatsheet->getImg(Card, item_no2, true));
					break;
				case 70:
					list->push_back(cheatsheet->getItem(Pill, pillsEffect[item_no2-1], !pillsDiscovered[item_no2-1]));
					imglist->push_back(cheatsheet->getImg(Pill, item_no2, !pillsDiscovered[item_no2-1]));
					break;
				}
			}
		}
	}
	catch (read_memory_error) {
		memory->disconnect();
		FREE(buff);
		throw;
	}
	catch (std::range_error& e) {
		ErrorLog("%s", e.what());
		ret = false;
	}
	catch (not_in_game) {
		WarningLog("Not in game");
		ret = false;
	}

	FREE(buff);

	if (!endInquire()) {
		ErrorLog("error occured after inquire");
		return false;
	}
	return ret;
}

bool Inquirer::getItemsFromPlayer(ItemList* list, ImageList* imglist, bool *pillsDiscovered, int *pillsEffect) {
	if (!beginInquire()) {
		ErrorLog("can not get items from room");
		return false;
	}
	
	uint8_t *buff = NULL;
	bool ret = true;
	try
	{
#ifdef _WIN64
		mem_address_t pManager = memory->readAddr(base + 0x2E4634);
#else
        mem_address_t pManager = memory->readAddr(base + 0x30AF30);
#endif
		data_t floorNum = memory->readData(pManager);
		if (floorNum == 0) throw not_in_game();

#ifdef _WIN64
		mem_address_t p2 = memory->readAddr(pManger + 0xB7D8);
		mem_address_t pPlayer = memory->readAddr(p2);
		data_t trinketNum1 = memory->readData(pPlayer + 0x1D9C);
		data_t trinketNum2 = memory->readData(pPlayer + 0x1DA0);
#else
        mem_address_t p2 = memory->readAddr(pManager + 0xD2A0);
        mem_address_t pPlayer = memory->readAddr(p2);
        data_t trinketNum1 = memory->readData(pPlayer + 0x1F20);
        data_t trinketNum2 = memory->readData(pPlayer + 0x1F24);
#endif

		if (trinketNum1 != 0) {
			list->push_back(cheatsheet->getItem(Trinket, trinketNum1));
			imglist->push_back(cheatsheet->getImg(Trinket, trinketNum1));
		}
		if (trinketNum2 != 0) {
			list->push_back(cheatsheet->getItem(Trinket, trinketNum2));
			imglist->push_back(cheatsheet->getImg(Trinket, trinketNum2));
		}

#ifdef _WIN64
		mem_address_t pTable = pPlayer + 0x1DA4;
#else
        mem_address_t pTable = pPlayer + 0x1F28;
#endif
		int tablesize = cheatsheet->size(Collectible);
		//Log("tablesize = %d", tablesize);
		buff = (uint8_t*)malloc(sizeof(data_t) * tablesize);
		memory->read(pTable, sizeof(data_t) * tablesize, buff, NULL);
		data_t* table = (data_t*)buff;
		for (int i = 0; i < tablesize; i++)
			if (table[i] != 0) {
				list->push_back(cheatsheet->getItem(Collectible, i));
				imglist->push_back(cheatsheet->getImg(Collectible, i));
			}
		//Log("%lu", list->size());
		bool havePHD = (bool)table[75];

#ifdef _WIN64
		data_t cardpillNum1 = memory->readData(pPlayer + 0x4BE0);
		data_t cardpillNum2 = memory->readData(pPlayer + 0x4BE8);
		data_t cardpillType1 = memory->readData(pPlayer + 0x4BE4) & 1;
		data_t cardpillType2 = memory->readData(pPlayer + 0x4BEC) & 1;
#else
		data_t cardpillNum1 = memory->readData(pPlayer + 0x4D64);
		data_t cardpillNum2 = memory->readData(pPlayer + 0x4D6C);
		data_t cardpillType1 = memory->readData(pPlayer + 0x4D68) & 1;
		data_t cardpillType2 = memory->readData(pPlayer + 0x4D70) & 1;
#endif
		// cardpillType == 0: pill
		// cardpillType == 1: card
		if (cardpillNum1) {
			if (cardpillType1) {
				list->push_back(cheatsheet->getItem(Card, cardpillNum1));
				imglist->push_back(cheatsheet->getImg(Card, cardpillNum1));
			}
			else {
				list->push_back(cheatsheet->getItem(Pill, pillsEffect[cardpillNum1-1], !havePHD && !pillsDiscovered[cardpillNum1-1]));
				imglist->push_back(cheatsheet->getImg(Pill, cardpillNum1));
			}
		}
		if (cardpillNum2) {
			if (cardpillType2) {
				list->push_back(cheatsheet->getItem(Card, cardpillNum2));
				imglist->push_back(cheatsheet->getImg(Card, cardpillNum2));
			}
			else {
				list->push_back(cheatsheet->getItem(Pill, pillsEffect[cardpillNum2-1], !havePHD && !pillsDiscovered[cardpillNum2-1]));
				imglist->push_back(cheatsheet->getImg(Pill, cardpillNum2));
			}
		}
	}
	catch (read_memory_error) {
		memory->disconnect();
		FREE(buff);
		throw;
	}
	catch (std::range_error& e) {
		ErrorLog("%s", e.what());
		ret = false;
	}
	catch (not_in_game) {
		WarningLog("Not in game");
		ret = false;
	}

	FREE(buff);

	if (!endInquire()) {
		ErrorLog("error occured after inquire");
		return false;
	}

	return ret;
}

bool Inquirer::getStat(int &attackStyle, float &attackDamage, int &attackDelay, float &attackRange,
	float &shotSpeed, float &shotHeight, float &moveSpeed, float &luck, float &playerX, float &playerY) {
	if (!beginInquire()) {
		ErrorLog("can not get items from room");
		return false;
	}

	bool ret = true;
	try {
#ifdef _WIN64
		mem_address_t pManager = memory->readAddr(base + 0x2E4634);
#else
        mem_address_t pManager = memory->readAddr(base + 0x30AF30);
#endif
		data_t floorNum = memory->readData(pManager);
		if (floorNum == 0) throw not_in_game();

#ifdef _WIN64
		mem_address_t p2 = memory->readAddr(pManager + 0xB7D8);
		mem_address_t pPlayer = memory->readAddr(p2);
#else
        mem_address_t p2 = memory->readAddr(pManager + 0xD2A0);
        mem_address_t pPlayer = memory->readAddr(p2);
#endif

#ifdef _WIN64
		data_t rawPlayerX     = memory->readData(pPlayer + 0x055C);
		data_t rawPlayerY     = memory->readData(pPlayer + 0x0560);
		data_t rawAttackDelay = memory->readData(pPlayer + 0x1C00);
		data_t rawShotSpeed   = memory->readData(pPlayer + 0x1C04);
		data_t rawAttackDamage= memory->readData(pPlayer + 0x1C10);
		data_t rawAttackRange = memory->readData(pPlayer + 0x1C14);
		data_t rawShotHeight  = memory->readData(pPlayer + 0x1C18);
		data_t rawAttackStyle = memory->readData(pPlayer + 0x1C22);
		data_t rawMoveSpeed   = memory->readData(pPlayer + 0x1CDC);
		data_t rawLuck        = memory->readData(pPlayer + 0x1CE0);
#else
        // TODO: I have not find coordinate :-)
        data_t rawPlayerX     = 0;
        data_t rawPlayerY     = 0;
        data_t rawAttackDelay = memory->readData(pPlayer + 0x1D68);
        data_t rawShotSpeed   = memory->readData(pPlayer + 0x1D6C);
        data_t rawAttackDamage= memory->readData(pPlayer + 0x1D78);
        data_t rawAttackRange = memory->readData(pPlayer + 0x1D7C);
        data_t rawShotHeight  = memory->readData(pPlayer + 0x1D80);
        data_t rawAttackStyle = memory->readData(pPlayer + 0x1D8A);
        data_t rawMoveSpeed   = memory->readData(pPlayer + 0x1E58);
        data_t rawLuck        = memory->readData(pPlayer + 0x1E5C);
#endif

		attackStyle = rawAttackStyle;
		attackDamage = *((float*)&rawAttackDamage);
		attackDelay = rawAttackDelay;
		attackRange = *((float*)&rawAttackRange);
		shotSpeed = *((float*)&rawShotSpeed);
		shotHeight = *((float*)&rawShotHeight);
		moveSpeed = *((float*)&rawMoveSpeed);
		luck = *((float*)&rawLuck);
		playerX = *((float*)&rawPlayerX);
		playerY = *((float*)&rawPlayerY);
	}
	catch (read_memory_error) {
		Log("HERE");
		memory->disconnect();
		throw;
	}
	catch (not_in_game) {
		WarningLog("Not in game");
		ret = false;
	}
	/*catch (std::range_error& e) {
		memory->disconnect();
		ErrorLog("%s", e.what());
		list->clear();
		return false;
	}*/
	if (!endInquire()) {
		ErrorLog("error occured after inquire");
		return false;
	}

	return ret;
}

void Inquirer::printItemList(ItemList* list) {
	for (Item* item : *list) {
		switch (item->getKind()) {
		case Collectible: std::cout << "\033[34m"; break;
		case Trinket:     std::cout << "\033[33m"; break;
		case Pill:        std::cout << "\033[31m"; break;
		case Card:        std::cout << "\033[37m"; break;
		case UnknownItem: ErrorLog("Unknown kind"); continue;
		}
		std::cout << item->getTitle() << "\033[0m" << std::endl;
		std::cout << item->getDescription() << std::endl;
		/* std::cout << item->getImage()->image() << std::endl; */
	}
	std::cout << std::endl;
}

void Inquirer::mainLoop() {
	/*ItemList* list = new ItemList();
	std::string input;
	do {
		std::cout << "> ";
		std::cin >> input;
		if (input == "room") {
			getItemsFromRoom(list);
			printItemList(list);
		}
		else if (input == "player") {
			getItemsFromPlayer(list);
			printItemList(list);
		}
		else if (input != "exit") {
			std::cout << "you can input room/player/exit" << std::endl;
			std::cout << "room   - get item list inside the room" << std::endl;
			std::cout << "player - get item list from player" << std::endl;
			std::cout << "exit   - exit the program" << std::endl;
			std::cout << std::endl;
		}
	} while (input != "exit");
	delete list;*/
}
