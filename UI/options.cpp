#include "options.h"
#include "ui_options.h"
#include <QDebug>
#include <QFileDialog>

Options::Options(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options),
    settings(settings)
{
    ui->setupUi(this);
    loadSettings();
    ui->radioButton_fromsteam->setChecked(launchFromSteam);
    ui->radioButton_custompath->setChecked(!launchFromSteam);
    ui->lineEdit_path->setEnabled(!launchFromSteam);
    ui->toolButton_choosepath->setEnabled(!launchFromSteam);
    ui->lineEdit_path->setText(launchPath);
    ui->comboBox_language->setCurrentIndex(language);
}

Options::~Options()
{
    delete ui;
}

void Options::loadSettings() {
    settings->beginGroup("MainWindow");
    launchFromSteam = settings->value("LaunchFromSteam", false).toBool();
    launchPath = settings->value("LaunchPath", "").toString();
    language = settings->value("Language", 0).toInt();
    settings->endGroup();
}

void Options::saveSettings() {
    launchFromSteam = ui->radioButton_fromsteam->isChecked();
    launchPath = ui->lineEdit_path->text();
    language = ui->comboBox_language->currentIndex();
    settings->beginGroup("MainWindow");
    settings->setValue("LaunchFromSteam", launchFromSteam);
    settings->setValue("LaunchPath", launchPath);
    settings->setValue("Language", language);
    settings->endGroup();
}

void Options::on_buttonBox_accepted()
{
    saveSettings();
}

void Options::on_comboBox_language_activated(int index)
{
    language = index;
}

void Options::on_radioButton_fromsteam_clicked()
{
    ui->lineEdit_path->setEnabled(false);
    ui->toolButton_choosepath->setEnabled(false);
}

void Options::on_radioButton_custompath_clicked()
{
    ui->lineEdit_path->setEnabled(true);
    ui->toolButton_choosepath->setEnabled(true);
}

void Options::on_toolButton_choosepath_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select game executable"), ui->lineEdit_path->text());
    if (!filename.isEmpty())
        ui->lineEdit_path->setText(filename);
}
