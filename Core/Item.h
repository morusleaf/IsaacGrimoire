#pragma once

#include <string>
#include <vector>
#include <QJsonObject>
#include <QtCore>
#include "Core/util.h"

#define MY_IMG 0
#define FL_PNG 1
#define QT_IMG 2
#define IMGTYPE QT_IMG

#if IMGTYPE == QT_IMG
#define ImageType QImage
#include <QImage>
#elif IMGTYPE == FL_PNG
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

#define KIND_COLLECTIBLE 0
#define KIND_TRINKET     1
#define KIND_CARD        2
#define KIND_PILL        3
#define KIND_UNKNOWN     4

#define ItemKindNum 4

class Item {
private:
	int id;
    QString title;
    QString description[LANGUAGES_NUM];
	int groundId;
    int kind;
public:
    Item(QJsonObject node);
	int getId();
    QString getTitle();
    QString getDescription(int Language = ENGLISH);
    int getKind();
	int getGroundId();
    static int str2ItemKind(std::string);
    static int str2ItemKind(QString);
	~Item();
};

typedef std::vector<Item*> ItemList;
typedef std::vector<ImageType*> ImageList;
