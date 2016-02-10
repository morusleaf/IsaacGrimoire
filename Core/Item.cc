#include "Core/Item.h"
#include "Core/util.h"
#include <QtCore>
#include <QDebug>

Item::Item(QJsonObject node) {
    kind = str2ItemKind(node["kind"].toString());
    id = node["id"].toInt();
    title = node["title"].toString();
    QJsonArray descriptionArray = node["description"].toArray();
    for (int i=0; i<LANGUAGES_NUM; i++) {
        QJsonArray descriptions = descriptionArray[i].toArray();
        // link every line to a string
        bool flag = false;
        description[i] = "";
        foreach (QJsonValue val, descriptions) {
            if (flag) description[i] += "\n";
            else flag = true;
            description[i] += val.toString();
        }
    }
    // if node has no groundid, it will be set to 0
    groundId = node["groundid"].toInt();
}

int Item::getId() {
	return id;
}

QString Item::getTitle() {
	return title;
}

QString Item::getDescription(int language) {
    return description[language];
}

int Item::getKind() {
	return kind;
}

int Item::getGroundId() {
	return groundId;
}

Item::~Item() { }

int Item::str2ItemKind(std::string text) {
    if (text == "collectible") return KIND_COLLECTIBLE;
    else if (text == "trinket") return KIND_TRINKET;
    else if (text == "pill") return KIND_PILL;
    else if (text == "card") return KIND_CARD;
    else return KIND_UNKNOWN;
}

int Item::str2ItemKind(QString text) {
    return str2ItemKind(text.toStdString());
}
