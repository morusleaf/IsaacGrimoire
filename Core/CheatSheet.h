#pragma once

#include "Core/Item.h"
#define MAX_PATH_LENGTH 1024

class CheatSheet {
private:
	ItemList lists[ItemKindNum];
    ItemList specialList;
#define UNKNOWN_ITEM specialList[5]
#define UNKNOWN_PILL specialList[6]
	std::vector<ImageType*> imglists[ItemKindNum];
#define UNKNOWN_ITEM_IMG specialImgList[5]
    std::vector<ImageType*> specialImgList;
//    void createSheetFromJS(std::string filename, int kind);
    void createSheetFromJSON(std::string filename, int kind);
    void createPillsImgListFromJSON(std::string filename);
    void createSpecialListFromJSON(std::string filename);
    //char rootpath[MAX_PATH_LENGTH] = "./";
    //char *pFileName = rootpath+2;
public:
	CheatSheet(std::string fileName[ItemKindNum], std::string pillsPicName,
            std::string specialName);
	~CheatSheet();
    Item* getItem(int type, int index, bool blind=false);
    ImageType* getImg(int kind, int index, bool blind=false);
    size_t size(int kind);
};
