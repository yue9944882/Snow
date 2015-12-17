
#include "missioncheck.h"
#include "global.h"

MissionCheck::MissionCheck(QWidget*parent,int cidx):QCheckBox(parent){
    this->compIndex=cidx;
}

void MissionCheck::adjustPosition(){
    this->move(2,10+45*this->compIndex);
}


MissionCheck::~MissionCheck(){

}

void MissionCheck::slotChangeGlobalIndex(){
    g_clickIndex=this->compIndex;
    pthread_mutex_unlock(&clickMutex);
}
