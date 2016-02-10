#include "about.h"
#include "ui_about.h"
#include "Core/version.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->label_plugin_version->setText(tr("Version: %1").arg(PLUGIN_VERSION));
    ui->label_game_version->setText(tr("Game Version: %1").arg(GAME_VERSION));
}

About::~About()
{
    delete ui;
}
