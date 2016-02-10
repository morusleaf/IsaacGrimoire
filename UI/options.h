#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QSettings *settings, QWidget *parent = 0);
    ~Options();
private slots:
    void on_buttonBox_accepted();

    void on_comboBox_language_activated(int index);

    void on_radioButton_fromsteam_clicked();

    void on_radioButton_custompath_clicked();

    void on_toolButton_choosepath_clicked();

private:
    Ui::Options *ui;
    QSettings *settings;
    int language;
    bool launchFromSteam;
    QString launchPath;
    void loadSettings();
    void saveSettings();
};

#endif // OPTIONS_H
