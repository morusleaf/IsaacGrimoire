#pragma once

#include <string>
#include <vector>
#include "json/json.h"

#define MY_IMG 0
#define FL_PNG 1
#define IMGTYPE FL_PNG

#if IMGTYPE == FL_PNG
#define ImageType Fl_PNG_Image
#include <FL/Fl_PNG_Image.H>
#else
// TODO: put a proper class here
#define ImageType MyImage
class MyImage {
private:
	std::string _imagePath;
public:
	MyImage(std::string imagePath) { _imagePath = imagePath; };
	std::string image() { return _imagePath; };
	~MyImage();
};
#endif

enum ItemKind {
	Collectible = 0,
	Trinket = 1,
	Card = 2,
	Pill = 3,
	UnknownItem = 4
};

#define ItemKindNum 4

class Item {
private:
	int id;
	// item's name
	std::string title;
	// line shown upon picked up
	//std::string pickup;
	std::string description;
	//std::string imagePath;
	//ImageType* image;
	int groundId;
	ItemKind kind;
public:
	Item(Json::Value node);
	int getId();
	std::string getTitle();
	//std::string getPickup();
	std::string getDescription();
	ItemKind getKind();
	//ImageType* getImage();
	int getGroundId();
	//void setImage(ImageType *_image);
	static ItemKind str2ItemKind(std::string);
	~Item();
};

typedef std::vector<Item*> ItemList;
typedef std::vector<ImageType*> ImageList;
/* template <class ImageType> using ItemList = std::vector<Item<ImageType>*>; */
