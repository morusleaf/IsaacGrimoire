#include "Core/CheatSheet.h"
#include <fstream>
#include "Core/util.h"
#include <QtCore>
#include <stdexcept>
#include <QtDebug>
//#ifdef _WIN32
//#else
//#include <CoreFoundation/CFBundle.h>
//#endif

CheatSheet::CheatSheet(std::string fileName[ItemKindNum], std::string pillsPicName, std::string specialName) {
	for (unsigned i = 0; i<ItemKindNum; i++)
		lists[i] = ItemList();

	for (unsigned i = 0; i<ItemKindNum; i++) {
        createSheetFromJSON(fileName[i], i);
	}

    createPillsImgListFromJSON(pillsPicName);
    createSpecialListFromJSON(specialName);
}

CheatSheet::~CheatSheet() {
	for (int i = 0; i < ItemKindNum; i++)
		for (Item* item : lists[i])
			delete item;
	for (int i = 0; i < ItemKindNum - 1; i++)
		for (ImageType* img : imglists[i])
			delete img;
}

void CheatSheet::createSpecialListFromJSON(std::string filename) {
    specialList.reserve(7);
    specialImgList.reserve(6);
    QFile file;
    file.setFileName(QString::fromStdString(filename));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    // [TODO]: use exception
    if (!file.isOpen())
        qDebug() << "can not open file";
    QString rawdata = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(rawdata.toUtf8());
    // [TODO] use exception
    if (doc.isEmpty())
        qDebug() << "error occurred during reading file";
    QJsonArray arr = doc.array();

    foreach (QJsonValue val, arr) {
        QJsonObject node = val.toObject();
        int id = node["id"].toInt();
        specialList[id] = new Item(node);
        QString imageRelPath = node["image"].toString();
        if (imageRelPath == "") continue;

        specialImgList[id] = new ImageType(":/image/resources/"+imageRelPath);
    }
}

void CheatSheet::createPillsImgListFromJSON(std::string filename) {
    QFile file;
    file.setFileName(QString::fromStdString(filename));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    // [TODO]: use exception
    if (!file.isOpen())
        qDebug() << "can not open file";
    QString rawdata = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(rawdata.toUtf8());
    // [TODO] use exception
    if (doc.isEmpty())
        qDebug() << "error occurred during reading file";
    QJsonArray arr = doc.array();

    foreach (QJsonValue val, arr) {
        QJsonObject node = val.toObject();
        QString imageRelPath = node["image"].toString();
        int index = node["index"].toInt();
        imglists[KIND_PILL][index] = new ImageType(":/image/resources/"+imageRelPath);
    }
}

void CheatSheet::createSheetFromJSON(std::string filename, int kind) {
    QFile file;
    file.setFileName(QString::fromStdString(filename));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    // [TODO]: use exception
    if (!file.isOpen())
        qDebug() << "can not open file";
    QString rawdata = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(rawdata.toUtf8());
    // [TODO] use exception
    if (doc.isEmpty())
        qDebug() << "error occurred during reading file";
    QJsonArray arr = doc.array();

    int maxIndex = -1;
    foreach (QJsonValue val, arr) {
        QJsonObject node = val.toObject();
        if (Item::str2ItemKind(node["kind"].toString()) != kind)
            continue;
        int index = node["id"].toInt();
        if (maxIndex < index)
            maxIndex = index;
    }

    // follow the indexing style (start from 1) in game
    // set the size first to avoid reallocate
    lists[kind].reserve(maxIndex+1);
    for (int i=0; i<=maxIndex; i++)
        lists[kind].push_back(NULL);
    if (kind != KIND_PILL)
        imglists[kind].reserve(maxIndex + 1);
    else
        imglists[kind].reserve(14);

    foreach(QJsonValue val, arr) {
        QJsonObject node = val.toObject();
        if (Item::str2ItemKind(node["kind"].toString()) != kind)
            continue;
        int index = node["id"].toInt();
        lists[kind][index] = new Item(node);
        if (kind != KIND_PILL) {
            QString ImgRelPath = node["image"].toString();
            imglists[kind][index] = new ImageType(":/image/resources/"+ImgRelPath);
        }
    }
}

Item* CheatSheet::getItem(int kind, int index, bool blind) {
	if (kind >= 0 && kind < ItemKindNum)
        if (kind == KIND_COLLECTIBLE && blind)
            return UNKNOWN_ITEM;
        else if (kind == KIND_CARD && blind) {
            int groundId = lists[KIND_CARD][index]->getGroundId();
            // if groundId == 0: Dice Shard, Emergency Contact
            if (groundId == 0) return lists[KIND_CARD][index];
            else return specialList[groundId];
        }
        else if (kind == KIND_PILL) {
			if (blind)
                return UNKNOWN_PILL;
			else
                return lists[KIND_PILL][index];
		}
        else if (index < lists[kind].size() && index > 0)
			return lists[kind][index];
		else
            throw std::range_error("index out of bound");
	else
		ErrorLog("unknown kind %d", kind);
	return NULL;
}

size_t CheatSheet::size(int kind) {
	return lists[kind].size();
}

ImageType* CheatSheet::getImg(int kind, int index, bool blind) {
	if (kind >= 0 && kind < ItemKindNum)
        if (kind == KIND_COLLECTIBLE && blind)
            return UNKNOWN_ITEM_IMG;
        else if (kind == KIND_CARD && blind) {
            int groundId = lists[KIND_CARD][index]->getGroundId();
            // if groundId == 0: Dice Shard, Emergency Contact
            if (groundId == 0) return imglists[KIND_CARD][index];
            else return specialImgList[groundId];
        }
        else if (index < lists[kind].size() && index > 0)
			return imglists[kind][index];
		else
            throw std::range_error("index out of bound");
	else
		ErrorLog("unknown kind %d", kind);
	return NULL;
}
