#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "options.h"
#include <QDebug>
#include "flowlayout.h"
#include "qimagebox.h"
#include "Core/exception/get_process_error.h"
#include "Core/exception/get_task_error.h"
#include "Core/exception/read_memory_error.h"
#include "Core/exception/not_in_game.h"
#include <assert.h>
#include <QDesktopServices>
#include "about.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ItemPane = new QFrame(this);
    ItemPane->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->ItemPaneScrollArea->setWidget(ItemPane);

    ItemPane->setLayout(new FlowLayout(-1, 0, 0));

    inquirer = new Inquirer();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(fetchdata()));
    timer->start(500);

    lastRoomItems = new ItemList();
    lastPlayerItems = new ItemList();

    translators[0] = new QTranslator(this);
    translators[0]->load(":/translations/translations/grimoire_en.ts");
    translators[1] = new QTranslator(this);
    translators[1]->load(":/translations/translations/grimoire_ch.ts");

    settings = new QSettings("MorusLeaf", "IsaacGrimoire", this);
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::saveSettings() {
    settings->beginGroup("MainWindow");
    settings->setValue("MainWindowGeometry", geometry());
    QList<int> sizes = ui->splitter->sizes();
    settings->setValue("ItemPaneSize", sizes.at(0));
    settings->setValue("InfoPaneSize", sizes.at(1));
    settings->setValue("Language", language);
    settings->setValue("HideStatPane", !ui->actionStatistics->isChecked());
    settings->setValue("LaunchFromSteam", launchFromSteam);
    settings->setValue("LaunchPath", launchPath);
    settings->endGroup();
}

void MainWindow::loadSettings() {
    settings->beginGroup("MainWindow");
    // window size
    QRect MainWindowRect = settings->value("MainWindowGeometry").toRect();
    if (!MainWindowRect.isEmpty())
        setGeometry(MainWindowRect);
    QRect ItemPaneRect = settings->value("ItemPaneGeometry").toRect();
    if (!ItemPaneRect.isEmpty())
        ui->ItemPaneScrollArea->setGeometry(ItemPaneRect);
    int ItemPaneSize = settings->value("ItemPaneSize", -1).toInt();
    int InfoPaneSize = settings->value("InfoPaneSize", -1).toInt();
    if (ItemPaneSize != -1) {
        QList<int> sizes;
        sizes << ItemPaneSize << InfoPaneSize;
        ui->splitter->setSizes(sizes);
    }
    // hide/show StatPane
    bool hideStatPane = settings->value("HideStatPane", false).toBool();
    if (hideStatPane) {
        ui->actionStatistics->setChecked(false);
        ui->StatPane->hide();
    }
    else {
        ui->actionStatistics->setChecked(true);
        ui->StatPane->show();
    }
    // language
    language = settings->value("Language", 0).toInt();
    qApp->installTranslator(translators[language]);
    ui->retranslateUi(this);
    ui->comboBox_language->setCurrentIndex(language);
    // launch
    launchFromSteam = settings->value("LaunchFromSteam", false).toBool();
    launchPath = settings->value("LaunchPath", "").toString();
    settings->endGroup();
}

void MainWindow::on_actionAbout_triggered()
{
    About about(this);
    about.exec();
}

void MainWindow::on_actionOptions_triggered()
{
    saveSettings();
    Options options(settings, this);
    int ret = options.exec();
    if (ret) {
        loadSettings();
        resetChoosingItem();
    }
}

void MainWindow::on_LaunchButton_clicked()
{
    if (launchFromSteam) {
        QDesktopServices::openUrl(QUrl("steam://rungameid/250900"));
    }
    else {
        QProcess *process = new QProcess(this);
        process->start("\"" + launchPath + "\"");
    }
}

void MainWindow::on_actionReport_Bugs_triggered()
{
    QDesktopServices::openUrl(QUrl("mailto:morusleaf@gmail.com"));
}

void MainWindow::on_actionStatistics_triggered()
{
    if (ui->actionStatistics->isChecked()) {
        ui->StatPane->show();
    }
    else {
        ui->StatPane->hide();
    }
}

void MainWindow::updateStatPane() {
    int attackStyle, attackDelay;
    float attackDamage, attackRange, shotSpeed, shotHeight, moveSpeed, luck;
    bool ret;
    try {
        ret = inquirer->getStat(attackStyle, attackDamage, attackDelay, attackRange, shotSpeed, shotHeight, moveSpeed, luck);
    }
    catch (...) {
        throw;
    }

    if (ret) {
        ui->stat_attackdamage->setText(QString::number(attackDamage));
        ui->stat_attackdelay->setText(QString::number(attackDelay));
        ui->stat_attackrange->setText(QString::number(attackRange));
        ui->stat_shotspeed->setText(QString::number(shotSpeed));
        ui->stat_shotheight->setText(QString::number(shotHeight));
        ui->stat_movespeed->setText(QString::number(moveSpeed));
        ui->stat_luck->setText(QString::number(luck));
    }
    else
        report_error("can not read statistics");
}

