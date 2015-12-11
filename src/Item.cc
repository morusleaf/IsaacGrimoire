#include "Item.h"
#include "util.h"

Item::Item(Json::Value node) {
	kind = str2ItemKind(node["kind"].asString());
	id = node["sid"].asInt();
	title = node["title"].asString();
	/*if (kind != Pill) {
		pickup = node["pickup"].asString();
		imagePath = TOP + node["image"].asString();
		image = new ImageType(imagePath.c_str());
	}*/
	std::stringstream stream;
	Json::Value descriptions = node["description"];
	// link every line to a string
	for (int i = 0; i<descriptions.size(); i++) {
		if (i != 0) stream << "\n";
		stream << descriptions[i].asString();
	}
	description = stream.str();
    groundId = node.get("groundid", 0).asInt();
    /* int _groundid = node.get("groundid", -1).asInt(); */
    /* if (_groundid != -1) groundid = _groundid; */
	/* if (kind == Card) groundId = node["groundid"].asInt(); */
}

int Item::getId() {
	return id;
}

std::string Item::getTitle() {
	return title;
}

//std::string Item::getPickup() {
//	return pickup;
//}

std::string Item::getDescription() {
	return description;
}

ItemKind Item::getKind() {
	return kind;
}

//ImageType* Item::getImage() {
//	return image;
//}
//
//void Item::setImage(ImageType *_image) {
//	image = _image;
//}

int Item::getGroundId() {
	return groundId;
}

Item::~Item() { }

ItemKind Item::str2ItemKind(std::string text) {
	if (text == "collectible") return Collectible;
	else if (text == "trinket") return Trinket;
	else if (text == "pill") return Pill;
	else if (text == "card") return Card;
	else return UnknownItem;
}
