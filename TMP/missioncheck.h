#ifndef MISSIONCHECK_H
#define MISSIONCHECK_H


#include <QCheckBox>

class MissionCheck:public QCheckBox{

    Q_OBJECT

public slots:
    void slotChangeGlobalIndex();
public:
    MissionCheck(QWidget*parent,int cidx);
    ~MissionCheck();
    void adjustPosition();
    int compIndex;
private:
};



#endif // MISSIONCHECK_H

