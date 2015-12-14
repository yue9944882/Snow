#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QString>
#include "missionbar.h"
#include "missioncheck.h"
//#include "newdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:

//    void slotNewMission();
    void slotNewMission();
    void slotNewMissionBar();
    void slotSetNextMissionDir();
//    void slotNewMissionThread();
//    void slotDelMissionBar();
//    void slotUpdateSelectTable();
//    void slotSetGlobalURL(const QString&);
//    void slotSetGlobalPath(const QString&);
//    void slotSetGlobalNum(int);
//    void slotSetGlobalDir();

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;
    //QString m_szPath;

private:

};

#endif // MAINWINDOW_H
