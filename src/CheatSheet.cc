#include "CheatSheet.h"
#include <fstream>
#include "util.h"
#include <stdexcept>
#include <CoreFoundation/CFBundle.h>

static bool endwith(std::string const &value, std::string const &ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

CheatSheet::CheatSheet(std::string fileName[ItemKindNum], std::string pillsPicName) {
#ifdef _WIN64
#else
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef bundleURL = CFBundleCopyBundleURL(mainBundle);
    CFURLGetFileSystemRepresentation(bundleURL, true, (UInt8*)rootpath, MAX_PATH_LENGTH);
    // now we get a path like /Users/.../xxx.app
    pFileName = rootpath;
    // reach the end of path
    while (*pFileName != '\0') pFileName++;
    // remove xxx.app
    while (*pFileName != '/') pFileName--;
    pFileName++;
    *pFileName = '\0';
#endif
	for (unsigned i = 0; i<ItemKindNum; i++)
		lists[i] = ItemList();

	for (unsigned i = 0; i<ItemKindNum; i++) {
		std::ifstream *ifs = new std::ifstream();
		ifs->exceptions(std::ifstream::failbit | std::ifstream::badbit);
        // append fileanme to the end of rootpath
        // now rootpath contains absolute path of the file
        strcpy(pFileName, fileName[i].c_str());
        ifs->open(rootpath);
		/* ifs->open(fileName[i]); */
		createSheetFromJS(ifs, (ItemKind)i);
		ifs->close();
		delete ifs;
	}

	std::ifstream *ifs = new std::ifstream();
	ifs->exceptions(std::ifstream::failbit | std::ifstream::badbit);
    strcpy(pFileName, pillsPicName.c_str());
	ifs->open(rootpath);
	createPillsImgListFromJSON(ifs);
	ifs->close();
	delete ifs;
}

CheatSheet::~CheatSheet() {
	for (int i = 0; i < ItemKindNum; i++)
		for (Item* item : lists[i])
			delete item;
	for (int i = 0; i < ItemKindNum - 1; i++)
		for (ImageType* img : imglists[i])
			delete img;
}

void CheatSheet::createPillsImgListFromJSON(std::ifstream *ifs) {
	Json::Value root;
	(*ifs) >> root;
	for (Json::Value node : root) {
		std::string imageRelPath = node["image"].asString();
		int index = node["index"].asInt();
        strcpy(pFileName, imageRelPath.c_str());
		imglists[Pill][index] = new ImageType(rootpath);
	}
}

// I am actually using JS file rather than JSON file.
// Since it is also used by an HTML and I can not find a convenient way to
// read local JSON file in HTML, so I added an assignment at the beginning.
void CheatSheet::createSheetFromJS(std::ifstream *ifs, ItemKind kind) {
	char* buffer = (char*)malloc(256);
	ifs->getline(buffer, 256);
	createSheetFromJSON(ifs, kind);
	FREE(buffer);
}

void CheatSheet::createSheetFromJSON(std::ifstream *ifs, ItemKind kind) {
	Json::Value root;
	(*ifs) >> root;

	int maxIndex = -1;
	for (Json::Value node : root) {
		std::string text = node["kind"].asString();
		if (Item::str2ItemKind(text) != kind) continue;
		int index = node["id"].asInt();
		if (maxIndex < index)
			maxIndex = index;
	}

	// follow the indexing style (start from 1) in game
	// set the size first to avoid reallocate
	lists[kind].reserve(maxIndex + 1);
	for (int i = 0; i <= maxIndex; i++)
		lists[kind].push_back(NULL);
	if (kind != Pill)
		imglists[kind].reserve(maxIndex + 1);
	else
		imglists[kind].reserve(14);

	for (Json::Value node : root) {
		std::string text = node["kind"].asString();
		if (Item::str2ItemKind(text) != kind) continue;
		int index = node["id"].asInt();
		lists[kind][index] = new Item(node);
		if (kind != Pill) {
            std::string ImgRelPath = node["image"].asString();
            strcpy(pFileName, ImgRelPath.c_str());
			imglists[kind][index] = new ImageType(rootpath);
        }
	}
}

Item* CheatSheet::getItem(ItemKind kind, int index, bool blind) {
	if (kind >= 0 && kind < ItemKindNum)
		if (kind == Collectible && blind)
			return lists[Collectible][lists[Collectible].size() - 1];
		else if (kind == Card && blind)
			return lists[kind][lists[kind][index]->getGroundId()];
		else if (kind == Pill) {
			if (blind)
				return lists[kind][lists[Pill].size() - 1];
			else
				return lists[kind][index];
		}
		else if (index < lists[kind].size() && index > 0)
			return lists[kind][index];
		else
			throw std::range_error("index " + std::to_string(index) + " out of bound");
	else
		ErrorLog("unknown kind %d", kind);
	return NULL;
}

size_t CheatSheet::size(ItemKind kind) {
	return lists[kind].size();
}

ImageType* CheatSheet::getImg(ItemKind kind, int index, bool blind) {
	if (kind >= 0 && kind < ItemKindNum)
		if (kind == Collectible && blind)
			return imglists[Collectible][lists[Collectible].size() - 1];
		else if (kind == Card && blind)
			return imglists[kind][lists[kind][index]->getGroundId()];
		else if (index < lists[kind].size() && index > 0)
			return imglists[kind][index];
		else
			throw std::range_error("index " + std::to_string(index) + " out of bound");
	else
		ErrorLog("unknown kind %d", kind);
	return NULL;
}