static bool listEqual(ItemList* v1, ItemList* v2) {
    if (v1 == NULL || v2 == NULL) return false;
    if (v1->size() != v2->size()) return false;
    unsigned int n = v1->size();
    return std::equal(v1->begin(), v1->begin() + n, v2->begin());
}

void MainWindow::updateItemPane() {
    ItemList *newRoomItems = new ItemList();
    ItemList *newPlayerItems = new ItemList();
    ImageList roomImgs;
    ImageList playerImgs;
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
    catch (...) {
        delete newRoomItems;
        delete newPlayerItems;
        throw;
    }

    if (!ret) {
        report_error("can not read items");
        return;
    }

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
    refreshItemPane(&roomImgs, &playerImgs, !roomListNotChanged);
}

void MainWindow::refreshItemPane(ImageList *roomImgs, ImageList *playerImgs, bool roomListChanged) {
    // clear ItemPane
    QLayoutItem *child;
    while ((child = ItemPane->layout()->takeAt(0)) != 0) {
        child->widget()->setParent(NULL);
        delete child;
    }

    assert(lastRoomItems->size() == roomImgs->size());
    assert(lastPlayerItems->size() == playerImgs->size());
    int roomListSize = lastRoomItems->size();
    for (int i = 0; i < roomListSize; i++) {
        Item *item = lastRoomItems->at(i);
        QImageBox *itembox = new QImageBox(item);
        itembox->setImage(roomImgs->at(i));
        connect(itembox, SIGNAL(chooseImageBox(Item*)), this, SLOT(onChooseImageBoxTriggered(Item*)));
        ItemPane->layout()->addWidget(itembox);
        if (i == 0 && roomListChanged) {
            itembox->setChosen();
        }
    }
    int playerListSize = lastPlayerItems->size();
    bool chooseFirstPlayerItem = (roomListSize == 0 || !roomListChanged);
    for (int i = 0; i < playerListSize; i++) {
        Item *item = lastPlayerItems->at(i);
        QImageBox *itembox = new QImageBox(item);
        itembox->setImage(playerImgs->at(i), true);
        connect(itembox, SIGNAL(chooseImageBox(Item*)), this, SLOT(onChooseImageBoxTriggered(Item*)));
        ItemPane->layout()->addWidget(itembox);
        if (i == 0 && chooseFirstPlayerItem) {
            itembox->setChosen();
        }
    }
}

void MainWindow::fetchdata() {
    try {
        updateItemPane();
        updateStatPane();

    }
    catch (get_task_error) {
        report_error("can not attach to game process");
    }
    catch (get_process_error& e) {
        report_error("can not find game process" + QString::fromStdString(e.what()));
    }
    catch (read_memory_error) {
        report_error("error occurred during reading memory");
    }
    catch (not_in_game) {
        report_error("not in game");
    }
    catch (...) {
        report_error("unknown error");
    }
}

void MainWindow::report_error(QString text) {
    updateStatusBar(text);
}

void MainWindow::updateStatusBar(QString text) {
    ui->statusBar->showMessage(text, 500);
}

void MainWindow::onChooseImageBoxTriggered(Item *item) {
    int layoutsize = ItemPane->layout()->count();
    for (int i=0; i<layoutsize; i++) {
        ((QImageBox*)ItemPane->layout()->itemAt(i)->widget())->setFrameStyle(QFrame::Plain);
    }
    ui->ItemTitle->setText(item->getTitle());
    ui->ItemDescription->setText(item->getDescription(language));
}

void MainWindow::resetChoosingItem() {
    int layoutsize = ItemPane->layout()->count();
    for (int i=0; i<layoutsize; i++) {
        ((QImageBox*)ItemPane->layout()->itemAt(i)->widget())->setFrameStyle(QFrame::Plain);
    }
    ui->ItemTitle->clear();
    ui->ItemDescription->clear();
}

void MainWindow::on_comboBox_language_activated(int index)
{
    language = index;
    qApp->installTranslator(translators[language]);
    ui->retranslateUi(this);
    resetChoosingItem();
    ui->comboBox_language->setCurrentIndex(language);
}
