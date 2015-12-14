#ifndef MISSIONCHECK_H
#define MISSIONCHECK_H


#include <QCheckBox>

class MissionCheck:public QCheckBox{

    Q_OBJECT

public slots:

public:
    MissionCheck(QWidget*parent,int cidx);
    ~MissionCheck();
    void adjustPosition();
private:
    int compIndex;

};



#endif // MISSIONCHECK_H

