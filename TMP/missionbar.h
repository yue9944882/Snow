#ifndef MISSIONBAR_H
#define MISSIONBAR_H

#include <QProgressBar>
#include <QLabel>
#include <QTimer>

class MissionBar:public QWidget{

    Q_OBJECT

public slots:
    void slotUpdateProgress();

public:
    MissionBar(QWidget*parent,int cidx);
    ~MissionBar();
    void adjustPosition();
    void setProgressBar(double value);
    int compIndex;
    QTimer*timer;
private:
    QProgressBar*progressBar;
    QLabel*staticLabel;
    QLabel*timeBar;
    QLabel*nameBar;
    QLabel*sizeBar;
    int timer_id;

    QString setTimeFormat(long);
protected:
    //void timerEvent(QTimerEvent *);
};


#endif // MISSIONBAR_H

