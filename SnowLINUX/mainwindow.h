#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QString>
#include <QTimer>
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
//    void slotDelMissionBar();/
    void slotUpdateSelectTable();
    void slotDelMission();
    void slotPauseMission();
    void slotContMission();
    void slotRestMission();
//    void slotSetGlobalURL(const QString&);
//    void slotSetGlobalPath(const QString&);
//    void slotSetGlobalNum(int);
//    void slotSetGlobalDir();
    void slotPlotWave();

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;
    //QString m_szPath;

private:
    QTimer*timer;

protected:
    void paintEvent(QPaintEvent*);

};

#endif // MAINWINDOW_H
