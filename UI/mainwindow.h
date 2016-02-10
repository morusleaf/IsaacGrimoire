#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Core/Inquirer.h"
#include <QFrame>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_LaunchButton_clicked();
    void on_actionAbout_triggered();
    void on_actionOptions_triggered();
    void on_actionStatistics_triggered();
    void on_actionReport_Bugs_triggered();
    void fetchdata();
    void onChooseImageBoxTriggered(Item *item);
    void on_comboBox_language_activated(int index);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    Inquirer *inquirer;
    QFrame *ItemPane;
    void updateStatPane();
    void updateItemPane();
    void refreshItemPane(ImageList *roomImgs, ImageList *playerImgs, bool roomListChanged);
    ItemList *lastRoomItems;
    ItemList *lastPlayerItems;
    void updateStatusBar(QString text);
    void report_error(QString text);
    QTranslator *translators[2];
    int language;
    QSettings *settings;
    void resetChoosingItem();
    void saveSettings();
    void loadSettings();
    bool launchFromSteam;
    QString launchPath;
};

#endif // MAINWINDOW_H
